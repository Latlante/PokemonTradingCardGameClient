#include "modellistenergies.h"
#include <QtQml/qqml.h>

ModelListEnergies::ModelListEnergies(QObject *parent) :
    QAbstractListModel(parent),
    m_listEnergies(QList<CardEnergy*>())
{

}

ModelListEnergies::~ModelListEnergies()
{
    while(m_listEnergies.count() > 0)
        delete m_listEnergies.takeFirst();
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelListEnergies::declareQML()
{
    qmlRegisterUncreatableType<ModelListEnergies>("model", 1, 0, "ModelListEnergies", "ModelListEnergies error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ModelListEnergies::addEnergy(CardEnergy *energy)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //qDebug() << __PRETTY_FUNCTION__ << rowCount() << energy->quantity() << energy->element();
    if(energy != nullptr)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount()+(energy->quantity()-1));
        m_listEnergies.append(energy);
        endInsertRows();
    }
}

CardEnergy* ModelListEnergies::takeEnergy(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    CardEnergy* energyToReturn = nullptr;

    if((index >= 0) && (index < rowCount()))
    {
        energyToReturn = energy(index);
        beginRemoveRows(QModelIndex(), index, index+(energyToReturn->quantity()-1));
        m_listEnergies.removeOne(energyToReturn);
        endRemoveRows();
    }

    return energyToReturn;
}

CardEnergy* ModelListEnergies::energy(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    CardEnergy* energyToReturn = nullptr;

    if((index >= 0) && (index < rowCount()))
    {
        energyToReturn = m_listEnergies[index];
    }

    return energyToReturn;
}

void ModelListEnergies::removeEnergy(CardEnergy *energy)
{
    if(energy != nullptr)
    {
        int index = m_listEnergies.indexOf(energy);

        if(index >= 0)
        {
            beginRemoveRows(QModelIndex(), index, index+(energy->quantity()-1));
            m_listEnergies.removeOne(energy);
            endRemoveRows();
        }
    }
}

QList<CardEnergy*> ModelListEnergies::takeAllEnergies()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QList<CardEnergy*> listToReturn;

    /*for(int i=0;i<rowCount();++i)
        listToReturn.append(takeEnergy(i));*/

    while(m_listEnergies.count() > 0)
        listToReturn.append(takeEnergy(0));

    return listToReturn;
}

unsigned short ModelListEnergies::countEnergies()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    unsigned short count = 0;

    foreach (CardEnergy* energy, m_listEnergies)
    {
        count += energy->quantity();
    }

    return count;
}

unsigned short ModelListEnergies::countEnergies(AbstractCard::Enum_element element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    int count = 0;

    foreach (CardEnergy* energy, m_listEnergies)
    {
        if (energy->element() == element)
            count += energy->quantity();
    }

    return count;
}

bool ModelListEnergies::hasEnoughEnergies(QMap<AbstractCard::Enum_element, unsigned short> cost)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    bool statusBack = true;

    if (cost.count() > 0)
    {
        QMap<AbstractCard::Enum_element, unsigned short> mapEnergiesAssigned;

        //Création d'une copie des énergies assignées similaire au coût de l'attaque
        //pour faciliter le traitement
        foreach(CardEnergy* energy, m_listEnergies)
        {
            AbstractCard::Enum_element elementEnergy = energy->element();
            unsigned short quantity = mapEnergiesAssigned[elementEnergy];
            quantity += energy->quantity();
            mapEnergiesAssigned[elementEnergy] = quantity;
        }

        //On commence par retirer toutes les énergies nécessaires à l'exception des normales
        foreach (AbstractCard::Enum_element element, cost.keys())
        {
            if(element != AbstractCard::Element_Normal)
            {
                if (mapEnergiesAssigned.value(element) >= cost.value(element))
                {
                    unsigned short quantity = mapEnergiesAssigned[element];
                    quantity -= cost.value(element);
                    mapEnergiesAssigned[element] = quantity;
                }
                else
                {
                    statusBack = false;
                    break;
                }
            }
        }

        //Si l'attaque nécessite des énergies "normales", on regarde ce qu'il reste
        if((statusBack == true) && (cost.keys().contains(AbstractCard::Element_Normal)))
        {
            unsigned short countEnergiesLeft = 0;
            foreach (AbstractCard::Enum_element element, mapEnergiesAssigned.keys())
                countEnergiesLeft += mapEnergiesAssigned.value(element);

            if(countEnergiesLeft < cost.value(AbstractCard::Element_Normal))
                statusBack = false;
        }
    }

    return statusBack;
}

int ModelListEnergies::countCard()
{
    return m_listEnergies.count();
}

QVariant ModelListEnergies::data(const QModelIndex &index, int role) const
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //qDebug() << __PRETTY_FUNCTION__ << index << role;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    int indexRow = 0;
    int quantityOfEnergies = 0;
    while((indexRow < m_listEnergies.count()) && (iRow >= quantityOfEnergies))
    {
        quantityOfEnergies += m_listEnergies[indexRow]->quantity();
        indexRow++;
    }

    switch(role)
    {
    case ListEnergiesRole_Icon:         return "energies/icones/" + QString::number(static_cast<int>(m_listEnergies[indexRow-1]->element())) + ".png";
    }

    return QVariant();
}

int ModelListEnergies::rowCount(const QModelIndex &) const
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    int count = 0;

    foreach (CardEnergy* energy, m_listEnergies)
    {
        count += energy->quantity();
    }

    //qDebug() << __PRETTY_FUNCTION__ << count;

    return count;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelListEnergies::roleNames() const
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QHash<int, QByteArray> roles;
    roles[ListEnergiesRole_Icon] = "icon";

    return roles;
}

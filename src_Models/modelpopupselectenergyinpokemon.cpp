#include "modelpopupselectenergyinpokemon.h"
#include <QtQml/qqml.h>

#include "src_Cards/cardpokemon.h"
#include "src_Models/modellistenergies.h"

ModelPopupSelectEnergyInPokemon::ModelPopupSelectEnergyInPokemon(QObject *parent) :
    QAbstractListModel(parent),
    m_listEnergies(QList<SelectionCards>()),
    m_numberOfEnergiesToSelect(1),
    m_elementFilter(AbstractCard::Element_Whatever)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelPopupSelectEnergyInPokemon::declareQML()
{
    qmlRegisterUncreatableType<ModelPopupSelectEnergyInPokemon>("model", 1, 0, "ModePopupSelectEnergyInPokemon", "ModePopupSelectEnergyInPokemon error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ModelPopupSelectEnergyInPokemon::setElementFilter(AbstractCard::Enum_element element)
{
    m_elementFilter = element;
}

void ModelPopupSelectEnergyInPokemon::addListEnergyFromPokemon(CardPokemon *pokemon)
{
    if((pokemon != nullptr) && (pokemon->modelListOfEnergies() != nullptr))
    {
        cleanPacket();
        ModelListEnergies* modelEnergies = pokemon->modelListOfEnergies();

        for(int i=0;i<modelEnergies->countCard();++i)
        {
            CardEnergy* cardEn = modelEnergies->energy(i);

            if((cardEn->element() == m_elementFilter) || (m_elementFilter == AbstractCard::Element_Whatever))
            {
                SelectionCards selection;
                selection.card = cardEn;
                selection.selected = false;

                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_listEnergies.append(selection);
                endInsertRows();
            }
        }
    }
}

unsigned short ModelPopupSelectEnergyInPokemon::numberOfEnergiesToSelect()
{
    return m_numberOfEnergiesToSelect;
}

void ModelPopupSelectEnergyInPokemon::setNumberOfEnergiesToSelect(unsigned short numberToSelect)
{
    m_numberOfEnergiesToSelect = numberToSelect;
}

unsigned short ModelPopupSelectEnergyInPokemon::numberOfEnergiesSelected()
{
    unsigned short countSelected = 0;

    foreach(SelectionCards selection, m_listEnergies)
    {
        countSelected += selection.selected ? selection.card->quantity() : 0;
    }

    return countSelected;
}

bool ModelPopupSelectEnergyInPokemon::isMaximumEnergiesSelected()
{
    return numberOfEnergiesSelected() >= numberOfEnergiesToSelect();
}

QList<AbstractCard*> ModelPopupSelectEnergyInPokemon::listIndexEnergiesSelected()
{
    QList<AbstractCard*> listCards;

    for(int i=0;i<m_listEnergies.count();++i)
    {
        if(m_listEnergies.at(i).selected == true)
        {
            listCards.append(m_listEnergies.at(i).card);
        }
    }

    return listCards;
}

QVariant ModelPopupSelectEnergyInPokemon::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__ << index << role;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    switch(role)
    {
    case SelectCardsRole_ImageCard:     return m_listEnergies[iRow].card->image();
    case SelectCardsRole_Selected:      return m_listEnergies[iRow].selected;
    }

    return QVariant();
}

bool ModelPopupSelectEnergyInPokemon::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return false;
    }

    switch(role)
    {
    //case SelectCardsRole_ImageCard:     return m_listCards[iRow].card->image();
    case SelectCardsRole_Selected:
        if(value.type() == QVariant::Bool)
        {
            bool isSelected = value.toBool();
            bool ignoreSelection = ((isSelected == true) && (isMaximumEnergiesSelected() == true));

            if(ignoreSelection == false)
            {
                SelectionCards selection = m_listEnergies[iRow];
                selection.selected = isSelected;
                m_listEnergies.replace(iRow, selection);
                emit dataChanged(index, index);
                emit numberOfEnergiesSelectedChanged();
            }
        }
        else
            return false;
    }

    return true;
}

int ModelPopupSelectEnergyInPokemon::rowCount(const QModelIndex &) const
{
    return m_listEnergies.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelPopupSelectEnergyInPokemon::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SelectCardsRole_ImageCard] = "imageCard";
    roles[SelectCardsRole_Selected] = "selected";

    return roles;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void ModelPopupSelectEnergyInPokemon::cleanPacket()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_listEnergies.clear();
    endRemoveRows();

    emit numberOfEnergiesSelectedChanged();
}

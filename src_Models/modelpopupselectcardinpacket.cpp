#include "modelpopupselectcardinpacket.h"
#include <QtQml/qqml.h>

#include "src_Models/modellistenergies.h"
#include "src_Packets/abstractpacket.h"

ModelPopupSelectCardInPacket::ModelPopupSelectCardInPacket(QObject *parent) :
    QAbstractListModel(parent),
    m_listCards(QList<SelectionCards>()),
    m_numberOfCardsToSelect(1),
    m_typeOfCardFilter(AbstractCard::TypeOfCard_Whatever)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelPopupSelectCardInPacket::declareQML()
{
    qmlRegisterUncreatableType<ModelPopupSelectCardInPacket>("model", 1, 0, "ModelPopupSelectCardInPacket", "ModelPopupSelectCardInPacket error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ModelPopupSelectCardInPacket::setTypeOfCardFilter(AbstractCard::Enum_typeOfCard typeOfCard)
{
    m_typeOfCardFilter = typeOfCard;
}

void ModelPopupSelectCardInPacket::addPacketFromAbstractPacket(AbstractPacket *packet)
{
    if(packet != nullptr)
    {
        cleanPacket();

        for(int i=0;i<packet->countCard();++i)
        {
            AbstractCard *abCard = packet->card(i);
            if((abCard->type() == m_typeOfCardFilter) || (m_typeOfCardFilter == AbstractCard::TypeOfCard_Whatever))
            {
                SelectionCards selection;
                selection.card = packet->card(i);
                selection.selected = false;
                selection.flipped = false;

                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_listCards.append(selection);
                endInsertRows();
            }
        }
    }
}

void ModelPopupSelectCardInPacket::addPacketFromModelListEnergies(ModelListEnergies *model)
{
    if(model != nullptr)
    {
        cleanPacket();

        for(int i=0;i<model->rowCount();++i)
        {
            SelectionCards selection;
            selection.card = model->energy(i);
            selection.selected = false;
            selection.flipped = false;

            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_listCards.append(selection);
            endInsertRows();
        }
    }
}

unsigned short ModelPopupSelectCardInPacket::numberOfCardsToSelect()
{
    return m_numberOfCardsToSelect;
}

void ModelPopupSelectCardInPacket::setNumberOfCardsToSelect(unsigned short numberToSelect)
{
    m_numberOfCardsToSelect = numberToSelect;
}

unsigned short ModelPopupSelectCardInPacket::numberOfCardsSelected()
{
    unsigned short countSelected = 0;

    foreach(SelectionCards selection, m_listCards)
    {
        countSelected += selection.selected ? 1 : 0;
    }

    return countSelected;
}

bool ModelPopupSelectCardInPacket::isMaximumCardsSelected()
{
    return numberOfCardsSelected() >= numberOfCardsToSelect();
}

QList<AbstractCard *> ModelPopupSelectCardInPacket::listCardsSelected()
{
    QList<AbstractCard*> listCards;

    for(int i=0;i<m_listCards.count();++i)
    {
        if(m_listCards.at(i).selected == true)
        {
            listCards.append(m_listCards.at(i).card);
        }
    }

    return listCards;
}

void ModelPopupSelectCardInPacket::flipIfSelected()
{
    for(int i=0;i<m_listCards.count();++i)
    {
        SelectionCards selection = m_listCards[i];

        if(selection.selected == true)
        {
            selection.flipped = true;
            m_listCards.replace(i, selection);

            //QVector<int> listRoles = QVector<int>() << SelectCardsRole_Flip;
            emit dataChanged(index(i, 0), index(i, 0), {SelectCardsRole_Flip});
        }
    }
}

QVariant ModelPopupSelectCardInPacket::data(const QModelIndex &index, int role) const
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
    case SelectCardsRole_ImageCard:     return m_listCards[iRow].card->image();
    case SelectCardsRole_Selected:      return m_listCards[iRow].selected;
    case SelectCardsRole_Flip:          return m_listCards[iRow].flipped;
    }

    return QVariant();
}

bool ModelPopupSelectCardInPacket::setData(const QModelIndex &index, const QVariant &value, int role)
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
            bool ignoreSelection = ((isSelected == true) && (isMaximumCardsSelected() == true));

            if(ignoreSelection == false)
            {
                SelectionCards selection = m_listCards[iRow];
                selection.selected = isSelected;
                m_listCards.replace(iRow, selection);
                emit dataChanged(index, index);
                emit numberOfCardsSelectedChanged();
            }
        }
        else
            return false;
    }

    return true;
}

int ModelPopupSelectCardInPacket::rowCount(const QModelIndex &) const
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__ << m_listCards.count();
#endif

    return m_listCards.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelPopupSelectCardInPacket::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SelectCardsRole_ImageCard] = "imageCard";
    roles[SelectCardsRole_Selected] = "selected";
    roles[SelectCardsRole_Flip] = "flip";

    return roles;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void ModelPopupSelectCardInPacket::cleanPacket()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_listCards.clear();
    endRemoveRows();

    emit numberOfCardsSelectedChanged();
}

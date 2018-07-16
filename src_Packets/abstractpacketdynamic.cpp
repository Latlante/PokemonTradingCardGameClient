#include "abstractpacketdynamic.h"

#include "src_Cards/abstractcard.h"

AbstractPacketDynamic::AbstractPacketDynamic(const QString &namePacket, QList<AbstractCard *> listCards) :
    AbstractPacket(namePacket, listCards)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int AbstractPacketDynamic::countCard() const
{
    return rowCount();
}

bool AbstractPacketDynamic::addNewCard(AbstractCard* newCard)
{
    //qDebug() << __PRETTY_FUNCTION__;
    bool statusBack = false;

    if ((NULL != newCard) && (!isFull()))
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_listCards.append(newCard);
        endInsertRows();

        connect(newCard, &AbstractCard::dataChanged, this, &AbstractPacketDynamic::updateAllData);

        emit countChanged();
        statusBack = true;
    }

    return statusBack;
}

AbstractCard* AbstractPacketDynamic::takeACard(int indexCard)
{
    AbstractCard* card = NULL;

    if ((indexCard >= 0) && (indexCard < countCard()))
    {
        beginRemoveRows(QModelIndex(), indexCard, indexCard);
        card = m_listCards.takeAt(indexCard);
        endRemoveRows();

        emit countChanged();
    }

    return card;
}

bool AbstractPacketDynamic::removeFromPacketWithoutDelete(AbstractCard *card)
{
    bool removeSuccess = false;

    int indexCard = m_listCards.indexOf(card);
    if(indexCard != -1)
    {
        beginRemoveRows(QModelIndex(), indexCard, indexCard);
        removeSuccess = m_listCards.removeOne(card);
        endRemoveRows();

        //On ne delete pas la carte volontairement

        emit countChanged();
    }

    return removeSuccess;
}

int AbstractPacketDynamic::rowCount(const QModelIndex&) const
{
    return m_listCards.count();
}

QVariant AbstractPacketDynamic::data(const QModelIndex &index, int role) const
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
    case PacDynamicRole_Card:         return QVariant::fromValue<AbstractCard*>(m_listCards[iRow]);
    case PacDynamicRole_Name:         return m_listCards[iRow]->name();
    case PacDynamicRole_ImageCard:    return m_listCards[iRow]->image();
    }

    return QVariant();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> AbstractPacketDynamic::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PacDynamicRole_Card] = "card";
    roles[PacDynamicRole_Name] = "name";
    roles[PacDynamicRole_ImageCard] = "imageCard";

    return roles;
}


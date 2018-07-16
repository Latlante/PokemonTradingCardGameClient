#include "abstractpacketstatic.h"
#include "src_Cards/abstractcard.h"

#include "src_Cards/cardpokemon.h"

AbstractPacketStatic::AbstractPacketStatic(const QString &namePacket, QList<AbstractCard *> listCards) :
    AbstractPacket(namePacket, listCards)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int AbstractPacketStatic::countCard() const
{
    return m_listCards.count();
}

bool AbstractPacketStatic::addNewCard(AbstractCard* newCard)
{
    bool statusBack = false;

    if ((NULL != newCard) && (!isFull()))
    {
        m_listCards.append(newCard);

        connect(newCard, &AbstractCard::dataChanged, this, &AbstractPacketStatic::updateAllData);

        emit dataChanged(index(countCard()-1, 0), index(countCard()-1, 0));
        emit countChanged();
        statusBack = true;
    }

    return statusBack;
}

AbstractCard* AbstractPacketStatic::takeACard(int indexCard)
{
    AbstractCard* card = NULL;

    if ((indexCard >= 0) && (indexCard < countCard()))
    {
        card = m_listCards.takeAt(indexCard);

        emit dataChanged(index(indexCard, 0), index(countCard(), 0));
        emit countChanged();
    }

    return card;
}

bool AbstractPacketStatic::removeFromPacketWithoutDelete(AbstractCard *card)
{
    bool removeSuccess = false;

    int indexCard = m_listCards.indexOf(card);
    if(indexCard != -1)
    {
        removeSuccess = m_listCards.removeOne(card);

        //On ne delete pas la carte volontairement

        emit dataChanged(index(indexCard, 0), index(countCard(), 0));
        emit countChanged();
    }

    return removeSuccess;
}

QVariant AbstractPacketStatic::data(const QModelIndex& index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__ << index << role;

    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    //Dans la liste
    if(iRow < countCard())
    {
        CardPokemon* cardPok = dynamic_cast<CardPokemon*>(m_listCards[iRow]);

        if(cardPok != nullptr)
        {
            switch(role)
            {
            case PacStaticRole_Card:            return QVariant::fromValue<AbstractCard*>(m_listCards[iRow]);
            case PacStaticRole_IsCard:          return true;
            case PacStaticRole_Name:            return m_listCards[iRow]->name();
            case PacStaticRole_ImageCard:       return m_listCards[iRow]->image();
            }
        }
        else
        {
            return QVariant::Invalid;
        }

    }
    //Au delà
    else
    {
        switch(role)
        {
        case PacStaticRole_Card:            return QVariant();
        case PacStaticRole_IsCard:          return false;
        case PacStaticRole_Name:            return "";
        case PacStaticRole_ImageCard:       return AbstractCard::imageByDefault();
        }
    }

    return QVariant::Invalid;
}


int AbstractPacketStatic::rowCount(const QModelIndex&) const
{
    return maxCards();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> AbstractPacketStatic::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PacStaticRole_Card] = "card";
    roles[PacStaticRole_IsCard] = "isCard";
    roles[PacStaticRole_Name] = "name";
    roles[PacStaticRole_ImageCard] = "imageCard";

    return roles;
}

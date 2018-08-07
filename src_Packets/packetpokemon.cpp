#include "packetpokemon.h"
#include <QDebug>

#include "src_Cards/cardpokemon.h"

PacketPokemon::PacketPokemon(const QString &namePacket, QList<AbstractCard *> listCards) :
    AbstractPacket(namePacket)/*,
    m_listCards(listCards)*/
{
    m_listCards = listCards;
}

PacketPokemon::~PacketPokemon()
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketPokemon::declareQML()
{
    qmlRegisterUncreatableType<PacketPokemon>("model", 1, 0, "PacketPokemon", "PacketPokemon cannot be created.");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractPacket::TypeOfPacket PacketPokemon::type()
{
    return AbstractPacket::Packet_Pokemon;
}

int PacketPokemon::countCard() const
{
    return rowCount();
}

bool PacketPokemon::addNewCard(AbstractCard* newCard)
{
    if(newCard != nullptr)
    {
        m_listCards.append(newCard);
        emit countChanged();
    }
}

AbstractCard* PacketPokemon::takeACard(int indexCard)
{
    AbstractCard *cardToReturn = nullptr;

    if((indexCard >= 0) && (indexCard < countCard()))
    {
        cardToReturn = m_listCards.takeAt(indexCard);
        emit countChanged();
    }

    return cardToReturn;
}

bool PacketPokemon::remove(AbstractCard* card)
{
    bool success = false;

    if(card != nullptr)
    {
        if(m_listCards.contains(card))
        {
            m_listCards.removeOne(card);
            emit countChanged();
            success = true;
        }
    }

    return success;
}

QVariant PacketPokemon::data(const QModelIndex &index, int role) const
{
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
            case PacketPokemon::ROLE_CARD:            return QVariant::fromValue<AbstractCard*>(m_listCards[iRow]);
            case PacketPokemon::ROLE_ISCARD:          return true;
            case PacketPokemon::ROLE_NAME:            return m_listCards[iRow]->name();
            case PacketPokemon::ROLE_IMAGECARD:       return m_listCards[iRow]->image();
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
        case PacketPokemon::ROLE_CARD:            return QVariant();
        case PacketPokemon::ROLE_ISCARD:          return false;
        case PacketPokemon::ROLE_NAME:            return "";
        case PacketPokemon::ROLE_IMAGECARD:       return AbstractCard::imageByDefault();
        }
    }

    return QVariant::Invalid;
}

int PacketPokemon::rowCount(const QModelIndex&) const
{
    return MAXCARDS_BENCH;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> PacketPokemon::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PacketPokemon::ROLE_CARD] = "card";
    roles[PacketPokemon::ROLE_ISCARD] = "isCard";
    roles[PacketPokemon::ROLE_NAME] = "name";
    roles[PacketPokemon::ROLE_IMAGECARD] = "imageCard";

    return roles;
}

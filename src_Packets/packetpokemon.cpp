#include "packetpokemon.h"
#include <QDebug>
#include <QQmlEngine>

#include "src_Cards/cardpokemon.h"

PacketPokemon::PacketPokemon(const QString &namePacket, unsigned int maxCard, QList<AbstractCard *> listCards) :
    AbstractPacket(namePacket),
    m_maxCard(maxCard)
{
    m_listCards = listCards;
}

PacketPokemon::~PacketPokemon()
{
    qDebug() << "PacketPokemon destroy";
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
CardPokemon* PacketPokemon::pokemon(int index)
{
    qDebug() << __PRETTY_FUNCTION__;

    CardPokemon* pokemonToReturn = nullptr;

    if((index >= 0) && (index < countCard()))
    {
        AbstractCard* abCard = card(index);

        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            pokemonToReturn = static_cast<CardPokemon*>(abCard);
        }
    }

    if(pokemonToReturn != nullptr)
        qDebug() << __PRETTY_FUNCTION__ << pokemonToReturn->name();
    else
        qDebug() << __PRETTY_FUNCTION__ << "pokemonToReturn is nullptr";

    return pokemonToReturn;
}

CardPokemon* PacketPokemon::pokemonFighter()
{
    return pokemon(0);
}

AbstractPacket::TypeOfPacket PacketPokemon::type()
{
    return AbstractPacket::Packet_Pokemon;
}

int PacketPokemon::countCard() const
{
    return m_listCards.count();
}

bool PacketPokemon::addNewCard(AbstractCard* newCard)
{
    qDebug() << __PRETTY_FUNCTION__;
    if(newCard != nullptr)
    {
        qDebug() << __PRETTY_FUNCTION__ << newCard->name();
        m_listCards.append(newCard);

        emit dataChanged(index(countCard()-1, 0), index(countCard()-1, 0));
        emit countChanged();
        emit pokemonFighterChanged();

        return true;
    }

    return false;
}

AbstractCard* PacketPokemon::takeACard(int indexCard)
{
    qDebug() << __PRETTY_FUNCTION__ << indexCard;
    AbstractCard *cardToReturn = nullptr;

    if((indexCard >= 0) && (indexCard < countCard()))
    {
        cardToReturn = m_listCards.takeAt(indexCard);

        emit dataChanged(index(indexCard, 0), index(countCard(), 0));
        emit countChanged();
    }

    return cardToReturn;
}

bool PacketPokemon::remove(AbstractCard* card)
{
    qDebug() << __PRETTY_FUNCTION__;
    bool success = false;

    if(card != nullptr)
    {
        qDebug() << __PRETTY_FUNCTION__ << card->name();
        if(m_listCards.contains(card))
        {
            int indexCard = m_listCards.indexOf(card);

            m_listCards.removeOne(card);

            emit dataChanged(index(indexCard, 0), index(countCard(), 0));
            emit countChanged();
            success = true;
        }
    }

    return success;
}

bool PacketPokemon::replacePokemon(CardPokemon *oldOne, CardPokemon *newOne)
{
    bool success = false;
    int indexCard = m_listCards.indexOf(oldOne);

    qDebug() << __PRETTY_FUNCTION__ << oldOne->name() << newOne->name() << indexCard;

    if(indexCard >= 0)
    {
        m_listCards.replace(indexCard, newOne);
        emit dataChanged(index(indexCard, 0), index(indexCard, 0), { PacketPokemon::ROLE_IMAGECARD, PacketPokemon::ROLE_NAME });
        success = true;
    }

    return success;
}

QVariant PacketPokemon::data(const QModelIndex &index, int role) const
{
    qDebug() << __PRETTY_FUNCTION__ << this->name() << index << rowCount() << countCard() << role;

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
            case PacketPokemon::ROLE_CARD:
                QQmlEngine::setObjectOwnership(cardPok, QQmlEngine::CppOwnership);
                return QVariant::fromValue<CardPokemon*>(cardPok);
            case PacketPokemon::ROLE_ISCARD:            return true;
            case PacketPokemon::ROLE_NAME:              return m_listCards[iRow]->name();
            case PacketPokemon::ROLE_IMAGECARD:         return m_listCards[iRow]->image();
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
        case PacketPokemon::ROLE_CARD:          return QVariant();
        case PacketPokemon::ROLE_ISCARD:        return false;
        case PacketPokemon::ROLE_NAME:          return "";
        case PacketPokemon::ROLE_IMAGECARD:     return AbstractCard::imageByDefault();
        }
    }

    return QVariant::Invalid;
}

int PacketPokemon::rowCount(const QModelIndex&) const
{
    return m_maxCard;
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

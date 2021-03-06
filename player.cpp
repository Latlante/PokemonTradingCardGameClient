#include "player.h"

#include <QDebug>
#include <QQmlEngine>
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Packets/packetgeneric.h"
#include "src_Packets/packethiddencards.h"
#include "src_Packets/packetpokemon.h"

Player::Player(unsigned int uid, QString name, int countDeck, QObject *parent) :
	QObject(parent),
    m_uid(uid),
    m_name(name),
    m_bench(new PacketPokemon("Bench", 5)),
    m_deck(new PacketGeneric("Deck", countDeck)),
    m_fight(new PacketPokemon("Fight", 1)),
    m_hand(new PacketGeneric("Hand")),
    m_rewards(new PacketGeneric("Rewards")),
    m_trash(new PacketGeneric("Trash")),
    m_initReady(false),
    m_canPlay(true)
{
}

Player::~Player()
{
    delete m_bench;
    delete m_deck;
    delete m_fight;
    delete m_hand;
    delete m_rewards;
    delete m_trash;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void Player::declareQML()
{
    qmlRegisterUncreatableType<Player>("model", 1, 0, "Player", "Player error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
unsigned int Player::uid()
{
    return m_uid;
}

const QString Player::name()
{
    return m_name;
}

PacketPokemon* Player::bench()
{
    QQmlEngine::setObjectOwnership(m_bench, QQmlEngine::CppOwnership);
    return m_bench;
}

PacketGeneric* Player::deck()
{
    QQmlEngine::setObjectOwnership(m_deck, QQmlEngine::CppOwnership);
	return m_deck;
}

PacketPokemon* Player::fight()
{
    QQmlEngine::setObjectOwnership(m_fight, QQmlEngine::CppOwnership);
    return m_fight;
}

PacketGeneric* Player::hand()
{
    QQmlEngine::setObjectOwnership(m_hand, QQmlEngine::CppOwnership);
	return m_hand;
}

PacketGeneric* Player::rewards()
{
    QQmlEngine::setObjectOwnership(m_rewards, QQmlEngine::CppOwnership);
    return m_rewards;
}

PacketGeneric* Player::trash()
{
    QQmlEngine::setObjectOwnership(m_trash, QQmlEngine::CppOwnership);
    return m_trash;
}

bool Player::isLoser()
{
    //Conditions de victoire:
    //  -> Plus de récompense à piocher
    //  -> Plus de carte dans le deck
    //  -> Plus de pokémon sur la banc
    bool hasAWinner = false;

    hasAWinner |= rewards()->isEmpty();
    hasAWinner |= deck()->isEmpty();
    hasAWinner |= bench()->isEmpty() && fight()->isEmpty();

    return hasAWinner;
}

bool Player::initReady()
{
    return m_initReady;
}

void Player::setInitReady(bool ready)
{
    if(m_initReady != ready)
    {
        m_initReady = ready;
        emit initReadyChanged();
    }
}

bool Player::canPlay()
{
    return m_canPlay;
}

void Player::setCanPlay(bool status)
{
    if(status != m_canPlay)
    {
        m_canPlay = status;
        emit canPlayChanged();
    }
}

bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, int index)
{
    qDebug() << __PRETTY_FUNCTION__ << index;
    bool moveSuccess = false;

    if(source != nullptr)
    {
        AbstractCard* cardToMove = source->card(index);
        moveSuccess = moveCardFromPacketToAnother(source, destination, cardToMove);
    }

    return moveSuccess;
}

bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, AbstractCard *cardToMove)
{
    QString strDebug = "";
    strDebug += "source: ";
    strDebug += source != nullptr ? "source: " + source->name() : "source: nullptr";
    strDebug += destination != nullptr ? ", destination: " + destination->name() : ", destination: nullptr";
    strDebug += cardToMove != nullptr ? ", card: " + cardToMove->name() + " (" + QString::number(cardToMove->id()) + ")" : ", card: nullptr";
    qDebug() << __PRETTY_FUNCTION__ << strDebug;

    bool moveSuccess = false;

    if(source != nullptr)
    {
        //action
        moveSuccess = source->remove(cardToMove);
    }
    else
    {
        QString messageError = "Element(s) is/are nullptr:\n";
        if(source == nullptr)
            messageError += "  - err: source is nullptr";
        else
            messageError += "  - source is " + source->name();

        qDebug() << __PRETTY_FUNCTION__ << messageError;

    }

    if(destination != nullptr)
    {
        moveSuccess = destination->addNewCard(cardToMove);
    }
    else
    {
        QString messageError = "Element(s) is/are nullptr:\n";
        if(destination == nullptr)
            messageError += "  - err: destination is nullptr";
        else
            messageError += "  - destination is " + destination->name();

        qDebug() << __PRETTY_FUNCTION__ << messageError;

    }

    return moveSuccess;
}

bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, int index, AbstractCard *cardToMove)
{
    QString strDebug = "";
    strDebug += "source: ";
    strDebug += source != nullptr ? "source: " + source->name() : "source: nullptr";
    strDebug += destination != nullptr ? ", destination: " + destination->name() : ", destination: nullptr";
    strDebug += ", index: " + QString::number(index);
    strDebug += cardToMove != nullptr ? ", card: " + cardToMove->name() + " (" + QString::number(cardToMove->id()) + ")" : ", card: nullptr";
    qDebug() << __PRETTY_FUNCTION__ << strDebug;

    bool moveSuccess = false;

    if(source != nullptr)
    {
        AbstractCard* cardToDelete = source->takeACard(index);

        if (cardToDelete != nullptr)
        {
            //move energies if the card is a pokemon
            if((cardToDelete->type() == AbstractCard::TypeOfCard_Pokemon) && (cardToMove->type() == AbstractCard::TypeOfCard_Pokemon))
            {
                CardPokemon* cardPokToDelete = dynamic_cast<CardPokemon*>(cardToDelete);
                CardPokemon* cardPokToMove = dynamic_cast<CardPokemon*>(cardToMove);
                if((cardPokToDelete != nullptr) && (cardPokToMove != nullptr))
                {
                    QList<CardEnergy*> listAllEnergies = cardPokToDelete->takeAllEnergies();
                    foreach(CardEnergy* energy, listAllEnergies)
                        cardPokToMove->addEnergy(energy);
                }
            }

            delete cardToDelete;
            cardToDelete = nullptr;
        }

        moveSuccess = true;
    }
    else
    {
        QString messageError = "Element(s) is/are nullptr:\n";
        if(source == nullptr)
            messageError += "  - err: source is nullptr";
        else
            messageError += "  - source is " + source->name();

        qDebug() << __PRETTY_FUNCTION__ << messageError;

    }

    if(destination != nullptr)
    {
        destination->addNewCard(cardToMove);
    }
    else
    {
        QString messageError = "Element(s) is/are nullptr:\n";
        if(destination == nullptr)
            messageError += "  - err: destination is nullptr";
        else
            messageError += "  - destination is " + destination->name();

        qDebug() << __PRETTY_FUNCTION__ << messageError;

    }

    return moveSuccess;
}

AbstractPacket* Player::packetFromEnumPacket(ConstantesShared::EnumPacket ePacket)
{
    switch(ePacket)
    {
    case ConstantesShared::PACKET_Bench:    return bench();
    case ConstantesShared::PACKET_Deck:     return deck();
    case ConstantesShared::PACKET_Fight:    return fight();
    case ConstantesShared::PACKET_Hand:     return hand();
    case ConstantesShared::PACKET_Rewards:  return rewards();
    case ConstantesShared::PACKET_Trash:    return trash();
    default:
        break;
    }

    return nullptr;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/

#include "player.h"

#include <QDebug>
#include <QQmlEngine>
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"

Player::Player(QString name, QList<AbstractCard*> listCards, QObject *parent) :
	QObject(parent),
    m_name(name),
    m_bench(new BenchArea("Bench")),
    m_deck(new PacketDeck("Deck", listCards)),
    m_fight(new FightArea("Fight")),
    m_hand(new PacketHand("Hand")),
    m_rewards(new PacketRewards("Rewards")),
    m_trash(new PacketTrash("Trash")),
    m_initReady(false),
    m_canPlay(true),
    m_energyPlayedForThisRound(false)
{
    foreach(AbstractCard* card, listCards)
        card->setOwner(this);
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
const QString Player::name()
{
    return m_name;
}

BenchArea* Player::bench()
{
    QQmlEngine::setObjectOwnership(m_bench, QQmlEngine::CppOwnership);
    return m_bench;
}

PacketDeck* Player::deck()
{
    QQmlEngine::setObjectOwnership(m_deck, QQmlEngine::CppOwnership);
	return m_deck;
}

FightArea* Player::fight()
{
    QQmlEngine::setObjectOwnership(m_fight, QQmlEngine::CppOwnership);
    return m_fight;
}

PacketHand* Player::hand()
{
    QQmlEngine::setObjectOwnership(m_hand, QQmlEngine::CppOwnership);
	return m_hand;
}

PacketRewards* Player::rewards()
{
    QQmlEngine::setObjectOwnership(m_rewards, QQmlEngine::CppOwnership);
    return m_rewards;
}

PacketTrash* Player::trash()
{
    QQmlEngine::setObjectOwnership(m_trash, QQmlEngine::CppOwnership);
    return m_trash;
}

void Player::newTurn()
{
    setCanPlay(true);
    m_energyPlayedForThisRound = false;
    //fight()->pokemonFighter()->setStatus(CardPokemon::Status_Normal);
}

void Player::turnFinished()
{
    //On remet à 0 les historiques de dégats pour chaque pokémon
    for(int i=0;i<fight()->countCard();++i)
    {
        fight()->pokemonFighting(i)->resetLastDamageReceived();
    }

    for(int i=0;i<bench()->countCard();++i)
    {
        bench()->cardPok(i)->resetLastDamageReceived();
    }

    //On bloque l'interface
    setCanPlay(false);
}

bool Player::isPlaying()
{
    return m_canPlay;
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

void Player::checkIfInitReady()
{
    if(fight()->countCard() > 0)
    {
        setInitReady(true);
        setCanPlay(false);
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


/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/

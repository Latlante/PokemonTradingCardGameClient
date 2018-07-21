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

void Player::drawOneCard()
{
    //qDebug() << m_name << " - "<< __PRETTY_FUNCTION__;

    moveCardFromDeckToHand();
}

void Player::drawOneReward(AbstractCard* cardReward)
{
    moveCardFromRewardToHand(cardReward);
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

bool Player::moveCardFromDeckToHand(AbstractCard *cardDeckToMove)
{
    if(cardDeckToMove == nullptr)
        return moveCardFromPacketToAnother(deck(), hand(), deck()->card(0));

    return moveCardFromPacketToAnother(deck(), hand(), cardDeckToMove);
}

bool Player::moveCardFromDeckToReward(AbstractCard* cardDeckToMove)
{
    if(cardDeckToMove == nullptr)
        return moveCardFromPacketToAnother(deck(), rewards(), deck()->card(0));

    return moveCardFromPacketToAnother(deck(), rewards(), cardDeckToMove);
}

bool Player::moveCardFromHandToBench(int indexHand, int indexBench)
{
    return false;
}

bool Player::moveCardFromHandToDeck(AbstractCard *cardHandToMove)
{
    return moveCardFromPacketToAnother(hand(), deck(), cardHandToMove);
}

bool Player::moveCardFromHandToFight(int indexHand)
{
    return false;
}

bool Player::moveCardFromHandToTrash(AbstractCard* cardHandToMove)
{
    return moveCardFromPacketToAnother(hand(), trash(), cardHandToMove);
}

bool Player::moveCardFromBenchToFight(CardPokemon* pokemonToMove)
{
    bool moveSuccess = false;

    if (pokemonToMove != nullptr)
    {
        //On refuse les évolutions
        if (pokemonToMove->isBase() == true)
        {
            moveSuccess = moveCardFromPacketToAnother(bench(), fight(), pokemonToMove);
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << ", cardPok n'est pas une base";
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << ", pokemonToMove is nullptr";
    }

    return moveSuccess;
}

bool Player::moveCardFromBenchToTrash(int index)
{
    CardPokemon* pokemonBench = bench()->cardPok(index);
    QList<AbstractCard*> listCardAttached = pokemonBench->purge();

    foreach(AbstractCard* card, listCardAttached)
        trash()->addNewCard(card);

    return moveCardFromPacketToAnother(bench(), trash(), index);
}

bool Player::moveCardFromFightToTrash(int index)
{
    CardPokemon* pokemonFighter = fight()->pokemonFighting(index);
    QList<AbstractCard*> listCardAttached = pokemonFighter->purge();

    foreach(AbstractCard* card, listCardAttached)
        trash()->addNewCard(card);

    return moveCardFromPacketToAnother(fight(), trash(), index);
}

bool Player::moveCardFromRewardToHand(AbstractCard* cardReward)
{
    return moveCardFromPacketToAnother(rewards(), hand(), cardReward);
}

bool Player::moveCardFromTrashToHand(AbstractCard *cardTrash)
{
    return moveCardFromPacketToAnother(trash(), hand(), cardTrash);
}

bool Player::moveAllCardFromHandToDeck()
{
    bool status = true;

    while((hand()->countCard() > 0) && (status == true))
    {
        status &= moveCardFromHandToDeck(0);
    }

    return status;
}

bool Player::swapCardsBetweenBenchAndFight(CardPokemon* pokemonBenchToSwap)
{
    bool status = true;

    if((fight() != nullptr) && (bench() != nullptr))
    {
        if((fight()->pokemonFighting(0) != nullptr) && (pokemonBenchToSwap != nullptr))
        {
            //A ce moment là, on sait que tout est bon
            //On commence donc par récupérer le pokémon qui se bat pour le remplace
            AbstractCard* pokFigther = fight()->takeACard(0);
            status &= moveCardFromBenchToFight(pokemonBenchToSwap);
            status &= bench()->addNewCard(pokFigther);
        }
        else
            status = false;
    }
    else
        status = false;

    return status;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, int index)
{
    bool moveSuccess = false;

    if (destination->isFull() == false)
    {
        AbstractCard* cardToMove = source->takeACard(index);

        if (cardToMove != nullptr)
        {
            destination->addNewCard(cardToMove);
            moveSuccess = true;
        }
        else
        {
            qCritical() << __PRETTY_FUNCTION__ << "Card is nullptr";

        }
    }

    return moveSuccess;
}

bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, AbstractCard *cardToMove)
{
    bool moveSuccess = false;

    if((source != nullptr) && (destination != nullptr) && (cardToMove != nullptr))
    {
        if (destination->isFull() == false)
        {
            moveSuccess = source->removeFromPacketWithoutDelete(cardToMove);

            if(moveSuccess == true)
                moveSuccess = destination->addNewCard(cardToMove);
        }
    }
    else
    {
        QString messageError = "Element(s) is/are nullptr:\n";
        if(source == nullptr)
            messageError += "  - err: source is nullptr";
        else
            messageError += "  - source is " + source->name();

        if(destination == nullptr)
            messageError += "  - err: destination is nullptr";
        else
            messageError += "  - destination is " + destination->name();

        if(cardToMove == nullptr)
            messageError += "  - err: cardToMove is nullptr";
        else
            messageError += "  - source is " + cardToMove->name();

        qCritical() << __PRETTY_FUNCTION__ << messageError;

    }

    return moveSuccess;
}

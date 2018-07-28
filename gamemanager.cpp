#include "gamemanager.h"

#include <QDebug>
#include <QEventLoop>
#include "src_Controler/ctrlpopups.h"
#include "src_Models/modellistenergies.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"
#include "common/utils.h"

const int GameManager::m_NUMBER_FIRST_CARDS = 7;
const int GameManager::m_NUMBER_REWARDS = 6;
GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager(QObject *parent) :
    QObject(parent),
#ifdef TESTS_UNITAIRES
    m_forcedValueHeadOrTail(false),
    m_nextValueHeadOrTail(0),
#endif
	m_listPlayers(QList<Player*>()),
    m_indexCurrentPlayer(-1),
    m_playerAttacking(nullptr),
    m_playerAttacked(nullptr),
    m_gameStatus(ConstantesQML::StepPreparation),
    m_gameIsReady(false),
    m_elementFromDisplays(QVariant())
{
	
}

GameManager::~GameManager()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
GameManager* GameManager::createInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new GameManager();
    }

    return m_instance;
}

void GameManager::deleteInstance()
{
    if(m_instance != NULL)
    {
        delete m_instance;
        m_instance = NULL;
    }
}

GameManager* GameManager::instance()
{
    return m_instance;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
//ACCESSEURS
Player* GameManager::currentPlayer()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    return m_playerAttacking;
}

Player* GameManager::playerAttacked()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    return m_playerAttacked;
}

Player* GameManager::playerAt(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    Player* play = NULL;

    if((index >= 0) && (index < m_listPlayers.count()))
    {
        play = m_listPlayers[index];
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "Erreur avec le Player" << index << "/" << m_listPlayers.count();
    }

    return play;
}


//PREPARATION DE LA PARTIE
Player *GameManager::addNewPlayer(QString name, QList<AbstractCard*> listCards)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
	Player* newPlayer = new Player(name, listCards);

    connect(newPlayer, &Player::endOfTurn, this, &GameManager::onEndOfTurn_Player);

	m_listPlayers.append(newPlayer);

    return newPlayer;
}

void GameManager::selectFirstPlayer()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
#ifdef TESTS_UNITAIRES
    setIndexCurrentPlayer(0);
#else
    int indexCurrentPlayer = Utils::selectFirstPlayer(m_listPlayers.count());
    setIndexCurrentPlayer(indexCurrentPlayer);
#endif
}

void GameManager::setInitReady()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    bool everyoneIsReady = true;

}

//PHASE DE COMBAT
void GameManager::nextPlayer()
{	
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    setIndexCurrentPlayer(m_indexCurrentPlayer+1);
}

CardPokemon::Enum_StatusOfAttack GameManager::attack(CardPokemon *pokemonAttacking, unsigned short index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    CardPokemon::Enum_StatusOfAttack statusOfAttack = CardPokemon::Attack_UnknownError;

    return statusOfAttack;
}

bool GameManager::retreat(CardPokemon *pokemonToRetreat)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    bool success = false;

    return success;
}

void GameManager::endOfTurn()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif


}

//FIN DE LA GAME
Player* GameManager::gameIsFinished()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //Conditions de victoire:
    //  -> Plus de récompense à piocher
    //  -> Plus de carte dans le deck
    //  -> Plus de pokémon sur la banc
    Player* playWinner = nullptr;
	
#ifndef TESTS_UNITAIRES
	foreach(Player* play, m_listPlayers)
	{
        if(play->isLoser())
            playWinner = enemyOf(play);
	}
#endif
	
    return playWinner;
}

QList<AbstractCard *> GameManager::displayPacket(AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

#ifdef TESTS_UNITAIRES
    Q_UNUSED(quantity)
    Q_UNUSED(typeOfCard)

    return {packet->card(0)};
#else
    emit displayPacketAsked(packet, quantity, typeOfCard);

    QEventLoop loop;
    connect(this, &GameManager::selectionDisplayFinished, &loop, &QEventLoop::quit);
    loop.exec();

    return m_elementFromDisplays.value<QList<AbstractCard*> >();
#endif
}

AbstractCard::Enum_element GameManager::displayAllElements(unsigned short quantity)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

#ifdef TESTS_UNITAIRES
    Q_UNUSED(quantity)

    return AbstractCard::Element_Electric;
#else
    emit displayAllElementsAsked(quantity);

    AbstractCard::Enum_element elementToReturn = AbstractCard::Element_None;

    QList<AbstractCard *> listSelectionCards = m_elementFromDisplays.value<QList<AbstractCard *> >();
    if(listSelectionCards.count() > 0)
    {
        AbstractCard *abCard = listSelectionCards.first();

        if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            elementToReturn = energy->element();
        }
    }

    return elementToReturn;
#endif
}

QList<AbstractCard *> GameManager::displaySelectHiddenCard(PacketRewards *packet, unsigned short quantity)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

#ifdef TESTS_UNITAIRES
    Q_UNUSED(quantity)

    return {rewards->card(0)};
#else
    emit displaySelectHiddenCardAsked(packet, quantity);

    return m_elementFromDisplays.value<QList<AbstractCard*> >();
#endif

}

QList<CardEnergy*> GameManager::displayEnergiesForAPokemon(CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

#ifdef TESTS_UNITAIRES
    Q_UNUSED(quantity)
    Q_UNUSED(element)
    return {pokemon->modelListOfEnergies()->energy(0)};
#else
    emit displayEnergiesForAPokemonAsked(pokemon, quantity, element);

    QList<CardEnergy*> listEnergies;
    QList<AbstractCard *> listSelectionCards = m_elementFromDisplays.value<QList<AbstractCard *> >();

    foreach(AbstractCard* abCard, listSelectionCards)
    {
        if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            listEnergies.append(static_cast<CardEnergy*>(abCard));
        }
    }

    return listEnergies;
#endif
}

int GameManager::displayAttacks(CardPokemon* card, bool blockRetreat)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

#ifdef TESTS_UNITAIRES
    Q_UNUSED(card)
    Q_UNUSED(blockRetreat)

    return 0;
#else
    bool authorizeRetreat = false;

    if(blockRetreat == false)
        authorizeRetreat = card->canRetreat();

    emit displayAttacksAsked(card, authorizeRetreat);

    qDebug() << __PRETTY_FUNCTION__ << m_elementFromDisplays.toInt();

    return m_elementFromDisplays.toInt();
#endif
}

void GameManager::displayMessage(QString message)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

#ifdef TESTS_UNITAIRES
    Q_UNUSED(message)
#else
    emit displayMessageAsked(message);
#endif
}

void GameManager::endOfSelectionDisplay(QVariant element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__ << element;
#endif

    m_elementFromDisplays = element;
    emit selectionDisplayFinished();
}

#ifdef TESTS_UNITAIRES
void GameManager::setForcedValueHeadOrTail(bool forced, unsigned short value)
{
    m_forcedValueHeadOrTail = forced;
    m_nextValueHeadOrTail = value;
}

unsigned short GameManager::headOrTail()
{
    if(m_forcedValueHeadOrTail)
        return m_nextValueHeadOrTail;

    return Utils::headOrTail();
}
#else
unsigned short GameManager::headOrTail()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    unsigned short coin = Utils::headOrTail();
    qDebug() << __PRETTY_FUNCTION__ << "coin=" << coin;

    emit headOrTailAsked();

    QEventLoop loop;
    connect(this, &GameManager::selectionDisplayFinished, &loop, &QEventLoop::quit);
    loop.exec();

    return m_elementFromDisplays.toInt();
}
#endif

/************************************************************
*****			  FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void GameManager::onEndOfTurn_Player()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //On bloque tous les joueurs
    foreach(Player* play, m_listPlayers)
        play->turnFinished();

    //On vérifie si quelqu'un a gagné
    bool hasAWinner = false;

    foreach(Player* play, m_listPlayers)
        hasAWinner |= play->isLoser();

    //S'il n'y a pas encore de vainqueur, on laisse le prochain joueur jouer
    if(hasAWinner == false)
    {
        nextPlayer();
    }
    else
    {
        foreach(Player* play, m_listPlayers)
        {
            if(play->isLoser())
            {
                qDebug() << "VICTOIRE DE " << play->name();
            }
        }

    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void GameManager::setIndexCurrentPlayer(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //Sécurité pour ne pas dépasser l'index
    index = index % m_listPlayers.count();

    //qDebug() << __PRETTY_FUNCTION__ << m_indexCurrentPlayer << index;

    if(m_indexCurrentPlayer != index)
    {
        m_indexCurrentPlayer = index;

        m_playerAttacking = m_listPlayers[index];
        m_playerAttacked = enemyOf(m_playerAttacking);

        emit indexCurrentPlayerChanged();
    }
}

Player* GameManager::enemyOf(Player *play)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    Player* playerEnnemy = nullptr;

    //A REVOIR
    if(play == m_listPlayers[0])
    {
        playerEnnemy = m_listPlayers[1];
    }
    else if(play == m_listPlayers[1])
    {
        playerEnnemy = m_listPlayers[0];
    }

    return playerEnnemy;
}



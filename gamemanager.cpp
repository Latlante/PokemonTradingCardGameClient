#include "gamemanager.h"

#include <QDebug>
#include <QEventLoop>
#include "src_Cards/cardempty.h"
#include "src_Controler/ctrlpopups.h"
#include "src_Models/modellistenergies.h"
#include "common/utils.h"

GameManager::GameManager(QObject *parent) :
    QObject(parent),
    m_uidGame(0),
    m_playerYou(nullptr),
    m_playerOpponent(nullptr),
    m_gameStatus(ConstantesQML::StepPreparation)
{
	
}

GameManager::~GameManager()
{
	
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool GameManager::isReady()
{
    return ((playerYou() != nullptr) &&
            (playerOpponent() != nullptr) &&
            (uidGame() != 0));
}

//ACCESSEURS
unsigned GameManager::uidGame()
{
    return m_uidGame;
}

void GameManager::setUidGame(unsigned int uid)
{
    m_uidGame = uid;
}

Player* GameManager::playerYou()
{
    return m_playerYou;
}

void GameManager::setPlayerYou(unsigned int uid, const QString &name)
{
    if(m_playerYou != nullptr)
        delete m_playerYou;

    m_playerYou = new Player(uid, name, MAXCARDS_DECK);
}

Player* GameManager::playerOpponent()
{
    return m_playerOpponent;
}

void GameManager::setPlayerOpponent(unsigned int uid, const QString &name)
{
    if(m_playerOpponent != nullptr)
        delete m_playerOpponent;

    m_playerOpponent = new Player(uid, name, MAXCARDS_DECK);
    m_playerOpponent->setCanPlay(false);
}

Player* GameManager::playerByName(const QString &name)
{
    Player* playToReturn = nullptr;

    Player* playYou = playerYou();
    if(playYou != nullptr)
    {
        if(playYou->name() == name)
            playToReturn = playYou;
    }

    if(playToReturn == nullptr)
    {
        Player* playOpponent = playerOpponent();
        if(playOpponent != nullptr)
        {
            if(playOpponent->name() == name)
                playToReturn = playOpponent;
        }
    }

    return playToReturn;
}

/************************************************************
*****			  FONCTIONS SLOTS PRIVEES				*****
************************************************************/


/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/


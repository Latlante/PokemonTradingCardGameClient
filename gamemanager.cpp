#include "gamemanager.h"

#include <QDebug>
#include <QEventLoop>
#include "src_Cards/cardempty.h"
#include "src_Controler/ctrlpopups.h"
#include "src_Models/modellistenergies.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"
#include "common/utils.h"

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
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
//ACCESSEURS
Player* GameManager::playerYou()
{
    return m_playerYou;
}

void GameManager::setPlayerYou(unsigned int uid, const QString &name)
{
    QList<AbstractCard*> listCardEmpty;
    for(int i=0;i<MAXCARDS_DECK;++i)
        listCardEmpty.append(new CardEmpty());

    if(m_playerYou != nullptr)
        delete m_playerYou;

    m_playerYou = new Player(uid, name, listCardEmpty);
}

Player* GameManager::playerOpponent()
{
    return m_playerOpponent;
}

void GameManager::setPlayerOpponent(unsigned int uid, const QString &name)
{
    QList<AbstractCard*> listCardEmpty;
    for(int i=0;i<MAXCARDS_DECK;++i)
        listCardEmpty.append(new CardEmpty());

    if(m_playerOpponent != nullptr)
        delete m_playerOpponent;

    m_playerOpponent = new Player(uid, name, listCardEmpty);
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


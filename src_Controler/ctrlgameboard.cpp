#include "ctrlgameboard.h"
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>
#include <QtQml/qqml.h>

#include "player.h"
#include "src_Cards/cardpokemon.h"
#include "src_Communication/socketclient.h"
#include "src_Controler/ctrlanimation.h"
#include "src_Controler/ctrlpopups.h"
#include "src_Controler/ctrlselectingcards.h"
#include "src_Models/factorymainpageloader.h"
#include "src_Models/modellistallplayers.h"
#include "src_Models/modellistofgamesavailable.h"
#include "src_Models/modelpopupselectcardinpacket.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packetrewards.h"

CtrlGameBoard::CtrlGameBoard(CtrlSelectingCards &ctrlSelectCards, CtrlPopups &ctrlPopups, CtrlAnimation &ctrlAnim, QObject *parent) :
    QObject(parent),
    m_socket(new SocketClient()),
    m_gameManager(GameManager::createInstance()),
    m_listAllPlayers(new ModelListAllPlayers()),
    m_listOfGamesAvailable(new ModelListOfGamesAvailable()),
    m_factoryMainPageLoader(new FactoryMainPageLoader()),
    m_ctrlAnim(ctrlAnim),
    m_ctrlPopups(ctrlPopups),
    m_ctrlSelectingCards(ctrlSelectCards),
    m_stepInProgress(false)
{
    //initGame();
    connect(&m_ctrlSelectingCards, &CtrlSelectingCards::listsComplete, this, &CtrlGameBoard::onListsComplete_CtrlSelectingCards);
    connect(m_gameManager, &GameManager::indexCurrentPlayerChanged, this, &CtrlGameBoard::currentPlayerChanged);
    connect(m_gameManager, &GameManager::gameStatusChanged, this, &CtrlGameBoard::gameStatusChanged);

    connect(m_gameManager, &GameManager::displayAllElementsAsked, this ,&CtrlGameBoard::onDisplayAllElementsAsked);
    connect(m_gameManager, &GameManager::displayEnergiesForAPokemonAsked, this, &CtrlGameBoard::onDisplayEnergiesForAPokemonAsked);
    connect(m_gameManager, &GameManager::displayMessageAsked, this, &CtrlGameBoard::onDisplayMessageAsked);
    connect(m_gameManager, &GameManager::displayPacketAsked, this, &CtrlGameBoard::onDisplayPacketAsked);
    connect(m_gameManager, &GameManager::displaySelectHiddenCardAsked, this, &CtrlGameBoard::onDisplaySelectHiddenCardAsked);
    connect(m_gameManager, &GameManager::displayAttacksAsked, this, &CtrlGameBoard::onDisplayAttacksAsked);
    connect(m_gameManager, &GameManager::headOrTailAsked, this, &CtrlGameBoard::onHeadOrTailAsked);
    connect(m_gameManager, &GameManager::movingCardAnimationStartAsked, this, &CtrlGameBoard::onMovingCardAnimationStart);
}

CtrlGameBoard::~CtrlGameBoard()
{
    delete m_socket;
    delete m_listAllPlayers;
    delete m_listOfGamesAvailable;
    delete m_factoryMainPageLoader;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CtrlGameBoard::declareQML()
{
    qmlRegisterUncreatableType<CtrlGameBoard>("model", 1, 0, "CtrlGameBoard", "CtrlGameBoard error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool CtrlGameBoard::install(QQmlApplicationEngine *pEngine)
{
    qDebug() << "installing controleur...";

    bool bInstalled = false;

    if (NULL != pEngine)
    {
        QQmlContext* pContext = pEngine->rootContext();
        if (NULL != pContext)
        {
            bInstalled = true;
            qDebug() << "CtrlGameBoard is installed in QML engine";
            pContext->setContextProperty("ctrlGameBoard", this);
        }
        else
        {
            qCritical() << "Qml context is null.";
        }
    }

    return bInstalled;
}

Player* CtrlGameBoard::currentPlayer()
{
    return m_gameManager->currentPlayer();
}

ConstantesQML::StepGame CtrlGameBoard::gameStatus()
{
    return m_gameManager->gameStatus();
}

ModelListAllPlayers* CtrlGameBoard::modelAllPlayers()
{
    return m_listAllPlayers;
}

ModelListOfGamesAvailable* CtrlGameBoard::modelAllOfGamesAvailable()
{
    return m_listOfGamesAvailable;
}

FactoryMainPageLoader* CtrlGameBoard::factory()
{
    return m_factoryMainPageLoader;
}

void CtrlGameBoard::initGame()
{
    m_gameManager->initGame();
}

Player* CtrlGameBoard::playerAt(int index)
{
    return m_gameManager->playerAt(index);
}

bool CtrlGameBoard::stepInProgress()
{
    return m_stepInProgress;
}

void CtrlGameBoard::setStepInProgress(bool inProgress)
{
    if(m_stepInProgress != inProgress)
    {
        m_stepInProgress = inProgress;
        emit stepInProgressChanged();
    }
}

void CtrlGameBoard::authentificate(const QString &name, const QString &password)
{
    qDebug() << __PRETTY_FUNCTION__;
    setStepInProgress(true);

    if(m_socket->tryToConnect())
    {
        qDebug() << __PRETTY_FUNCTION__ << "Connection success";

        QJsonDocument jsonResponse;
        if(m_socket->authentificate(name, password, jsonResponse))
        {
            qDebug() << __PRETTY_FUNCTION__ << "Authentification success";

            QJsonObject obj = jsonResponse.object();
            m_socket->setToken(obj["token"].toString());

            QJsonArray arrayGames = obj["games"].toArray();
            for(int i=0;i<arrayGames.count();++i)
            {
                QJsonObject objGame = arrayGames[i].toObject();
                m_listOfGamesAvailable->addNewGame(objGame["uid"].toInt(),
                                                   objGame["name"].toString(),
                                                   //objGame["opponent"].toString());
                                                   "opponent");
            }

            setStepInProgress(false);
            m_factoryMainPageLoader->displayCreateChooseGame();
        }
        else
        {
            setStepInProgress(false);
            qDebug() << __PRETTY_FUNCTION__ << "Error during the authentification";
        }
    }
    else
    {
        setStepInProgress(false);
        qDebug() << __PRETTY_FUNCTION__ << "Error during the connection";
    }
}

void CtrlGameBoard::listOfAllPlayers()
{
    qDebug() << __PRETTY_FUNCTION__;
    setStepInProgress(true);

    //Get list of all players
    QJsonDocument jsonResponse;

    if(m_socket->listAllPlayers(jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        QJsonArray arrayPlayers = obj["players"].toArray();
        for(int i=0;i<arrayPlayers.count();++i)
        {
            //MODIF A VENIR
            //QJsonObject objPlayer = arrayPlayers[i].toObject();
            //m_listAllPlayers->addNewPlayer(objPlayer["id"].toInt(), objPlayer["name"].toString());

            m_listAllPlayers->addNewPlayer(i, arrayPlayers[i].toString());
        }

        if(m_listAllPlayers->rowCount() > 0)
        {
            setStepInProgress(false);
            m_factoryMainPageLoader->displayAllPlayers();
        }
    }
}

void CtrlGameBoard::createANewGame(const QString &opponent)
{
    qDebug() << __PRETTY_FUNCTION__ << opponent;
    if(m_socket->createANewGame())
    {
        m_factoryMainPageLoader->displaySelectCards();
    }
}

void CtrlGameBoard::listOfGamesAvailable()
{

}

void CtrlGameBoard::joinAGame(int idGame)
{

}

void CtrlGameBoard::returnToTheMenu()
{
    m_factoryMainPageLoader->displayCreateChooseGame();
}

void CtrlGameBoard::onClicked_ButtonOk_SelectPlayers(QStringList listOfPlayers)
{
    m_factoryMainPageLoader->displaySelectCards();
    m_ctrlSelectingCards.selectCards(listOfPlayers);

    /*for(int i=0;i<listOfPlayers.count();++i)
    {
        m_ctrlSelectingCards.newSelection(listOfPlayers[i], i == (listOfPlayers.count()-1));

        QEventLoop loop;
        connect(this, &CtrlGameBoard::nextPlayer, &loop, &QEventLoop::quit);
        loop.exec();
    }*/
}

void CtrlGameBoard::displaySelectingCardsForNextPlayers()
{
    //emit nextPlayer();
}

void CtrlGameBoard::onClicked_ButtonOk_SelectCards()
{
    //initGame();

    m_factoryMainPageLoader->displayBoard();
}

void CtrlGameBoard::onClicked_ButtonReadyPreparation()
{
    m_gameManager->setInitReady();
}

void CtrlGameBoard::actionAttack(CardPokemon *card)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif
    //Choix de l'attaque
    //Player* playerAttacking = card->owner();
    //Player* playerAttacked = m_gameManager->ennemyOf(playerAttacking);
    int indexAttack = m_gameManager->displayAttacks(card);

    if(indexAttack < 4)         //Correspond à une attaque
    {
        //Le pokémon attaquant attaque
        CardPokemon::Enum_StatusOfAttack statusOfAttack = m_gameManager->attack(card, indexAttack);

        switch(statusOfAttack)
        {
        case CardPokemon::Attack_OK:
            m_gameManager->endOfTurn();
            break;
        case CardPokemon::Attack_AttackBlocked:
            m_gameManager->displayMessage("Impossible d'utiliser cette attaque pendant ce tour");
            break;
        case CardPokemon::Attack_NotEnoughEnergies:
            m_gameManager->displayMessage("Vous n'avez pas assez d'énergies");
            break;
        case CardPokemon::Attack_WrongStatus:
            m_gameManager->displayMessage("Vous ne pouvez pas attaquer pendant que vous êtes " + card->statusFormatString());
            break;
        case CardPokemon::Attack_IndexNOK:
            qCritical() << __PRETTY_FUNCTION__ << "Erreur d'index";
            break;
        case CardPokemon::Attack_UnknownError:
            qCritical() << __PRETTY_FUNCTION__ << "Erreur inconnue";
            break;
        }

    }
    else if(indexAttack == 4)   //Correspond à la retraite
    {
        bool success = m_gameManager->retreat(card);

        if(success == false)
            qCritical() << __PRETTY_FUNCTION__ << "Erreur lors de l'échange";
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "erreur de indexAttack=" << indexAttack;
    }
}

void CtrlGameBoard::actionFinishYourTurn()
{
    m_gameManager->endOfTurn();
}

void CtrlGameBoard::testAnimation()
{
    onMovingCardAnimationStart();
}

/************************************************************
*****             FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void CtrlGameBoard::onListsComplete_CtrlSelectingCards()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QMap<QString,QList<AbstractCard*> > listCardsByPlayer = m_ctrlSelectingCards.listCardsByPlayer();

    for(int i=0;i<listCardsByPlayer.keys().count();++i)
    {
        QString name = listCardsByPlayer.keys()[i];
        m_gameManager->addNewPlayer(name, listCardsByPlayer[name]);
    }

    m_gameManager->initGame();
    m_factoryMainPageLoader->displayBoard();
    m_gameManager->displayMessage("Préparez votre jeu");

}

void CtrlGameBoard::onDisplayPacketAsked(AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QList<AbstractCard *> list = m_ctrlPopups.displayPacket(packet, quantity, typeOfCard);
    m_gameManager->endOfSelectionDisplay(QVariant::fromValue(list));
}

void CtrlGameBoard::onDisplayAllElementsAsked(unsigned short quantity)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QList<AbstractCard *> list = m_ctrlPopups.displayAllElements(quantity);
    m_gameManager->endOfSelectionDisplay(QVariant::fromValue(list));
}

void CtrlGameBoard::onDisplaySelectHiddenCardAsked(PacketRewards *packet, unsigned short quantity)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QList<AbstractCard *> list = m_ctrlPopups.displaySelectHiddenCard(packet, quantity);
    m_gameManager->endOfSelectionDisplay(QVariant::fromValue(list));
}

void CtrlGameBoard::onDisplayEnergiesForAPokemonAsked(CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QList<AbstractCard *> list = m_ctrlPopups.displayEnergiesForAPokemon(pokemon, quantity, element);
    m_gameManager->endOfSelectionDisplay(QVariant::fromValue(list));
}

void CtrlGameBoard::onDisplayAttacksAsked(CardPokemon *card, bool authorizeRetreat)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    int attackIndex = m_ctrlPopups.displayAttacks(card, authorizeRetreat);
    m_gameManager->endOfSelectionDisplay(QVariant::fromValue(attackIndex));
}

void CtrlGameBoard::onDisplayMessageAsked(QString message)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_ctrlPopups.displayMessage(message);
    m_gameManager->endOfSelectionDisplay();
}

void CtrlGameBoard::onHeadOrTailAsked()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    unsigned short coin = Utils::headOrTail();
    m_ctrlPopups.displayHeadOrTail(coin);
    m_gameManager->endOfSelectionDisplay(coin);
}

void CtrlGameBoard::onMovingCardAnimationStart()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_ctrlAnim.startAnimationMovingCard(CtrlAnimation::Location_Deck, CtrlAnimation::Location_Hand);
}

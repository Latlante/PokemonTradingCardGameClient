#include "ctrlgameboard.h"
#include <QDir>
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
#include "common/database.h"
#include "src_Cards/cardpokemon.h"
#include "src_Communication/socketclient.h"
#include "src_Controler/ctrlanimation.h"
#include "src_Controler/ctrlpopups.h"
#include "src_Controler/ctrlselectingcards.h"
#include "src_Models/factorymainpageloader.h"
#include "src_Models/modellistallplayers.h"
#include "src_Models/modellistofgamesavailable.h"
#include "src_Models/modellistselection.h"
#include "src_Models/modelpopupselectcardinpacket.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packetrewards.h"

CtrlGameBoard::CtrlGameBoard(CtrlSelectingCards &ctrlSelectCards, CtrlPopups &ctrlPopups, CtrlAnimation &ctrlAnim, QObject *parent) :
    QObject(parent),
    m_socket(new SocketClient()),
    m_gameManager(new GameManager()),
    m_listAllPlayers(new ModelListAllPlayers()),
    m_listOfGamesAvailable(new ModelListOfGamesAvailable()),
    m_factoryMainPageLoader(new FactoryMainPageLoader()),
    m_ctrlAnim(ctrlAnim),
    m_ctrlPopups(ctrlPopups),
    m_ctrlSelectingCards(ctrlSelectCards),
    m_stepInProgress(false),
    m_idGame(0),
    m_gameStatus(ConstantesQML::StepPreparation)
{

}

CtrlGameBoard::~CtrlGameBoard()
{
    delete m_socket;
    delete m_gameManager;
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

    if (nullptr != pEngine)
    {
        QQmlContext* pContext = pEngine->rootContext();
        if (nullptr != pContext)
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

ModelListAllPlayers* CtrlGameBoard::modelAllPlayers()
{
    return m_listAllPlayers;
}

ModelListOfGamesAvailable* CtrlGameBoard::modelAllOfGamesAvailable()
{
    return m_listOfGamesAvailable;
}

ModelListSelection* CtrlGameBoard::modelListSelect()
{
    QStringList listFilesSaved = QDir("save").entryList();
    listFilesSaved.removeOne(".");
    listFilesSaved.removeOne("..");

    ModelListSelection* model = new ModelListSelection();
    foreach(QString file, listFilesSaved)
        model->addElement(file);

    model->setNameSelection("Paquet(s) sauvegardé(s)");

    return model;
}

FactoryMainPageLoader* CtrlGameBoard::factory()
{
    return m_factoryMainPageLoader;
}

Player* CtrlGameBoard::playerYou()
{
    return m_gameManager->playerYou();
}

Player* CtrlGameBoard::playerOpponent()
{
    return m_gameManager->playerOpponent();
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

ConstantesQML::StepGame CtrlGameBoard::gameStatus()
{
    return m_gameStatus;
}

void CtrlGameBoard::setGameStatus(ConstantesQML::StepGame status)
{
    if(status != m_gameStatus)
    {
        m_gameStatus = status;
        emit gameStatusChanged();
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
                                                   objGame["opponent"].toString());
            }

            setStepInProgress(false);
            m_gameManager->setPlayerYou(0, name);
            m_ctrlSelectingCards.setName(name);
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

    /*m_ctrlSelectingCards.setName(name);
    m_factoryMainPageLoader->displaySelectCards();*/
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
        m_listAllPlayers->clear();
        for(int i=0;i<arrayPlayers.count();++i)
        {
            //MODIF A VENIR
            QJsonObject objPlayer = arrayPlayers[i].toObject();
            m_listAllPlayers->addNewPlayer(objPlayer["uid"].toInt(), objPlayer["name"].toString());

            //m_listAllPlayers->addNewPlayer(i, arrayPlayers[i].toString());
        }

        if(m_listAllPlayers->rowCount() > 0)
        {
            setStepInProgress(false);
            m_factoryMainPageLoader->displayAllPlayers();
        }
    }
}

void CtrlGameBoard::listOfGamesAlreadyExisting()
{
    qDebug() << __PRETTY_FUNCTION__;
    setStepInProgress(true);

    QJsonDocument jsonResponse;

    if(m_socket->listAllGamesAlreadyExisting(jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj.contains("games"))
        {
            QJsonArray arrayGames = obj["games"].toArray();
            for(int i=0;i<arrayGames.count();++i)
            {
                QJsonObject objGame = arrayGames[i].toObject();
                m_listOfGamesAvailable->addNewGame(objGame["uid"].toInt(),
                                                   objGame["name"].toString(),
                                                   objGame["opponent"].toString());
            }

            setStepInProgress(false);
            m_factoryMainPageLoader->displaySelectCards();
        }
    }
}

void CtrlGameBoard::createANewGame(const QString& nameGame, int idOpponent)
{
    qDebug() << __PRETTY_FUNCTION__ << nameGame << idOpponent;
    setStepInProgress(true);

    QJsonDocument jsonResponse;

    if(m_socket->createANewGame(nameGame, idOpponent, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj["idGame"].toInt() != 0)
        {
            int idGame = obj["idGame"].toInt();
            m_listOfGamesAvailable->addNewGame(idGame, nameGame, m_listAllPlayers->namePlayerFromId(idOpponent));
            m_gameManager->setPlayerOpponent(static_cast<unsigned int>(idOpponent), m_listAllPlayers->namePlayerFromId(idOpponent));
            m_gameManager->setUidGame(static_cast<unsigned int>(idGame));
            setStepInProgress(false);
            m_factoryMainPageLoader->displaySelectCards();
        }
    }
}

void CtrlGameBoard::listOfGamesAvailable()
{
//    if(m_listOfGamesAvailable->count() > 1)
//    {
//        m_factoryMainPageLoader->displayAllGamesAvailable();
//    }
//    else if(m_listOfGamesAvailable->count() == 1)
//    {
//        joinAGame(m_listOfGamesAvailable->id(0));
//    }
    m_factoryMainPageLoader->displayAllGamesAvailable();
}

void CtrlGameBoard::joinAGame(int idGame)
{
    qDebug() << __PRETTY_FUNCTION__;
    QJsonDocument jsonResponse;
    setStepInProgress(true);

    if(m_socket->getAllInfoOnTheGame(idGame, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        m_idGame = idGame;
        QJsonObject obj = jsonResponse.object();

        if(obj["success"].toString() == "ok")
        {
            setStepInProgress(false);

            ConstantesQML::StepGame step = static_cast<ConstantesQML::StepGame>(obj["gameStatus"].toInt());
            switch(step)
            {
            case ConstantesQML::StepSelectionCards:
                m_factoryMainPageLoader->displaySelectCards();
                break;

            case ConstantesQML::StepPreparation:
            case ConstantesQML::StepGameInProgress:
                m_factoryMainPageLoader->displayBoard();
                break;

            case ConstantesQML::StepFinished:
                m_factoryMainPageLoader->displayBoard();
                m_ctrlPopups.displayMessage("Victoire de " + obj["winner"].toString());
                break;
            }
        }
        else
        {
            qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
        }
    }
}

void CtrlGameBoard::returnToTheMenu()
{
    m_factoryMainPageLoader->displayCreateChooseGame();
}

void CtrlGameBoard::sendCardsSelected()
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<InfoCard> listCards = m_ctrlSelectingCards.listCards();
    QJsonDocument jsonResponse;
    setStepInProgress(true);

    if(m_socket->sendCardsSelected(m_idGame, listCards, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj["success"].toString() == "ok")
        {
            setStepInProgress(false);
            m_factoryMainPageLoader->displayBoard();
        }
        else
        {
            qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
        }
    }
}

void CtrlGameBoard::initReady()
{
    qDebug() << __PRETTY_FUNCTION__;
    QJsonDocument jsonResponse;
    setStepInProgress(true);

    if(m_socket->initIsReady(m_idGame, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj["success"].toString() == "ok")
        {
            setStepInProgress(false);
            //m_factoryMainPageLoader->displayBoard();
        }
        else
        {
            qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
        }
    }
}

void CtrlGameBoard::moveACard(int idPacketOrigin, int idCardOrigin, int idPacketDestination, int idCardDestination)
{
    qDebug() << __PRETTY_FUNCTION__;
    QJsonDocument jsonResponse;
    setStepInProgress(true);

    if(m_socket->moveACard(m_idGame, static_cast<ConstantesShared::EnumPacket>(idPacketOrigin), idCardOrigin, static_cast<ConstantesShared::EnumPacket>(idPacketDestination), idCardDestination, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj["success"].toString() == "ok")
        {
            setStepInProgress(false);
        }
        else
        {
            qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
        }
    }
}

void CtrlGameBoard::testAnimation()
{
    onMovingCardAnimationStart();
}

void CtrlGameBoard::displayPageSelection()
{
    m_factoryMainPageLoader->displayPageSelection();
}

void CtrlGameBoard::validatePageSelection(const QString &item)
{
    m_ctrlSelectingCards.loadPacket(item);
    m_factoryMainPageLoader->displaySelectCards();
}

/************************************************************
*****             FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void CtrlGameBoard::onMovingCardAnimationStart()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_ctrlAnim.startAnimationMovingCard(CtrlAnimation::Location_Deck, CtrlAnimation::Location_Hand);
}

/************************************************************
*****				FONCTIONS PRIVATE					*****
************************************************************/
void CtrlGameBoard::executeActions(QJsonObject objActions)
{
    if((objActions.contains("indexBegin") == true) && (objActions.contains("indexEnd") == true))
    {
        int indexActionBegin = objActions["indexBegin"].toInt();
        int indexActionEnd = objActions["indexEnd"].toInt();

        for(int indexAction=indexActionBegin;indexAction<indexActionEnd;++indexAction)
        {
            QJsonObject objAction = objActions[QString::number(indexAction)].toObject();
            QString namePlayer = objActions["namePlayer"].toString();
            int phase = objAction["index"].toInt();

            if(!objAction.isEmpty())
            {
                Player* play =  m_gameManager->playerByName(namePlayer);

                switch(phase)
                {
                case ConstantesShared::PHASE_NotifNewGameCreated:
                {
                    if((objAction.contains("uidGame")) && (objAction.contains("nameGame")) && (objAction.contains("opponent")))
                    {
                        const int uidGame = objAction["uidGame"].toInt();
                        const QString nameGame = objAction["nameGame"].toString();
                        const QString opponent = objAction["opponent"].toString();
                        m_listOfGamesAvailable->addNewGame(uidGame, nameGame, opponent);
                    }
                }
                    break;

                case ConstantesShared::PHASE_NotifPlayerIsReady:
                {
                    if(objAction.contains("everyoneIsReady"))
                    {
                        const bool everyoneIsReady = objAction["everyoneIsReady"].toBool();
                        if(everyoneIsReady)
                        {
                            setGameStatus(ConstantesQML::StepGameInProgress);
                        }
                        else
                        {
                            QJsonArray arrayPlayers = objAction["players"].toArray();
                            for(int i=0;i<arrayPlayers.count();++i)
                            {
                                QJsonObject objPlayer = arrayPlayers[i].toObject();
                                if((!objPlayer.isEmpty()) && (objPlayer.contains("namePlayer")) && (objPlayer.contains("ready")))
                                {
                                    const QString namePlayer = objPlayer["namePlayer"].toString();
                                    bool initReady = objPlayer["ready"].toBool();
                                    Player* play = m_gameManager->playerByName(namePlayer);
                                    if(play != nullptr)
                                    {
                                        play->setInitReady(initReady);
                                    }
                                }
                            }

                        }

                    }
                    else
                        qWarning() << __PRETTY_FUNCTION__ << "PHASE_NotifPlayerIsReady does not contain \"everyoneIsReady\"";
                }
                    break;

                case ConstantesShared::PHASE_NotifEndOfTurn:
                {
                    if((objAction.contains("endOfTurn")) && (objAction.contains("newTurn")))
                    {
                        const QString namePlayerEndOfTurn = objAction["endOfTurn"].toString();
                        const QString namePlayerNewTurn = objAction["newTurn"].toString();
                        Player* playerEndOfTurn = m_gameManager->playerByName(namePlayerEndOfTurn);
                        Player* playerNewTurn = m_gameManager->playerByName(namePlayerNewTurn);

                        if(playerEndOfTurn != nullptr)
                            playerEndOfTurn->setCanPlay(false);

                        if(playerNewTurn != nullptr)
                            playerNewTurn->setCanPlay(true);
                    }

                }
                    break;

                case ConstantesShared::PHASE_NotifCardMoved:
                {
                    const int idPacketOrigin = objAction["idPacketOrigin"].toInt();
                    const int idPacketDestination = objAction["idPacketDestination"].toInt();
                    const int indexCardOrigin = objAction["indexCardOrigin"].toInt();
                    AbstractPacket* packetOrigin = play->packetFromEnumPacket(static_cast<ConstantesShared::EnumPacket>(idPacketOrigin));
                    AbstractPacket* packetDestination = play->packetFromEnumPacket(static_cast<ConstantesShared::EnumPacket>(idPacketDestination));

                    if(objAction.contains("idCard"))
                    {
                        Database db;
                        const int idCard = objAction["idCard"].toInt();
                        AbstractCard* abCard = db.cardById(idCard);

                        if(abCard != nullptr)
                            play->moveCardFromPacketToAnother(packetOrigin, packetDestination, indexCardOrigin, abCard);
                        else
                            qCritical() << __PRETTY_FUNCTION__ << "abCard id nullptr for " << idCard;
                    }
                    else
                        play->moveCardFromPacketToAnother(packetOrigin, packetDestination, indexCardOrigin);
                }
                    break;

                case ConstantesShared::PHASE_NotifDataPokemonChanged:
                {
                    if((objAction.contains("namePlayer")) && (objAction.contains("idPacket")) && (objAction.contains("indexCard")))
                    {
                        const QString namePlayer = objAction["namePlayer"].toString();
                        Player* play = m_gameManager->playerByName(namePlayer);
                        if(play != nullptr)
                        {
                            int idPacket = objAction["idPacket"].toInt();
                            ConstantesShared::EnumPacket ePacket = static_cast<ConstantesShared::EnumPacket>(idPacket);
                            AbstractPacket* packet = play->packetFromEnumPacket(ePacket);
                            if(packet != nullptr)
                            {
                                const int indexCard = objAction["indexCard"].toInt();
                                AbstractCard* abCard = packet->card(indexCard);
                                if(abCard != nullptr)
                                {
                                    if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
                                    {
                                        CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
                                        int lifeLeft = objAction["lifeLeft"].toInt();
                                        QJsonArray arrayAttacks = objAction["attacks"].toArray();

                                        pokemon->setLifeLeft(static_cast<unsigned short>(lifeLeft));
                                        for(int i=0;i<arrayAttacks.count();++i)
                                        {
                                            QJsonObject objAttack = arrayAttacks[i].toObject();
                                            if(!objAttack.isEmpty())
                                                pokemon->setAttackBlocked(objAttack["index"].toInt(), objAttack["available"].toBool());
                                        }
                                    }
                                    else
                                        qWarning() << __PRETTY_FUNCTION__ << "abCard is not a pokemon card " << abCard->type();

                                }
                                else
                                    qWarning() << __PRETTY_FUNCTION__ << "abCard " << indexCard << " is nullptr";

                            }
                            else
                                qWarning() << __PRETTY_FUNCTION__ << "packet " << idPacket << " is nullptr";

                        }
                        else
                            qWarning() << __PRETTY_FUNCTION__ << "player " << namePlayer << " is nullptr";
                    }
                }
                    break;

                case ConstantesShared::PHASE_NotifEnergyAdded:
                {
                    if((objAction.contains("namePlayer")) && (objAction.contains("idPacket")) && (objAction.contains("indexCard")) && (objAction.contains("elementEnergy")))
                    {
                        const QString namePlayer = objAction["namePlayer"].toString();

                        Player* play = m_gameManager->playerByName(namePlayer);
                        if(play != nullptr)
                        {
                            const int idPacket = objAction["idPacket"].toInt();
                            const ConstantesShared::EnumPacket ePacket = static_cast<ConstantesShared::EnumPacket>(idPacket);
                            AbstractPacket* packet = play->packetFromEnumPacket(ePacket);
                            if(packet != nullptr)
                            {
                                const int indexCard = objAction["indexCard"].toInt();
                                AbstractCard* abCard = packet->card(indexCard);
                                if(abCard != nullptr)
                                {
                                    if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
                                    {
                                        Database db;
                                        const int elementEnergy = objAction["elementEnergy"].toInt();
                                        CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);

                                        pokemon->addEnergy(db.newCardEnergyFromElement(static_cast<AbstractCard::Enum_element>(elementEnergy)));
                                    }
                                    else
                                        qWarning() << __PRETTY_FUNCTION__ << "abCard is not a pokemon card " << abCard->type();

                                }
                                else
                                    qWarning() << __PRETTY_FUNCTION__ << "abCard " << indexCard << " is nullptr";

                            }
                            else
                                qWarning() << __PRETTY_FUNCTION__ << "packet " << idPacket << " is nullptr";

                        }
                        else
                            qWarning() << __PRETTY_FUNCTION__ << "player " << namePlayer << " is nullptr";


                    }

                }
                    break;

                case ConstantesShared::PHASE_NotifEnergyRemoved:
                {
                    if((objAction.contains("namePlayer")) && (objAction.contains("idPacket")) && (objAction.contains("indexCard")) && (objAction.contains("indexEnergy")))
                    {
                        const QString namePlayer = objAction["namePlayer"].toString();

                        Player* play = m_gameManager->playerByName(namePlayer);
                        if(play != nullptr)
                        {
                            const int idPacket = objAction["idPacket"].toInt();
                            const ConstantesShared::EnumPacket ePacket = static_cast<ConstantesShared::EnumPacket>(idPacket);
                            AbstractPacket* packet = play->packetFromEnumPacket(ePacket);
                            if(packet != nullptr)
                            {
                                const int indexCard = objAction["indexCard"].toInt();
                                AbstractCard* abCard = packet->card(indexCard);
                                if(abCard != nullptr)
                                {
                                    if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
                                    {
                                        Database db;
                                        const int indexEnergy = objAction["indexEnergy"].toInt();
                                        CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);

                                        delete pokemon->takeEnergy(indexEnergy);
                                    }
                                    else
                                        qWarning() << __PRETTY_FUNCTION__ << "abCard is not a pokemon card " << abCard->type();

                                }
                                else
                                    qWarning() << __PRETTY_FUNCTION__ << "abCard " << indexCard << " is nullptr";

                            }
                            else
                                qWarning() << __PRETTY_FUNCTION__ << "packet " << idPacket << " is nullptr";

                        }
                        else
                            qWarning() << __PRETTY_FUNCTION__ << "player " << namePlayer << " is nullptr";


                    }

                }
                    break;

                default:
                    qWarning() << "phase " << phase << " not developp yet";
                }
            }
            else
                qWarning() << __PRETTY_FUNCTION__ << "objAction is empty";

        }
    }
    else
        qWarning() << __PRETTY_FUNCTION__ << "actions does not contains indexBegin and indexEnd";
}

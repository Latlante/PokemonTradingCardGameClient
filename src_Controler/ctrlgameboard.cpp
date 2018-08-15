#include "ctrlgameboard.h"
#include <QDir>
#include <QEventLoop>
#include <QJsonArray>
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
#include "src_Packets/packetgeneric.h"
#include "src_Packets/packethiddencards.h"
#include "src_Packets/packetpokemon.h"

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
    m_gameStatus(ConstantesQML::StepPreparation)
{
    connect(m_socket, &SocketClient::newNotification, this, &CtrlGameBoard::onNewNotification);
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
    m_ctrlAnim.setStepInProgress(true);

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

            m_gameManager->setPlayerYou(0, name);
            m_ctrlSelectingCards.setName(name);
            m_factoryMainPageLoader->displayCreateChooseGame();
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << "Error during the authentification";
        }
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << "Error during the connection";
    }

    m_ctrlAnim.setStepInProgress(false);
    /*m_ctrlSelectingCards.setName(name);
    m_factoryMainPageLoader->displaySelectCards();*/
}

void CtrlGameBoard::listOfAllPlayers()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_ctrlAnim.setStepInProgress(true);

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
            m_listAllPlayers->addNewPlayer(objPlayer["uid"].toString().toInt(), objPlayer["name"].toString());

            //m_listAllPlayers->addNewPlayer(i, arrayPlayers[i].toString());
        }

        if(m_listAllPlayers->rowCount() > 0)
        {
            m_factoryMainPageLoader->displayAllPlayers();
        }
    }
    m_ctrlAnim.setStepInProgress(false);
}

void CtrlGameBoard::listOfGamesAlreadyExisting()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_ctrlAnim.setStepInProgress(true);

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
        }
    }
    m_ctrlAnim.setStepInProgress(false);
}

void CtrlGameBoard::createANewGame(const QString& nameGame, int idOpponent)
{
    qDebug() << __PRETTY_FUNCTION__ << nameGame << idOpponent;
    m_ctrlAnim.setStepInProgress(true);

    QJsonDocument jsonResponse;

    if(m_socket->createANewGame(nameGame, idOpponent, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj.contains("actions"))
            executeActions(obj["actions"].toObject());

        if(obj["idGame"].toInt() != 0)
        {
            int idGame = obj["idGame"].toInt();
            m_listOfGamesAvailable->addNewGame(idGame, nameGame, m_listAllPlayers->namePlayerFromId(idOpponent));
            m_gameManager->setPlayerOpponent(static_cast<unsigned int>(idOpponent), m_listAllPlayers->namePlayerFromId(idOpponent));
            m_gameManager->setUidGame(static_cast<unsigned int>(idGame));
            m_factoryMainPageLoader->displaySelectCards();
        }
    }
    m_ctrlAnim.setStepInProgress(false);
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

void CtrlGameBoard::joinAGame(int idGame, const QString& nameGame, const QString &nameOpponent)
{
    qDebug() << __PRETTY_FUNCTION__ << idGame;
    QJsonDocument jsonResponse;
    m_ctrlAnim.setStepInProgress(true);

    if(m_socket->getAllInfoOnTheGame(idGame, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj.contains("actions"))
            executeActions(obj["actions"].toObject());

        if(obj["success"].toString() == "ok")
        {
            unsigned int uidPlayerOpponent = m_listAllPlayers->uidFromNamePlayer(nameOpponent);
            m_gameManager->setPlayerOpponent(uidPlayerOpponent, nameOpponent);
            m_gameManager->setUidGame(static_cast<unsigned int>(idGame));

            if(obj.contains("you"))
                fillBoardPlayerYou(obj["you"].toObject());

            if(obj.contains("enemy"))
                fillBoardPlayerOpponent(obj["enemy"].toObject());

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
    m_ctrlAnim.setStepInProgress(false);
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
    m_ctrlAnim.setStepInProgress(true);

    if(m_socket->sendCardsSelected(m_gameManager->uidGame(), listCards, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();
        qDebug() << __PRETTY_FUNCTION__ << "1";

        if(obj.contains("actions"))
            executeActions(obj["actions"].toObject());

        qDebug() << __PRETTY_FUNCTION__ << "2";

        if(obj["success"].toString() == "ok")
        {
            qDebug() << __PRETTY_FUNCTION__ << "3";
            m_factoryMainPageLoader->displayBoard();
            qDebug() << __PRETTY_FUNCTION__ << "4";
        }
        else
        {
            qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << "5";
    m_ctrlAnim.setStepInProgress(false);
}

void CtrlGameBoard::initReady()
{
    qDebug() << __PRETTY_FUNCTION__;
    QJsonDocument jsonResponse;
    m_ctrlAnim.setStepInProgress(true);

    if(m_socket->initIsReady(m_gameManager->uidGame(), jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj.contains("actions"))
            executeActions(obj["actions"].toObject());

        if(obj["success"].toString() == "ok")
        {
            m_ctrlAnim.setStepInProgress(false);
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
    m_ctrlAnim.setStepInProgress(true);

    if(m_socket->moveACard(m_gameManager->uidGame(), static_cast<ConstantesShared::EnumPacket>(idPacketOrigin), idCardOrigin, static_cast<ConstantesShared::EnumPacket>(idPacketDestination), idCardDestination, jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj.contains("actions"))
            executeActions(obj["actions"].toObject());

        if(obj["success"].toString() == "ok")
        {
            m_ctrlAnim.setStepInProgress(false);
        }
        else
        {
            qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
        }
    }
}

void CtrlGameBoard::attackRetreat(CardPokemon* pokemon)
{
    if(pokemon != nullptr)
    {
        qDebug() << __PRETTY_FUNCTION__ << pokemon->name();

        int indexAttack = m_ctrlPopups.displayAttacks(pokemon, true);

        if(indexAttack < 4)
        {
            QJsonDocument jsonResponse;
            m_ctrlAnim.setStepInProgress(true);

            if(m_socket->attack(m_gameManager->uidGame(), indexAttack, jsonResponse))
            {
                qDebug() << __PRETTY_FUNCTION__ << "request success";
                QJsonObject obj = jsonResponse.object();

                if(obj.contains("actions"))
                    executeActions(obj["actions"].toObject());

                if(obj["success"].toString() == "ok")
                {
                    m_ctrlAnim.setStepInProgress(false);
                    //Nothing to do because changes are in actions
                }
                else
                {
                    qWarning() << __PRETTY_FUNCTION__ << "no success:" << jsonResponse.toJson();
                }
            }
        }
    }
    else
        qWarning() << __PRETTY_FUNCTION__ << "pokemon is nullptr";

}

void CtrlGameBoard::skipTheTurn()
{
    qDebug() << __PRETTY_FUNCTION__;
    QJsonDocument jsonResponse;
    m_ctrlAnim.setStepInProgress(true);

    if(m_socket->skipTheTurn(m_gameManager->uidGame(), jsonResponse))
    {
        qDebug() << __PRETTY_FUNCTION__ << "request success";
        QJsonObject obj = jsonResponse.object();

        if(obj.contains("actions"))
            executeActions(obj["actions"].toObject());

        if(obj["success"].toString() == "ok")
        {
            m_ctrlAnim.setStepInProgress(false);
            //Nothing to do because changes are in actions
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

void CtrlGameBoard::onNewNotification(QJsonDocument docActions)
{
    qDebug() << __PRETTY_FUNCTION__ << docActions.toJson(QJsonDocument::Compact);
    QJsonObject objActions = docActions.object();
    if((objActions.contains("indexBegin")) && (objActions.contains("indexEnd")))
        executeActions(objActions);
}

/************************************************************
*****				FONCTIONS PRIVATE					*****
************************************************************/
void CtrlGameBoard::executeActions(QJsonObject objActions)
{
    qDebug() << __PRETTY_FUNCTION__;
    if((objActions.contains("indexBegin") == true) && (objActions.contains("indexEnd") == true))
    {
        int indexActionBegin = objActions["indexBegin"].toInt();
        int indexActionEnd = objActions["indexEnd"].toInt();

        qDebug() << __PRETTY_FUNCTION__ << indexActionBegin << indexActionEnd;

        for(int indexAction=indexActionBegin;indexAction<indexActionEnd;++indexAction)
        {
            QJsonObject objAction = objActions[QString::number(indexAction)].toObject();
            QString namePlayer = objAction["namePlayer"].toString();
            int phase = objAction["phase"].toInt();

            if(!objAction.isEmpty())
            {
                //Player* play =  m_gameManager->playerByName(namePlayer);
                /*qDebug() << play
                         << play->name()
                         << m_gameManager->playerYou()
                         << m_gameManager->playerYou()->name()
                         << m_gameManager->playerOpponent()
                         << m_gameManager->playerOpponent()->name();*/

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
                    Player* play = m_gameManager->playerByName(namePlayer);

                    if(play != nullptr)
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
                    else
                        qWarning() << "player (" << namePlayer << ") is nullptr for phase: " << phase;

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
                    if((objAction.contains("namePlayer")) && (objAction.contains("idPacketOrigin")) && (objAction.contains("indexCardOrigin")) && (objAction.contains("idPacketDestination")) && (objAction.contains("indexCardDestination")) && (objAction.contains("elementEnergy")))
                    {
                        const QString namePlayer = objAction["namePlayer"].toString();

                        Player* play = m_gameManager->playerByName(namePlayer);
                        if(play != nullptr)
                        {
                            const int idPacketOrigin = objAction["idPacketOrigin"].toInt();
                            const int idPacketDestination = objAction["idPacketDestination"].toInt();
                            const ConstantesShared::EnumPacket ePacketOrigin = static_cast<ConstantesShared::EnumPacket>(idPacketOrigin);
                            const ConstantesShared::EnumPacket ePacketDestination = static_cast<ConstantesShared::EnumPacket>(idPacketDestination);
                            const int indexCardOrigin = objAction["indexCardOrigin"].toInt();
                            const int indexCardDestination = objAction["indexCardDestination"].toInt();
                            AbstractPacket* packetOrigin = play->packetFromEnumPacket(ePacketOrigin);
                            AbstractPacket* packetDestination = play->packetFromEnumPacket(ePacketDestination);
                            if((packetOrigin != nullptr) && (packetDestination != nullptr))
                            {
                                //Add energy to the pokemon
                                AbstractCard* abCardDestination = packetDestination->card(indexCardDestination);

                                if(abCardDestination != nullptr)
                                {
                                    if(abCardDestination->type() == AbstractCard::TypeOfCard_Pokemon)
                                    {
                                        Database db;
                                        const int elementEnergy = objAction["elementEnergy"].toInt();
                                        CardPokemon* pokemon = static_cast<CardPokemon*>(abCardDestination);

                                        AbstractCard* abCardEnergy = db.cardById(static_cast<AbstractCard::Enum_element>(elementEnergy));
                                        if(abCardEnergy->type() == AbstractCard::TypeOfCard_Energy)
                                        {
                                            pokemon->addEnergy(static_cast<CardEnergy*>(abCardEnergy));

                                        }
                                    }
                                    else
                                        qWarning() << __PRETTY_FUNCTION__ << "abCardDestination is not a pokemon card " << abCardDestination->type();

                                }
                                else
                                    qWarning() << __PRETTY_FUNCTION__ << "abCard " << indexCardDestination << " is nullptr";

                                //Remove in packet origin
                                AbstractCard* abCardOrigin = packetOrigin->card(indexCardOrigin);

                                if(abCardOrigin != nullptr)
                                {
                                    if(abCardOrigin->type() == AbstractCard::TypeOfCard_Energy)
                                    {
                                        if(!packetOrigin->remove(abCardOrigin))
                                        {
                                            qCritical() << __PRETTY_FUNCTION__ << "abCardOrigin cannot be deleted";
                                        }
                                    }
                                    else
                                        qWarning() << __PRETTY_FUNCTION__ << "abCardOrigin is not a energy card " << abCardOrigin->type();
                                }
                                else
                                    packetOrigin->remove(nullptr);
                            }
                            else
                                qWarning() << __PRETTY_FUNCTION__ << "packet " << idPacketOrigin << "or" << idPacketDestination << " is nullptr";

                        }
                        else
                            qWarning() << __PRETTY_FUNCTION__ << "player " << namePlayer << " is nullptr";


                    }
                    else
                        qWarning() << __PRETTY_FUNCTION__ << "PHASE_NotifEnergyAdded: an element is missing";

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

void CtrlGameBoard::fillBoardPlayerYou(QJsonObject objYou)
{
    Player* playerYou = m_gameManager->playerYou();
    if(playerYou != nullptr)
    {
        Database db;
        //Bench
        if(objYou.contains("bench"))
        {
            QJsonArray arrayBench = objYou["bench"].toArray();
            for(int indexBench=0;indexBench<arrayBench.count();++indexBench)
            {
                QJsonObject objPokemon = arrayBench[indexBench].toObject();
                AbstractCard* abCard = db.cardById(objPokemon["id"].toInt());

                if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
                {
                    CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
                    unsigned short damage = static_cast<unsigned short>(objPokemon["damage"].toInt());
                    pokemon->setLifeLeft(pokemon->lifeTotal() - damage);

                    if(objPokemon.contains("energies"))
                    {
                        QJsonArray arrayEnergies = objPokemon["energies"].toArray();
                        for(int indexEnergy=0;indexEnergy<arrayEnergies.count();++indexEnergy)
                        {
                            CardEnergy* energy = db.newCardEnergyFromElement(static_cast<AbstractCard::Enum_element>(arrayEnergies[indexEnergy].toInt()));
                            pokemon->addEnergy(energy);
                        }
                    }

                    playerYou->bench()->addNewCard(pokemon);
                }

            }
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "bench is not in json request";

        //Deck
        if(objYou.contains("deckCount"))
        {
            int deckCount = objYou["deckCount"].toInt();
            playerYou->deck()->setCountCard(deckCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "deckCount is not in json request";

        //Fight
        if(objYou.contains("fight"))
        {
            QJsonObject objPokemon = objYou["fight"].toObject();
            AbstractCard* abCard = db.cardById(objPokemon["id"].toInt());

            if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
            {
                CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
                unsigned short damage = static_cast<unsigned short>(objPokemon["damage"].toInt());
                pokemon->setLifeLeft(pokemon->lifeTotal() - damage);

                int idStatus = objPokemon["status"].toInt();
                pokemon->setStatus(static_cast<CardPokemon::Enum_statusOfPokemon>(idStatus));

                if(objPokemon.contains("energies"))
                {
                    QJsonArray arrayEnergies = objPokemon["energies"].toArray();
                    for(int indexEnergy=0;indexEnergy<arrayEnergies.count();++indexEnergy)
                    {
                        CardEnergy* energy = db.newCardEnergyFromElement(static_cast<AbstractCard::Enum_element>(arrayEnergies[indexEnergy].toInt()));
                        pokemon->addEnergy(energy);
                    }
                }

                playerYou->bench()->addNewCard(pokemon);
            }

        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "fight is not in json request";

        //Hand
        if(objYou.contains("hand"))
        {
            QJsonArray arrayIdCardInHand = objYou["hand"].toArray();
            for(int indexArrayHand=0;indexArrayHand<arrayIdCardInHand.count();++indexArrayHand)
            {
                AbstractCard* abCard = db.cardById(arrayIdCardInHand[indexArrayHand].toInt());
                playerYou->hand()->addNewCard(abCard);
            }
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "hand is not in json request";

        //Rewards
        if(objYou.contains("rewardsCount"))
        {
            int rewardsCount = objYou["rewardsCount"].toInt();
            playerYou->rewards()->setCountCard(rewardsCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "rewardsCount is not in json request";

        //Trash
        if(objYou.contains("trashCount"))
        {
            int trashCount = objYou["trashCount"].toInt();
            playerYou->trash()->setCountCard(trashCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "trashCount is not in json request";
    }
    else
        qWarning() << __PRETTY_FUNCTION__ << "player is nullptr";
}

void CtrlGameBoard::fillBoardPlayerOpponent(QJsonObject objEnemy)
{
    qDebug() << __PRETTY_FUNCTION__;
    Player* playerOpponent = m_gameManager->playerOpponent();
    if(playerOpponent != nullptr)
    {
        Database db;
        qDebug() << __PRETTY_FUNCTION__ << "Bench";
        //Bench
        if(objEnemy.contains("bench"))
        {
            QJsonArray arrayBench = objEnemy["bench"].toArray();
            for(int indexBench=0;indexBench<arrayBench.count();++indexBench)
            {
                QJsonObject objPokemon = arrayBench[indexBench].toObject();
                AbstractCard* abCard = db.cardById(objPokemon["id"].toInt());

                if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
                {
                    CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
                    unsigned short damage = static_cast<unsigned short>(objPokemon["damage"].toInt());
                    pokemon->setLifeLeft(pokemon->lifeTotal() - damage);

                    if(objPokemon.contains("energies"))
                    {
                        QJsonArray arrayEnergies = objPokemon["energies"].toArray();
                        for(int indexEnergy=0;indexEnergy<arrayEnergies.count();++indexEnergy)
                        {
                            CardEnergy* energy = db.newCardEnergyFromElement(static_cast<AbstractCard::Enum_element>(arrayEnergies[indexEnergy].toInt()));
                            pokemon->addEnergy(energy);
                        }
                    }

                    playerOpponent->bench()->addNewCard(pokemon);
                }

            }
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "bench is not in json request";

        //Deck
        qDebug() << __PRETTY_FUNCTION__ << "Deck";
        if(objEnemy.contains("deckCount"))
        {
            int deckCount = objEnemy["deckCount"].toInt();
            playerOpponent->deck()->setCountCard(deckCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "deckCount is not in json request";

        //Fight
        qDebug() << __PRETTY_FUNCTION__ << "Fight";
        if(objEnemy.contains("fight"))
        {
            QJsonObject objPokemon = objEnemy["fight"].toObject();
            AbstractCard* abCard = db.cardById(objPokemon["id"].toInt());

            if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
            {
                CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
                unsigned short damage = static_cast<unsigned short>(objPokemon["damage"].toInt());
                pokemon->setLifeLeft(pokemon->lifeTotal() - damage);

                int idStatus = objPokemon["status"].toInt();
                pokemon->setStatus(static_cast<CardPokemon::Enum_statusOfPokemon>(idStatus));

                if(objPokemon.contains("energies"))
                {
                    QJsonArray arrayEnergies = objPokemon["energies"].toArray();
                    for(int indexEnergy=0;indexEnergy<arrayEnergies.count();++indexEnergy)
                    {
                        CardEnergy* energy = db.newCardEnergyFromElement(static_cast<AbstractCard::Enum_element>(arrayEnergies[indexEnergy].toInt()));
                        pokemon->addEnergy(energy);
                    }
                }

                playerOpponent->bench()->addNewCard(pokemon);
            }

        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "fight is not in json request";

        //Hand
        qDebug() << __PRETTY_FUNCTION__ << "Hand";
        if(objEnemy.contains("handCount"))
        {
            int handCount = objEnemy["handCount"].toInt();
            playerOpponent->hand()->setCountCard(handCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "handCount is not in json request";

        //Rewards
        qDebug() << __PRETTY_FUNCTION__ << "Rewards";
        if(objEnemy.contains("rewardsCount"))
        {
            int rewardsCount = objEnemy["rewardsCount"].toInt();
            playerOpponent->rewards()->setCountCard(rewardsCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "rewardsCount is not in json request";

        //Trash
        qDebug() << __PRETTY_FUNCTION__ << "Trash";
        if(objEnemy.contains("trashCount"))
        {
            int trashCount = objEnemy["trashCount"].toInt();
            playerOpponent->trash()->setCountCard(trashCount);
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "trashCount is not in json request";

    }
    else
        qWarning() << __PRETTY_FUNCTION__ << "player is nullptr";


}

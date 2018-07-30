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
    m_gameManager(GameManager::createInstance()),
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
    //initGame();
    connect(m_gameManager, &GameManager::indexCurrentPlayerChanged, this, &CtrlGameBoard::currentPlayerChanged);

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
                                                   //objGame["opponent"].toString());
                                                   "opponent");
            }

            setStepInProgress(false);
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
            m_idGame = obj["idGame"].toInt();
            m_listOfGamesAvailable->addNewGame(m_idGame, nameGame, m_listAllPlayers->namePlayerFromId(idOpponent));
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

void CtrlGameBoard::onClicked_ButtonOk_SelectPlayers(QStringList listOfPlayers)
{
    m_factoryMainPageLoader->displaySelectCards();
    //m_ctrlSelectingCards.selectCards(listOfPlayers);

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
void CtrlGameBoard::onListsComplete_CtrlSelectingCards()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif


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

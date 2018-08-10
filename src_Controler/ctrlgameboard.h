#ifndef CTRLGAMEBOARD_H
#define CTRLGAMEBOARD_H

#include <QObject>
#include <QJsonDocument>

#include "gamemanager.h"

class QQmlEngine;
class QQmlApplicationEngine;
class MyModel;

class FactoryMainPageLoader;
class Player;
class CtrlAnimation;
class CtrlPopups;
class CtrlSelectingCards;
class ModelListAllPlayers;
class ModelListOfGamesAvailable;
class ModelListSelection;
class ModelPopupSelectCardInPacket;
class BenchArea;
class PacketDeck;
class PacketRewards;
class SocketClient;

class CtrlGameBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConstantesQML::StepGame gameStatus READ gameStatus NOTIFY gameStatusChanged)


public:
    explicit CtrlGameBoard(CtrlSelectingCards& ctrlSelectCards, CtrlPopups& ctrlPopups, CtrlAnimation& ctrlAnim, QObject *parent = nullptr);
    ~CtrlGameBoard();

    static void declareQML();
    bool install(QQmlApplicationEngine *pEngine);

    Q_INVOKABLE ModelListAllPlayers* modelAllPlayers();
    Q_INVOKABLE ModelListOfGamesAvailable* modelAllOfGamesAvailable();
    Q_INVOKABLE ModelListSelection* modelListSelect();
    Q_INVOKABLE FactoryMainPageLoader* factory();
    Q_INVOKABLE Player* playerYou();
    Q_INVOKABLE Player* playerOpponent();

    ConstantesQML::StepGame gameStatus();
    void setGameStatus(ConstantesQML::StepGame status);

    //Actions
    Q_INVOKABLE void authentificate(const QString& name, const QString& password);

    Q_INVOKABLE void listOfAllPlayers();
    Q_INVOKABLE void listOfGamesAlreadyExisting();
    Q_INVOKABLE void createANewGame(const QString& nameGame, int idOpponent);
    Q_INVOKABLE void listOfGamesAvailable();
    Q_INVOKABLE void joinAGame(int idGame, const QString& nameGame, const QString &nameOpponent);
    Q_INVOKABLE void returnToTheMenu();

    Q_INVOKABLE void sendCardsSelected();
    Q_INVOKABLE void initReady();

    Q_INVOKABLE void moveACard(int idPacketOrigin, int idCardOrigin, int idPacketDestination, int idCardDestination);

    Q_INVOKABLE void attackRetreat(CardPokemon* pokemon);
    Q_INVOKABLE void skipTheTurn();

    Q_INVOKABLE void testAnimation();
    Q_INVOKABLE void displayPageSelection();
    Q_INVOKABLE void validatePageSelection(const QString& item);

signals:
    void nextPlayer();
    void currentPlayerChanged();
    void gameStatusChanged();

private slots:
    void onMovingCardAnimationStart();
    void onNewNotification(QJsonDocument docActions);

private:
    SocketClient* m_socket;
    GameManager* m_gameManager;
    ModelListAllPlayers* m_listAllPlayers;
    ModelListOfGamesAvailable* m_listOfGamesAvailable;
    FactoryMainPageLoader* m_factoryMainPageLoader;
    CtrlAnimation& m_ctrlAnim;
    CtrlPopups& m_ctrlPopups;
    CtrlSelectingCards& m_ctrlSelectingCards;

    ConstantesQML::StepGame m_gameStatus;

    void executeActions(QJsonObject objActions);
    void fillBoardPlayerYou(QJsonObject objYou);
    void fillBoardPlayerOpponent(QJsonObject objEnemy);
};

#endif // CTRLGAMEBOARD_H

#ifndef CTRLGAMEBOARD_H
#define CTRLGAMEBOARD_H

#include <QObject>

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
class ModelPopupSelectCardInPacket;
class BenchArea;
class PacketDeck;
class PacketRewards;
class SocketClient;

class CtrlGameBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Player* currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(ConstantesQML::StepGame gameStatus READ gameStatus NOTIFY gameStatusChanged)
    Q_PROPERTY(bool stepInProgress READ stepInProgress WRITE setStepInProgress NOTIFY stepInProgressChanged)


public:
    explicit CtrlGameBoard(CtrlSelectingCards& ctrlSelectCards, CtrlPopups& ctrlPopups, CtrlAnimation& ctrlAnim, QObject *parent = nullptr);
    ~CtrlGameBoard();

    static void declareQML();
    bool install(QQmlApplicationEngine *pEngine);

    Player* currentPlayer();
    ConstantesQML::StepGame gameStatus();

    Q_INVOKABLE ModelListAllPlayers* modelAllPlayers();
    Q_INVOKABLE ModelListOfGamesAvailable* modelAllOfGamesAvailable();
    Q_INVOKABLE FactoryMainPageLoader* factory();
    Q_INVOKABLE void initGame();
    Q_INVOKABLE Player* playerAt(int index);

    bool stepInProgress();
    void setStepInProgress(bool inProgress);

    //Actions
    Q_INVOKABLE void authentificate(const QString& name, const QString& password);

    Q_INVOKABLE void listOfAllPlayers();
    Q_INVOKABLE void createANewGame(const QString& opponent);
    Q_INVOKABLE void listOfGamesAvailable();
    Q_INVOKABLE void joinAGame(int idGame);
    Q_INVOKABLE void returnToTheMenu();

    Q_INVOKABLE void onClicked_ButtonOk_SelectPlayers(QStringList listOfPlayers);
    Q_INVOKABLE void displaySelectingCardsForNextPlayers();
    Q_INVOKABLE void onClicked_ButtonOk_SelectCards();
    Q_INVOKABLE void onClicked_ButtonReadyPreparation();

    Q_INVOKABLE void actionAttack(CardPokemon* card);
    Q_INVOKABLE void actionFinishYourTurn();

    Q_INVOKABLE void testAnimation();


signals:
    void nextPlayer();
    void currentPlayerChanged();
    void gameStatusChanged();
    void stepInProgressChanged();

private slots:
    void onListsComplete_CtrlSelectingCards();

    void onDisplayPacketAsked(AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard);
    void onDisplayAllElementsAsked(unsigned short quantity);
    void onDisplaySelectHiddenCardAsked(PacketRewards *packet, unsigned short quantity);
    void onDisplayEnergiesForAPokemonAsked(CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element);
    void onDisplayAttacksAsked(CardPokemon* card, bool authorizeRetreat);
    void onDisplayMessageAsked(QString message);
    void onHeadOrTailAsked();

    void onMovingCardAnimationStart();

private:
    SocketClient* m_socket;
    GameManager* m_gameManager;
    ModelListAllPlayers* m_listAllPlayers;
    ModelListOfGamesAvailable* m_listOfGamesAvailable;
    FactoryMainPageLoader* m_factoryMainPageLoader;
    CtrlAnimation& m_ctrlAnim;
    CtrlPopups& m_ctrlPopups;
    CtrlSelectingCards& m_ctrlSelectingCards;

    bool m_stepInProgress;

};

#endif // CTRLGAMEBOARD_H

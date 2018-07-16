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
class ListPlayers;
class ModelPopupSelectCardInPacket;
class BenchArea;
class PacketDeck;
class PacketRewards;

class CtrlGameBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Player* currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(ConstantesQML::StepGame gameStatus READ gameStatus NOTIFY gameStatusChanged)

public:
    explicit CtrlGameBoard(CtrlSelectingCards& ctrlSelectCards, CtrlPopups& ctrlPopups, CtrlAnimation& ctrlAnim, QObject *parent = nullptr);
    ~CtrlGameBoard();

    static void declareQML();

    bool install(QQmlApplicationEngine *pEngine);

    Player* currentPlayer();
    ConstantesQML::StepGame gameStatus();

    Q_INVOKABLE ListPlayers* newListPlayers();
    Q_INVOKABLE FactoryMainPageLoader* factory();
    Q_INVOKABLE void initGame();
    Q_INVOKABLE Player* playerAt(int index);

    //Authentification
    Q_INVOKABLE void authentifiate(const QString& name, const QString& password);

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
    GameManager* m_gameManager;
    FactoryMainPageLoader* m_factoryMainPageLoader;
    CtrlAnimation& m_ctrlAnim;
    CtrlPopups& m_ctrlPopups;
    CtrlSelectingCards& m_ctrlSelectingCards;

};

#endif // CTRLGAMEBOARD_H

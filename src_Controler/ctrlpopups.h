#ifndef CTRLPOPUPS_H
#define CTRLPOPUPS_H

#include <QObject>

#include "src_Cards/abstractcard.h"

class CardEnergy;
class CardPokemon;
class AbstractPacket;
class PacketGeneric;
class ModelPopupSelectCardInPacket;
class ModelPopupSelectEnergyInPokemon;
class QQmlEngine;
class QQmlApplicationEngine;

class CtrlPopups : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool onePopupIsDisplayed READ onePopupIsDisplayed NOTIFY onePopupIsDisplayedChanged)
    Q_PROPERTY(bool popupIsEnabled READ popupIsEnabled WRITE setPopupIsEnabled NOTIFY popupIsEnabledChanged)

    //SELECT CARD IN PACKET
    Q_PROPERTY(bool selectCardInPacketVisible READ selectCardInPacketVisible WRITE setSelectCardInPacketVisible NOTIFY selectCardInPacketVisibleChanged)

    //SELECT HIDDEN CARD
    Q_PROPERTY(bool selectHiddenCardVisible READ selectHiddenCardVisible WRITE setSelectHiddenCardVisible NOTIFY selectHiddenCardVisibleChanged)
    Q_PROPERTY(bool animationFlippedCards READ animationFlippedCards NOTIFY animationFlippedCardsChanged)

    //SELECT ATTACK
    Q_PROPERTY(bool popupSelectingAttacks_Visible READ popupSelectingAttacks_Visible WRITE setPopupSelectingAttacks_Visible NOTIFY popupSelectingAttacks_VisibleChanged)
    Q_PROPERTY(CardPokemon* popupSelectingAttacks_Card READ popupSelectingAttacks_Card NOTIFY popupSelectingAttacks_CardChanged)
    Q_PROPERTY(int popupSelectingAttacks_IndexAttack READ popupSelectingAttacks_IndexAttack WRITE setPopupSelectingAttacks_IndexAttack NOTIFY popupSelectingAttacks_IndexAttackChanged)
    Q_PROPERTY(bool popupSelectingAttacks_AuthorizeRetreat READ popupSelectingAttacks_AuthorizeRetreat WRITE setPopupSelectingAttacks_AuthorizeRetreat NOTIFY popupSelectingAttacks_AuthorizeRetreatChanged)

    //SELECT ENERGY IN POKEMON
    Q_PROPERTY(bool selectEnergiesInPokemonVisible READ selectEnergiesInPokemonVisible WRITE setSelectEnergiesInPokemonVisible NOTIFY selectEnergiesInPokemonVisibleChanged)

    //MESSAGE
    Q_PROPERTY(bool messageVisible READ messageVisible WRITE setMessageVisible NOTIFY messageVisibleChanged)
    Q_PROPERTY(QString messageContent READ messageContent WRITE setMessageContent NOTIFY messageContentChanged)

    //COIN HEAD OR TAIL
    Q_PROPERTY(bool headOrTailVisible READ headOrTailVisible WRITE setHeadOrTailVisible NOTIFY headOrTailVisibleChanged)
    Q_PROPERTY(int headOrTailValue READ headOrTailValue WRITE setHeadOrTailValue NOTIFY headOrTailValueChanged)

public:
    explicit CtrlPopups(QObject *parent = nullptr);
    ~CtrlPopups();

    static void declareQML();
    bool install(QQmlApplicationEngine *pEngine);

    bool onePopupIsDisplayed();
    void setOnePopupIsDisplayed(bool visible);

    bool popupIsEnabled();
    void setPopupIsEnabled(bool enable);

    //SELECT CARD IN PACKET
    Q_INVOKABLE ModelPopupSelectCardInPacket* modelSelectCardInPacket();
    void displayPacket(AbstractPacket* packet, unsigned short quantity = 1, AbstractCard::Enum_typeOfCard typeOfCard = AbstractCard::TypeOfCard_Whatever);
    QList<AbstractCard *> displayAllElements(unsigned short quantity = 1);
    void closeDisplayPacket();
    QList<AbstractCard*> listCardsSelected();
    QList<int> listIndexCardsSelected();
    bool selectCardInPacketVisible();
    void setSelectCardInPacketVisible(bool state);

    //SELECT HIDDEN CARD
    void displaySelectHiddenCard(int numberOfCards, unsigned short quantity);
    QList<int> listHiddenCardSelected();
    void selectHiddenCardShowCardsSelected(QList<AbstractCard*> listCards);
    bool selectHiddenCardVisible();
    void setSelectHiddenCardVisible(bool state);
    bool animationFlippedCards();
    void startAnimationFlippedCards();
    //Q_INVOKABLE void selectHiddenCardDone();

    //SELECT ATTACK
    void displayAttacks(CardPokemon* card, bool authorizeRetreat);
    int displayAttacksWithLoopEvent(CardPokemon* card, bool authorizeRetreat);
    bool popupSelectingAttacks_Visible();
    void setPopupSelectingAttacks_Visible(bool state);
    CardPokemon* popupSelectingAttacks_Card();
    void setPopupSelectingAttacks_Card(CardPokemon* card);
    int popupSelectingAttacks_IndexAttack();
    void setPopupSelectingAttacks_IndexAttack(int index);
    bool popupSelectingAttacks_AuthorizeRetreat();
    void setPopupSelectingAttacks_AuthorizeRetreat(bool authorize);

    //SELECT ENERGY IN POKEMON
    Q_INVOKABLE ModelPopupSelectEnergyInPokemon* modelSelectEnergyInPokemon();
    void displayEnergiesForAPokemon(CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element);
    void hideDisplayEnergiesForAPokemon();
    bool selectEnergiesInPokemonVisible();
    void setSelectEnergiesInPokemonVisible(bool state);

    //MESSAGE
    void displayMessage(QString message);
    bool messageVisible();
    void setMessageVisible(bool visible);
    QString messageContent();
    void setMessageContent(QString message);

    //COIN HEAD OR TAIL
    void displayHeadOrTail(int value);
    bool headOrTailVisible();
    void setHeadOrTailVisible(bool visible);
    int headOrTailValue();
    void setHeadOrTailValue(int value);

    Q_INVOKABLE void selectionCardsFinished();

signals:
    void onePopupIsDisplayedChanged();
    void popupIsEnabledChanged();

    //SELECT CARD IN PACKET
    void selectCardInPacketVisibleChanged();

    //SELECT HIDDEN CARD
    void selectHiddenCardVisibleChanged();
    void animationFlippedCardsChanged();
    void selectionHiddenCardFinished();

    //SELECT ATTACK
    void popupSelectingAttacks_VisibleChanged();
    void popupSelectingAttacks_CardChanged();
    void popupSelectingAttacks_IndexAttackChanged();
    void popupSelectingAttacks_AuthorizeRetreatChanged();

    //SELECT ENERGY IN POKEMON
    void selectEnergiesInPokemonVisibleChanged();

    //MESSAGE
    void messageVisibleChanged();
    void messageContentChanged();

    //COIN HEAD OR TAIL
    bool headOrTailVisibleChanged();
    int headOrTailValueChanged();

    void selectionFinished();

private:
    bool m_onePopupIsDisplayed;
    bool m_popupIsEnabled;

    //SELECT CARD IN PACKET
    ModelPopupSelectCardInPacket* m_modelSelectCardInPacket;
    bool m_selectCardInPacketVisible;

    //SELECT HIDDEN CARD
    bool m_selectHiddenCardVisible;
    bool m_animationFlippedCards;

    //SELECT ATTACK
    bool m_popupSelectingAttacks_Visible;
    CardPokemon* m_popupSelectingAttacks_Card;
    int m_popupSelectingAttacks_IndexAttack;
    bool m_popupSelectingAttacks_AuthorizeRetreat;

    //SELECT ENERGY IN POKEMON
    ModelPopupSelectEnergyInPokemon* m_modelSelectEnergyInPokemon;
    bool m_selectEnergiesInPokemonVisible;

    //MESSAGE
    bool m_messageVisible;
    QString m_messageContent;

    //COIN HEAD OR TAIL
    bool m_headOrTailVisible;
    int m_headOrTailValue;


    QList<AbstractCard *> displayAbstractPacket(AbstractPacket* packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard);
};

#endif // CTRLPOPUPS_H

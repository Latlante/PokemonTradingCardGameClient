#include "ctrlpopups.h"

#include <QDebug>
#include <QEventLoop>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqml.h>
#include <QTimer>

#include "common/database.h"
#include "src_Cards/cardpokemon.h"
#include "src_Models/modelpopupselectcardinpacket.h"
#include "src_Models/modelpopupselectenergyinpokemon.h"
#include "src_Packets/abstractpacket.h"
#include "src_Packets/packetgeneric.h"

CtrlPopups::CtrlPopups(QObject *parent) :
    QObject(parent),
    m_onePopupIsDisplayed(false),
    m_modelSelectCardInPacket(new ModelPopupSelectCardInPacket()),
    m_selectCardInPacketVisible(false),
    m_selectHiddenCardVisible(false),
    m_animationFlippedCards(false),
    m_popupSelectingAttacks_Visible(false),
    m_popupSelectingAttacks_Card(nullptr),
    m_popupSelectingAttacks_IndexAttack(-1),
    m_popupSelectingAttacks_AuthorizeRetreat(false),
    m_modelSelectEnergyInPokemon(new ModelPopupSelectEnergyInPokemon()),
    m_selectEnergiesInPokemonVisible(false),
    m_messageVisible(false),
    m_messageContent(""),
    m_headOrTailVisible(false),
    m_headOrTailValue(0)
{

}

CtrlPopups::~CtrlPopups()
{
    delete m_modelSelectCardInPacket;
    delete m_modelSelectEnergyInPokemon;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CtrlPopups::declareQML()
{
    qmlRegisterUncreatableType<CtrlPopups>("model", 1, 0, "CtrlPopups", "CtrlPopups error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool CtrlPopups::install(QQmlApplicationEngine *pEngine)
{
    qDebug() << "installing CtrlPopups...";

    bool bInstalled = false;

    if (NULL != pEngine)
    {
        QQmlContext* pContext = pEngine->rootContext();
        if (NULL != pContext)
        {
            bInstalled = true;
            qDebug() << "CtrlPopups is installed in QML engine";
            pContext->setContextProperty("ctrlPopups", this);
        }
        else
        {
            qCritical() << "Qml context is null.";
        }
    }

    return bInstalled;
}

bool CtrlPopups::onePopupIsDisplayed()
{
    return m_onePopupIsDisplayed;
}

void CtrlPopups::setOnePopupIsDisplayed(bool visible)
{
    if(m_onePopupIsDisplayed != visible)
    {
        m_onePopupIsDisplayed = visible;
        emit onePopupIsDisplayedChanged();
    }
}

//**************************************
//      SELECT CARD IN PACKET
//**************************************
ModelPopupSelectCardInPacket* CtrlPopups::modelSelectCardInPacket()
{
    return m_modelSelectCardInPacket;
}

void CtrlPopups::displayPacket(AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard)
{
    //Vérification
    unsigned short newQuantity = quantity;
    if(newQuantity > packet->countCard())
        newQuantity = packet->countCard();

    //Initialisation
    m_modelSelectCardInPacket->setTypeOfCardFilter(typeOfCard);
    m_modelSelectCardInPacket->setNumberOfCardsToSelect(newQuantity);
    m_modelSelectCardInPacket->addPacketFromAbstractPacket(packet);
    setSelectCardInPacketVisible(true);

    //En attente
    /*QEventLoop loop;
    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
    loop.exec();

    //Configuration de fin
    setSelectCardInPacketVisible(false);

    //Renvoi de l'information
    return m_modelSelectCardInPacket->listCardsSelected();*/
}

QList<AbstractCard *> CtrlPopups::displayAllElements(unsigned short quantity)
{
    /*Database db;
    QList<int> listIdEnergiesAvailable = db.listIdAllCardsEnergies();
    QList<AbstractCard*> listCards;
    foreach(int id, listIdEnergiesAvailable)
        listCards.append(db.cardById(id));

    PacketDynamicCustom* packetAllEnergies = new PacketDynamicCustom("all Energies", listCards);

    return displayPacket(packetAllEnergies, quantity);*/
}

void CtrlPopups::closeDisplayPacket()
{
    setSelectCardInPacketVisible(false);
}

QList<AbstractCard*> CtrlPopups::listCardsSelected()
{
    return m_modelSelectCardInPacket->listCardsSelected();
}

QList<int> CtrlPopups::listIndexCardsSelected()
{
    return m_modelSelectCardInPacket->listIndexSelected();
}

bool CtrlPopups::selectCardInPacketVisible()
{
    return m_selectCardInPacketVisible;
}

void CtrlPopups::setSelectCardInPacketVisible(bool state)
{
    if(m_selectCardInPacketVisible != state)
    {
        setOnePopupIsDisplayed(state);
        m_selectCardInPacketVisible = state;
        emit selectCardInPacketVisibleChanged();
    }
}

//**************************************
//          SELECT HIDDEN CARD
//**************************************
void CtrlPopups::displaySelectHiddenCard(int numberOfCards, unsigned short quantity)
{
    //PacketGeneric* packet = new PacketGeneric("hiddenCards", numberOfCards);

    //Initialisation
    m_modelSelectCardInPacket->setNumberOfCardsToSelect(quantity);
    m_modelSelectCardInPacket->addNumberOfCards(numberOfCards);
    setSelectHiddenCardVisible(true);

    //En attente
//    QEventLoop loop;
//    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
//    loop.exec();

//    //Configuration de fin
//    setSelectHiddenCardVisible(false);

//    return m_modelSelectCardInPacket->listIndexSelected();
}

QList<int> CtrlPopups::listHiddenCardSelected()
{
    return m_modelSelectCardInPacket->listIndexSelected();
}

void CtrlPopups::selectHiddenCardShowCardsSelected(QList<AbstractCard*> listCards)
{
    int indexLoop = 0;
    int indexListCards = 0;

    while((indexLoop < m_modelSelectCardInPacket->rowCount()) && (indexListCards < listCards.count()))
    {
        if(m_modelSelectCardInPacket->data(m_modelSelectCardInPacket->index(indexLoop, 0), ModelPopupSelectCardInPacket::SelectCardsRole_Selected).toBool() == true)
        {
            m_modelSelectCardInPacket->replaceCard(indexLoop, listCards[indexListCards]);
            indexListCards++;
        }
        indexLoop++;
    }

    //En attente
    /*QEventLoop loop;
    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
    loop.exec();*/
}

bool CtrlPopups::selectHiddenCardVisible()
{
    return m_selectHiddenCardVisible;
}

void CtrlPopups::setSelectHiddenCardVisible(bool state)
{
    if(m_selectHiddenCardVisible != state)
    {
        setOnePopupIsDisplayed(state);
        m_selectHiddenCardVisible = state;
        emit selectHiddenCardVisibleChanged();
    }
}

bool CtrlPopups::animationFlippedCards()
{
    return m_animationFlippedCards;
}

void CtrlPopups::startAnimationFlippedCards()
{
    if(m_animationFlippedCards != true)
    {
        //Création d'un effet de "Trigger"
        m_animationFlippedCards = true;
        emit animationFlippedCardsChanged();

        QEventLoop loop;
        QTimer timer;
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        timer.start(3000);
        loop.exec();

        m_animationFlippedCards = false;
        emit animationFlippedCardsChanged();

        //Configuration de fin
        setSelectHiddenCardVisible(false);
    }
}

/*void CtrlPopups::selectHiddenCardDone()
{
    emit selectionHiddenCardFinished();
}*/

//**************************************
//            SELECT ATTACK
//**************************************
int CtrlPopups::displayAttacks(CardPokemon* card, bool authorizeRetreat)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //Initialisation
    setPopupSelectingAttacks_IndexAttack(-1);
    setPopupSelectingAttacks_AuthorizeRetreat(authorizeRetreat);
    setPopupSelectingAttacks_Card(card);
    setPopupSelectingAttacks_Visible(true);

    qDebug() << __PRETTY_FUNCTION__ << "En attente";

    //En attente
    QEventLoop loop;
    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << __PRETTY_FUNCTION__ << "Passé";

    //Configuration de fin
    setPopupSelectingAttacks_Visible(false);

    //Renvoi de l'information
    return m_popupSelectingAttacks_IndexAttack;
}

bool CtrlPopups::popupSelectingAttacks_Visible()
{
    return m_popupSelectingAttacks_Visible;
}

void CtrlPopups::setPopupSelectingAttacks_Visible(bool state)
{
    if(m_popupSelectingAttacks_Visible != state)
    {
        setOnePopupIsDisplayed(state);
        m_popupSelectingAttacks_Visible = state;
        emit popupSelectingAttacks_VisibleChanged();
    }
}

CardPokemon* CtrlPopups::popupSelectingAttacks_Card()
{
    return m_popupSelectingAttacks_Card;
}

void CtrlPopups::setPopupSelectingAttacks_Card(CardPokemon *card)
{
    if(m_popupSelectingAttacks_Card != card)
    {
        m_popupSelectingAttacks_Card = card;
        emit popupSelectingAttacks_CardChanged();
    }
}

int CtrlPopups::popupSelectingAttacks_IndexAttack()
{
    return m_popupSelectingAttacks_IndexAttack;
}

void CtrlPopups::setPopupSelectingAttacks_IndexAttack(int index)
{
    if(m_popupSelectingAttacks_IndexAttack != index)
    {
        m_popupSelectingAttacks_IndexAttack = index;
        emit popupSelectingAttacks_IndexAttackChanged();
    }
}

bool CtrlPopups::popupSelectingAttacks_AuthorizeRetreat()
{
    return m_popupSelectingAttacks_AuthorizeRetreat;
}

void CtrlPopups::setPopupSelectingAttacks_AuthorizeRetreat(bool authorize)
{
    if(m_popupSelectingAttacks_AuthorizeRetreat != authorize)
    {
        m_popupSelectingAttacks_AuthorizeRetreat = authorize;
        emit popupSelectingAttacks_AuthorizeRetreatChanged();
    }
}

//**************************************
//      SELECT ENERGY IN POKEMON
//**************************************
ModelPopupSelectEnergyInPokemon* CtrlPopups::modelSelectEnergyInPokemon()
{
    QQmlEngine::setObjectOwnership(m_modelSelectEnergyInPokemon, QQmlEngine::CppOwnership);
    return m_modelSelectEnergyInPokemon;
}

QList<AbstractCard *> CtrlPopups::displayEnergiesForAPokemon(CardPokemon *pokemon, unsigned short quantity, AbstractCard::Enum_element element)
{
    //Vérification
    unsigned short newQuantity = quantity;
    if(newQuantity > pokemon->countEnergies())
        newQuantity = pokemon->countEnergies();

    //Initialisation
    m_modelSelectEnergyInPokemon->setElementFilter(element);
    m_modelSelectEnergyInPokemon->addListEnergyFromPokemon(pokemon);
    m_modelSelectEnergyInPokemon->setNumberOfEnergiesToSelect(newQuantity);
    setSelectEnergiesInPokemonVisible(true);

    //En attente
    QEventLoop loop;
    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
    loop.exec();

    //Configuration de fin
    setSelectEnergiesInPokemonVisible(false);

    //Renvoi de l'information
    return m_modelSelectEnergyInPokemon->listIndexEnergiesSelected();
}

bool CtrlPopups::selectEnergiesInPokemonVisible()
{
    return m_selectEnergiesInPokemonVisible;
}

void CtrlPopups::setSelectEnergiesInPokemonVisible(bool state)
{
    if(m_selectEnergiesInPokemonVisible != state)
    {
        setOnePopupIsDisplayed(state);
        m_selectEnergiesInPokemonVisible = state;
        emit selectEnergiesInPokemonVisibleChanged();
    }
}

void CtrlPopups::selectionCardsFinished()
{
    emit selectionFinished();
}

//**************************************
//              MESSAGE
//**************************************
void CtrlPopups::displayMessage(QString message)
{
    //Initialisation
    setMessageContent(message);
    setMessageVisible(true);

    //En attente
    QEventLoop loop;
    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
    loop.exec();

    //Configuration de fin
    setMessageVisible(false);
}

bool CtrlPopups::messageVisible()
{
    return m_messageVisible;
}

void CtrlPopups::setMessageVisible(bool visible)
{
    if(m_messageVisible != visible)
    {
        setOnePopupIsDisplayed(visible);
        m_messageVisible = visible;
        emit messageVisibleChanged();
    }
}

QString CtrlPopups::messageContent()
{
    return m_messageContent;
}

void CtrlPopups::setMessageContent(QString message)
{
    if(m_messageContent != message)
    {
        m_messageContent = message;
        emit messageContentChanged();
    }
}

//**************************************
//          COIN HEAD OR TAIL
//**************************************
void CtrlPopups::displayHeadOrTail(int value)
{
    //Initialisation
    setHeadOrTailValue(value);
    setHeadOrTailVisible(true);

    //En attente
    QEventLoop loop;
    connect(this, &CtrlPopups::selectionFinished, &loop, &QEventLoop::quit);
    loop.exec();

    //Configuration de fin
    setHeadOrTailVisible(false);
}

bool CtrlPopups::headOrTailVisible()
{
    return m_headOrTailVisible;
}

void CtrlPopups::setHeadOrTailVisible(bool visible)
{
    if(m_headOrTailVisible != visible)
    {
        setOnePopupIsDisplayed(visible);
        m_headOrTailVisible = visible;
        emit headOrTailVisibleChanged();
    }
}

int CtrlPopups::headOrTailValue()
{
    return m_headOrTailValue;
}

void CtrlPopups::setHeadOrTailValue(int value)
{
    if(m_headOrTailValue != value)
    {
        m_headOrTailValue = value;
        emit headOrTailValueChanged();
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/


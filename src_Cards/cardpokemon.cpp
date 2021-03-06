#include "cardpokemon.h"

#include <QDebug>
#include <QQmlEngine>
#include <QtQml/qqml.h>
#include <QUrl>
#include "gamemanager.h"
#include "player.h"
#include "common/utils.h"
#include "src_Cards/cardenergy.h"
#include "src_Models/modellistenergies.h"
#include "src_Packets/packetgeneric.h"

CardPokemon::CardPokemon(unsigned short id,
                         const QString& name,
                         AbstractCard::Enum_element element,
                         unsigned short lifeTotal,
                         QList<AttackData> listAttacks,
                         CardPokemon::Enum_element weaknessElement,
                         CardPokemon::Enum_CoefWeaknessResistance weaknessCoef,
                         CardPokemon::Enum_element resistanceElement,
                         CardPokemon::Enum_CoefWeaknessResistance resistanceCoef,
                         short evolutionFrom,
                         unsigned short costRetreat) :
    AbstractCard(id, name),
	m_element(element),
	m_lifeTotal(lifeTotal),
    m_lifeLeft(lifeTotal),
    m_status(Status_Normal),
    m_protectedAgainstDamageForTheNextTurnThreshold(0),
    m_protectedAgainstEffectForTheNextTurn(false),
	m_listAttacks(listAttacks),
    m_modelListEnergies(new ModelListEnergies()),
    m_weaknessElement(weaknessElement),
    m_weaknessCoef(weaknessCoef),
    m_resistanceElement(resistanceElement),
    m_resistanceCoef(resistanceCoef),
    m_cardEvolution(nullptr),
    m_evolutionFrom(evolutionFrom),
    m_costRetreat(costRetreat),
    m_damageOfPoisonPerRound(DAMAGE_POISON),
    m_destinyBond(false),
    m_lastDamageReceived(0)
{
	
}

CardPokemon::CardPokemon(const CardPokemon &card) :
    AbstractCard(),
    m_status(Status_Normal),
    m_protectedAgainstDamageForTheNextTurnThreshold(0),
    m_protectedAgainstEffectForTheNextTurn(false),
    m_modelListEnergies(new ModelListEnergies()),
    m_cardEvolution(nullptr)
{
    *this = card;
}
				
CardPokemon::~CardPokemon()
{
    qDebug() << "CardPokemon destroy";
    delete m_cardEvolution;
    delete m_modelListEnergies;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CardPokemon::declareQML()
{
    qmlRegisterUncreatableType<CardPokemon>("model", 1, 0, "CardPokemon", "CardPokemon error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int CardPokemon::id()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    int idCard = -1;

    if(m_cardEvolution != nullptr)
        idCard = m_cardEvolution->id();
    else
        idCard = m_id;

    return idCard;
}

const QString CardPokemon::name()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QString nameCard = "";

    if(m_cardEvolution != nullptr)
        nameCard = m_cardEvolution->name();
    else
        nameCard = m_name;

    return nameCard;
}

AbstractCard::Enum_typeOfCard CardPokemon::type()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return AbstractCard::TypeOfCard_Pokemon;
}

QUrl CardPokemon::image()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QUrl path;

    //qDebug() << __PRETTY_FUNCTION__ << this << m_id << m_name << m_cardEvolution;

    if(m_cardEvolution != nullptr)
        path = m_cardEvolution->image();
    else
        path = QUrl("pokemon/cards/" + QString::number(id()) + ".png");

    return path;
}

AbstractCard* CardPokemon::clone()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return new CardPokemon(*this);
}

AbstractCard::Enum_element CardPokemon::element()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //Element ne change pas entre les évolutions (pour le set de base)
    /*if(m_cardEvolution != nullptr)
        return m_cardEvolution->element();*/

    return m_element;
}

QString CardPokemon::elementFormatString()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return elementToString(element());
}

unsigned short CardPokemon::lifeTotal()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    if(m_cardEvolution != nullptr)
        return m_cardEvolution->lifeTotal();

	return m_lifeTotal;
}

void CardPokemon::setLifeTotal(unsigned short life)
{
    if(m_cardEvolution != nullptr)
        m_cardEvolution->setLifeTotal(life);
    else
    {
        if(lifeTotal() != life)
        {
            m_lifeTotal = life;
        }
    }
}

bool CardPokemon::isDied()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return lifeLeft() <= 0;
}

unsigned short CardPokemon::lifeLeft()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_lifeLeft;
}

void CardPokemon::setLifeLeft(unsigned short life)
{
    if(lifeLeft() != life)
    {
        m_lifeLeft = life;
        emit lifeLeftChanged();
    }
}

unsigned short CardPokemon::damageMarker()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return (lifeTotal() - lifeLeft()) / DAMAGE_MARQUER_VALUE;
}

CardPokemon::Enum_statusOfPokemon CardPokemon::status()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

	return m_status;
}

QString CardPokemon::statusFormatString()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return statusToString(status());
}

void CardPokemon::setStatus(Enum_statusOfPokemon status)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    if((m_status != status) && (!isProtectedAgainstEffectForTheNextTurn()) && (status != Status_None))
    {
        m_status = status;
        emit statusChanged();
        emit dataChanged();
    }
}

bool CardPokemon::isProtectedAgainstDamageForTheNextTurn()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_protectedAgainstDamageForTheNextTurnThreshold != 0;
}

unsigned short CardPokemon::protectedAgainstDamageForTheNextTurnThreshold()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_protectedAgainstDamageForTheNextTurnThreshold;
}

void CardPokemon::setProtectedAgainstDamageForTheNextTurn(bool status)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_protectedAgainstDamageForTheNextTurnThreshold = status == true ? 0xFFFF : 0;
}

void CardPokemon::setProtectedAgainstDamageForTheNextTurn(unsigned short threshold)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_protectedAgainstDamageForTheNextTurnThreshold = threshold;
}

bool CardPokemon::isProtectedAgainstEffectForTheNextTurn()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_protectedAgainstEffectForTheNextTurn;
}

void CardPokemon::setProtectedAgainstEffectForTheNextTurn(bool status)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_protectedAgainstEffectForTheNextTurn = status;
}

void CardPokemon::setInvincibleForTheNextTurn(bool status)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    setProtectedAgainstDamageForTheNextTurn(status);
    setProtectedAgainstEffectForTheNextTurn(status);
}

CardPokemon::Enum_element CardPokemon::weaknessElement()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    CardPokemon::Enum_element elementToReturn = m_weaknessElement;

    if(m_cardEvolution != nullptr)
        elementToReturn = m_cardEvolution->weaknessElement();

    return elementToReturn;
}

void CardPokemon::setWeaknessElement(CardPokemon::Enum_element element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    if(m_cardEvolution != nullptr)
        m_cardEvolution->setWeaknessElement(element);
    else
        m_weaknessElement = element;
}

CardPokemon::Enum_CoefWeaknessResistance CardPokemon::weaknessCoef()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_weaknessCoef;
}

void CardPokemon::setWeaknessCoef(CardPokemon::Enum_CoefWeaknessResistance coef)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_weaknessCoef = coef;
}

CardPokemon::Enum_element CardPokemon::resistanceElement()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    CardPokemon::Enum_element elementToReturn = m_resistanceElement;

    if(m_cardEvolution != nullptr)
        elementToReturn = m_cardEvolution->resistanceElement();

    return elementToReturn;
}

void CardPokemon::setResistanceElement(CardPokemon::Enum_element element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    if(m_cardEvolution != nullptr)
        m_cardEvolution->setResistanceElement(element);
    else
        m_resistanceElement = element;
}

CardPokemon::Enum_CoefWeaknessResistance CardPokemon::resistanceCoef()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_resistanceCoef;
}

void CardPokemon::setResistanceCoef(CardPokemon::Enum_CoefWeaknessResistance coef)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_resistanceCoef = coef;
}

QList<AttackData> CardPokemon::listAttacks()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    if(m_cardEvolution != nullptr)
        return m_cardEvolution->listAttacks();

	return m_listAttacks;
}

int CardPokemon::attacksCount()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return listAttacks().count();
}

QString CardPokemon::attackName(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QString nameToReturn = "";

    if((index >= 0) && (index < attacksCount()))
        nameToReturn = listAttacks()[index].name;

    return nameToReturn;
}

QString CardPokemon::attackDescription(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QString descriptionToReturn = "";

    if((index >= 0) && (index < attacksCount()))
        descriptionToReturn = listAttacks()[index].description;

    return descriptionToReturn;
}

unsigned short CardPokemon::attackDamage(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    unsigned short damageToReturn = 0;

    if((index >= 0) && (index < attacksCount()))
        damageToReturn = listAttacks()[index].damage;

    return damageToReturn;
}

bool CardPokemon::replaceOneAttack(int index, AttackData data)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    bool statusBack = true;

    if((index >= 0) && (index < listAttacks().count()))
    {
        //Nettoyage de l'ancienne action
        AttackData currentData = listAttacks()[index];


        //On ne copie volontairement pas le coût en énergie
        //currentData.costEnergies = data.costEnergies;
        currentData.damage = data.damage;
        currentData.description = data.description;
        currentData.name = data.name;
        currentData.blocked = false;

        m_listAttacks.replace(index, currentData);
    }
    else
        statusBack = false;

    return statusBack;
}

bool CardPokemon::attackIsBlocked(int indexAttack)
{
    bool result = true;

    if((indexAttack >= 0) && (indexAttack < listAttacks().count()))
        result = listAttacks()[indexAttack].blocked;

    return result;
}

void CardPokemon::setAttackBlocked(int indexAttack, bool blocked)
{
    if((indexAttack >= 0) && (indexAttack < listAttacks().count()))
    {
        AttackData data = listAttacks()[indexAttack];
        data.blocked = blocked;
        m_listAttacks.replace(indexAttack, data);
    }
}

#ifdef TESTS_UNITAIRES
void CardPokemon::setAttacks(int index, AttackData data)
{
    if(index > m_listAttacks.count())
        m_listAttacks.append(data);
    else
        m_listAttacks.replace(index, data);
}
#endif

void CardPokemon::addEnergy(CardEnergy *energy)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    m_modelListEnergies->addEnergy(energy);
}

CardEnergy* CardPokemon::takeEnergy(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_modelListEnergies->takeEnergy(index);
}

QList<CardEnergy*> CardPokemon::takeAllEnergies()
{
    return m_modelListEnergies->takeAllEnergies();
}

void CardPokemon::moveEnergiesInTrash(QList<CardEnergy*> listEnergies)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //On les supprime
    foreach(CardEnergy* energy, listEnergies)
        m_modelListEnergies->removeEnergy(energy);

    //Maintenant on peut les placer dans la défausse
    foreach(CardEnergy* energy, listEnergies)
        m_owner->trash()->addNewCard(energy);
}

void CardPokemon::moveAllEnergiesInTrash()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    //On récupére les énergies
    QList<CardEnergy*> listEnergies = m_modelListEnergies->takeAllEnergies();

    //Maintenant on peut les placer dans la défausse
    foreach(CardEnergy* energy, listEnergies)
        m_owner->trash()->addNewCard(energy);
}

unsigned short CardPokemon::countEnergies()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_modelListEnergies->countEnergies();
}

unsigned short CardPokemon::countEnergies(Enum_element element)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_modelListEnergies->countEnergies(element);
}

ModelListEnergies* CardPokemon::modelListOfEnergies()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QQmlEngine::setObjectOwnership(m_modelListEnergies, QQmlEngine::CppOwnership);
    return m_modelListEnergies;
}

bool CardPokemon::isBase()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

	return -1 == m_evolutionFrom;
}

bool CardPokemon::isSubEvolutionOf(CardPokemon* evolution)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_id == evolution->m_evolutionFrom;
}

bool CardPokemon::isEvolutionOf(CardPokemon* subEvolution)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return m_evolutionFrom == subEvolution->m_id;
}

unsigned short CardPokemon::costRetreat()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    unsigned short cost;

    if(m_cardEvolution != nullptr)
        cost = m_cardEvolution->costRetreat();
    else
        cost = m_costRetreat;

    return cost;
}

bool CardPokemon::canRetreat()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return countEnergies() >= m_costRetreat;
}

CardPokemon& CardPokemon::operator =(const CardPokemon& source)
{
    m_id = source.m_id;
    m_name = source.m_name;
    m_owner = source.m_owner;
    m_element = source.m_element;
    m_lifeTotal = source.m_lifeTotal;
    m_listAttacks = source.m_listAttacks;
    //m_modelListEnergies = source.m_modelListEnergies;     //Pas besoin
    m_cardEvolution = source.m_cardEvolution;
    m_evolutionFrom = source.m_evolutionFrom;
    m_costRetreat = source.m_costRetreat;
    m_weaknessElement = source.m_weaknessElement;
    m_weaknessCoef = source.m_weaknessCoef;
    m_resistanceElement = source.m_resistanceElement;
    m_resistanceCoef = source.m_resistanceCoef;

    m_damageOfPoisonPerRound = source.m_damageOfPoisonPerRound;
    m_destinyBond = source.m_destinyBond;
    m_lastAttackUsed = source.m_lastAttackUsed;


    return *this;
}

QList<AbstractCard*> CardPokemon::purge()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    QList<AbstractCard*> listPurge;

    if(m_cardEvolution != nullptr)
    {
        listPurge = m_cardEvolution->purge();
        listPurge.append(m_cardEvolution);
        m_cardEvolution = nullptr;
    }

    QList<CardEnergy*> listEnergies = m_modelListEnergies->takeAllEnergies();
    foreach(CardEnergy* energy, listEnergies)
        listPurge.append(energy);

    return listPurge;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QString CardPokemon::statusToString(Enum_statusOfPokemon status)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    switch(status)
    {
    case Status_Confused:   return "Confus";
    case Status_Normal:     return "OK";
    case Status_Paralyzed:  return "Paralysé";
    case Status_Poisoned:   return "Empoisonné";
    case Status_Slept:      return "Endormi";
    default: return "Erreur de conversion de status";
    }

    return "Erreur de conversion de status";
}


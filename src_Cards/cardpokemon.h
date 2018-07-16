#ifndef CARDPOKEMON_H
#define CARDPOKEMON_H

#include "abstractcard.h"

#include <QMap>

class AbstractAction;
class CardEnergy;
class ModelListEnergies;

struct AttackData
{
	QString name;
	QString description;
	unsigned short damage;
    QMap<AbstractCard::Enum_element, unsigned short> costEnergies;
    AbstractAction* action;
    short numberOfTurnAttackStillBlocks;

    bool operator ==(const AttackData& extData)
    {
        return ((name == extData.name) &&
                (description == extData.description) &&
                (damage == extData.damage) &&
                (costEnergies == extData.costEnergies) &&
                (action == extData.action) &&
                (numberOfTurnAttackStillBlocks == extData.numberOfTurnAttackStillBlocks));
    }
};

class CardPokemon : public AbstractCard
{
	Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY hasEvolved)
    Q_PROPERTY(QUrl image READ image NOTIFY hasEvolved)
    Q_PROPERTY(unsigned short lifeTotal READ lifeTotal NOTIFY hasEvolved)
    Q_PROPERTY(unsigned short lifeLeft READ lifeLeft NOTIFY lifeLeftChanged)
    Q_PROPERTY(Enum_statusOfPokemon status READ status NOTIFY statusChanged)
	
public:
    enum Enum_statusOfPokemon
    {
        Status_None = -1,
        Status_Confused = 0,
        Status_Normal,
        Status_Paralyzed,
        Status_Poisoned,
        Status_Slept
    };
    Q_ENUMS(Enum_statusOfPokemon)

    enum Enum_StatusOfAttack
    {
        Attack_OK = 0,
        Attack_UnknownError,
        Attack_IndexNOK,
        Attack_NotEnoughEnergies,
        Attack_WrongStatus,
        Attack_AttackBlocked
    };

    enum Enum_CoefWeaknessResistance
    {
        WeaknessResistance_NoCoef = -1,
        WeaknessResistance_Coef2 = 0,
        WeaknessResistance_Difference30
    };

    CardPokemon(unsigned short id, 
				const QString& name, 
				AbstractCard::Enum_element element,
				unsigned short lifeTotal,
				QList<AttackData> listAttacks,
                CardPokemon::Enum_element weaknessElement,
                CardPokemon::Enum_CoefWeaknessResistance weaknessCoef,
                CardPokemon::Enum_element resistanceElement,
                CardPokemon::Enum_CoefWeaknessResistance resistanceCoef,
                short evolutionFrom = -1,
                unsigned short costRetreat = 0);
    CardPokemon(const CardPokemon& card);
    ~CardPokemon();

    static void declareQML();

    int id() override;
    const QString name() override;
    Q_INVOKABLE AbstractCard::Enum_typeOfCard type() override;
    Q_INVOKABLE QUrl image() override;
    AbstractCard* clone() override;
    Q_INVOKABLE AbstractCard::Enum_element element();
    Q_INVOKABLE QString elementFormatString();
    unsigned short lifeTotal();
    bool isDied();
    unsigned short lifeLeft();
    unsigned short damageMarker();
    Enum_statusOfPokemon status();
    Q_INVOKABLE QString statusFormatString();
    void setStatus(Enum_statusOfPokemon status);

    bool isProtectedAgainstDamageForTheNextTurn();
    unsigned short protectedAgainstDamageForTheNextTurnThreshold();
    void setProtectedAgainstDamageForTheNextTurn(bool status);
    void setProtectedAgainstDamageForTheNextTurn(unsigned short threshold);
    bool isProtectedAgainstEffectForTheNextTurn();
    void setProtectedAgainstEffectForTheNextTurn(bool status);
    void setInvincibleForTheNextTurn(bool status);

    CardPokemon::Enum_element weaknessElement();
    void setWeaknessElement(CardPokemon::Enum_element element);
    CardPokemon::Enum_CoefWeaknessResistance weaknessCoef();
    void setWeaknessCoef(CardPokemon::Enum_CoefWeaknessResistance coef);
    CardPokemon::Enum_element resistanceElement();
    void setResistanceElement(CardPokemon::Enum_element element);
    CardPokemon::Enum_CoefWeaknessResistance resistanceCoef();
    void setResistanceCoef(CardPokemon::Enum_CoefWeaknessResistance coef);


    Q_INVOKABLE QList<AttackData> listAttacks();
    Q_INVOKABLE int attacksCount();
    Q_INVOKABLE QString attackName(int index);
    Q_INVOKABLE QString attackDescription(int index);
    Q_INVOKABLE unsigned short attackDamage(int index);
    bool replaceOneAttack(int index, AttackData data);
    short numberOfTurnAttackStillBlocks(int indexAttack);
    void decrementNumberOfTurnAttackStillBlocks();
    void setNumberOfTurnAttackStillBlocks(int indexAttack, short value);
#ifdef TESTS_UNITAIRES
    void setAttacks(int index, AttackData data);
#endif
	
    void addEnergy(CardEnergy* energy);
    CardEnergy* takeEnergy(int index);
    void moveEnergiesInTrash(QList<CardEnergy *> listEnergies);
    void moveAllEnergiesInTrash();
	unsigned short countEnergies();
	unsigned short countEnergies(Enum_element element);
    Q_INVOKABLE ModelListEnergies* modelListOfEnergies();
	
    Enum_StatusOfAttack tryToAttack(int indexAttack, CardPokemon *enemy);
	void takeDamage(unsigned short damage);
    void killed();
    void restoreLife(unsigned short life);
	bool canAttackFromStatus();
	bool hasEnoughEnergies(AttackData attack);
	bool hasEnoughEnergies(int indexAttack);

    bool evolve(CardPokemon* evolution);
	bool isBase();
	bool isSubEvolutionOf(CardPokemon* evolution);
    bool isEvolutionOf(CardPokemon* subEvolution);

    void applyDamageIfPoisoned();
    unsigned short damagePoisonPerRound();
    void setDamagePoisonPerRound(unsigned short damage);
    bool isDestinyBond();
    void setDestinyBond(bool state);
    AttackData lastAttackUsed();
    int lastIndexOfAttackUsed();
    unsigned short lastDamageReceived();
    void resetLastDamageReceived();

    unsigned short costRetreat();
    bool canRetreat();

    CardPokemon& operator =(const CardPokemon& source);

    QList<AbstractCard*> purge();

signals:
    void lifeLeftChanged();
    void statusChanged();
    void listEnergiesChanged();
    void hasEvolved();

private:
	AbstractCard::Enum_element m_element;
	unsigned short m_lifeTotal;
    unsigned short m_damage;
    CardPokemon::Enum_statusOfPokemon m_status;
    unsigned short m_protectedAgainstDamageForTheNextTurnThreshold;
    bool m_protectedAgainstEffectForTheNextTurn;
    QList<AttackData> m_listAttacks;
    ModelListEnergies* m_modelListEnergies;
    CardPokemon::Enum_element m_weaknessElement;
    CardPokemon::Enum_CoefWeaknessResistance m_weaknessCoef;
    CardPokemon::Enum_element m_resistanceElement;
    CardPokemon::Enum_CoefWeaknessResistance m_resistanceCoef;
    CardPokemon* m_cardEvolution;
	short m_evolutionFrom;
    unsigned short m_costRetreat;

    unsigned short m_damageOfPoisonPerRound;
    bool m_destinyBond;
    AttackData m_lastAttackUsed;
    unsigned short m_lastDamageReceived;
	
    unsigned short currentDamage();
    void setDamage(unsigned short damage);
    QString statusToString(Enum_statusOfPokemon status);
    unsigned short calculOfNewDamageDependOfWeaknessAndResistance(CardPokemon *enemy, unsigned short originalDamage);
};

#endif // CARDPOKEMON_H

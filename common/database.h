#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Cards/cardaction.h"

#define INDEX_START_POKEMON     1
#define INDEX_START_ENERGIES    1000
#define INDEX_START_ACTION      1100

class Database : public QObject
{
    Q_OBJECT
public:
    enum Enum_InformationAttack
    {
        InfoAtt_Name = 0,
        InfoAtt_Description,
        InfoAtt_Damage,
        InfoAtt_DamageCaption,
        InfoAtt_FirstEnergies,
        InfoAtt_ActionType = InfoAtt_FirstEnergies + AbstractCard::Element_COUNT,
        InfoAtt_ActionArgument1,
        InfoAtt_ActionArgument2,
        InfoAtt_COUNT
    };

    enum Enum_InformationPokemonDatabase
    {
        InfoDbPok_Id = 0,
        InfoDbPok_Useable,
        InfoDbPok_Name,
        InfoDbPok_Life,
        InfoDbPok_Element,
        InfoDbPok_ElementSecond,
        InfoDbPok_Weakness,
        InfoDbPok_WeaknessCoef,
        InfoDbPok_Resistance,
        InfoDbPok_ResistanceCoef,
        InfoDbPok_CostRetreat,
        InfoDbPok_IdSubevolution,
        InfoDbPok_Att1,
        InfoDbPok_Att2 = InfoDbPok_Att1 + InfoAtt_COUNT,
        InfoDbPok_Att3 = InfoDbPok_Att2 + InfoAtt_COUNT,
        InfoDbPok_Capacity = InfoDbPok_Att3 + InfoAtt_COUNT
    };

    enum Enum_InformationEnergiesDatabase
    {
        InfoDbNrj_Id = 0,
        InfoDbNrj_Useable,
        InfoDbNrj_Name,
        InfoDbNrj_Element,
        InfoDbNrj_Quantity,
    };

    enum Enum_InformationDresseurDatabase
    {
        InfoDbTrainer_Id = 0,
        InfoDbTrainer_Useable,
        InfoDbTrainer_Name,
        InfoDbTrainer_Description,
        InfoDbTrainer_Type
        //InfoDbTrainer_Argument1,
        //InfoDbTrainer_Argument2
    };

    explicit Database(QObject *parent = nullptr);

    QList<int> listIdAllCardsPokemon();
    QList<int> listIdAllCardsEnergies();
    QList<int> listIdAllCardsTrainers();
    AbstractCard* cardById(int id);

signals:

private:
    static const QString m_PATH_DB_ENERGIES;
    static const QString m_PATH_DB_POKEMON;
    static const QString m_PATH_DB;

    CardPokemon* newCardPokemon(const QString &infoCsv);
    CardEnergy* newCardEnergy(const QString& infoCsv);
    CardAction* newCardTrainer(const QString& infoCsv);
};

#endif // DATABASE_H

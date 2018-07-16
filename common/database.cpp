#include "database.h"

#include <QFile>
#include <QVariant>
#include "src_Actions/actioncreationfactory.h"

const QString Database::m_PATH_DB_ENERGIES = ":/energies/db/db_energies.csv";
const QString Database::m_PATH_DB_POKEMON = ":/pokemon/db/db_pokemon.csv";
const QString Database::m_PATH_DB = ":/database.csv";

Database::Database(QObject *parent) : QObject(parent)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QList<int> Database::listIdAllCardsPokemon()
{
    QList<int> listId;

    QFile fichier(m_PATH_DB);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray textFromFile = fichier.readAll();
    QString contenuGeneral = QString::fromLatin1(textFromFile);
    QStringList textSplitted = contenuGeneral.split("\n");

    foreach(QString line, textSplitted)
    {
        unsigned int idPokemon = line.section(";", InfoDbPok_Id, InfoDbPok_Id).toInt();
        if((idPokemon >= INDEX_START_POKEMON) &&
                (idPokemon < INDEX_START_ENERGIES) &&
                (line.section(";", InfoDbPok_Useable, InfoDbPok_Useable) == "1"))
        {
            listId.append(idPokemon);
        }
    }

    return listId;
}

QList<int> Database::listIdAllCardsEnergies()
{
    QList<int> listId;

    QFile fichier(m_PATH_DB);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray textFromFile = fichier.readAll();
    QString contenuGeneral = QString::fromLatin1(textFromFile);
    QStringList textSplitted = contenuGeneral.split("\n");

    foreach(QString line, textSplitted)
    {
        int idEnergy = line.section(";", InfoDbNrj_Id, InfoDbNrj_Id).toInt();
        if((idEnergy >= INDEX_START_ENERGIES) &&
                (idEnergy < INDEX_START_ACTION) &&
                (line.section(";", InfoDbNrj_Useable, InfoDbNrj_Useable) == "1"))
        {
            listId.append(idEnergy);
        }

    }

    return listId;
}

QList<int> Database::listIdAllCardsTrainers()
{
    QList<int> listId;

    QFile fichier(m_PATH_DB);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray textFromFile = fichier.readAll();
    QString contenuGeneral = QString::fromLatin1(textFromFile);
    QStringList textSplitted = contenuGeneral.split("\n");

    foreach(QString line, textSplitted)
    {
        int idTrainer = line.section(";", InfoDbTrainer_Id, InfoDbTrainer_Id).toInt();
        if((idTrainer >= INDEX_START_ACTION) &&
                (line.section(";", InfoDbTrainer_Useable, InfoDbTrainer_Useable) == "1"))
        {
            listId.append(idTrainer);
        }

    }

    return listId;
}

AbstractCard* Database::cardById(int id)
{
    AbstractCard* cardToReturn = NULL;
    QFile fichier(m_PATH_DB);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray textFromFile = fichier.readAll();
    QString contenuGeneral = QString::fromLatin1(textFromFile);
    QStringList contenuParLigne = contenuGeneral.split("\n");

    int indexLine = -1;
    for(int i=0;i<contenuParLigne.count();++i)
    //foreach(QString ligne, contenuParLigne)
    {
        int idCard = contenuParLigne[i].section(";", 0, 0).toInt();

        if (idCard == id)
        {
            indexLine = i;
            break;
        }
    }


    if ((id >= INDEX_START_POKEMON) && (id < INDEX_START_ENERGIES))
    {
        cardToReturn = newCardPokemon(contenuParLigne[indexLine]);
    }
    else if ((id >= INDEX_START_ENERGIES) && (id < INDEX_START_ACTION))
    {
        cardToReturn = newCardEnergy(contenuParLigne[indexLine]);
    }
    else if(id >= INDEX_START_ACTION)
    {
        cardToReturn = newCardTrainer(contenuParLigne[indexLine]);
    }

    return cardToReturn;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
CardPokemon* Database::newCardPokemon(const QString& infoCsv)
{
    CardPokemon* cardPokemonToReturn = NULL;
    QStringList arguments = infoCsv.split(";");

    if(arguments[InfoDbPok_Useable] == "1")
    {
        QList<AttackData> listAttacks;
        int offset = InfoDbPok_Att1;
        for(int i=0;i<3;++i)
        {
            if(arguments[offset+InfoAtt_Name] != "")
            {
                AttackData attack;
                attack.name = arguments[offset+InfoAtt_Name];
                attack.description = arguments[offset+InfoAtt_Description];
                attack.damage = arguments[offset+InfoAtt_Damage].toInt();

                QMap<AbstractCard::Enum_element, unsigned short> listEnergies;
                for(int indexEnergies=0;indexEnergies<AbstractCard::Element_COUNT;++indexEnergies)
                {
                    int indexEnergy = static_cast<int>(InfoAtt_FirstEnergies)+indexEnergies;
                    QString cellContains = arguments[offset+indexEnergy];

                    if (cellContains != "")
                    {
                        listEnergies.insert(static_cast<AbstractCard::Enum_element>(indexEnergies), cellContains.toUShort());
                    }
                }

                attack.costEnergies = listEnergies;
                bool ok;
                int idAction = arguments[offset+InfoAtt_ActionType].toInt(&ok);
                attack.action = ActionCreationFactory::createAction(static_cast<AbstractAction::Enum_typeOfAction>(idAction),
                                                                    QVariant::fromValue(arguments[offset+InfoAtt_ActionArgument1]),
                                                                    QVariant::fromValue(arguments[offset+InfoAtt_ActionArgument2]));
                attack.numberOfTurnAttackStillBlocks = 0;
                listAttacks.append(attack);
            }
            offset += InfoAtt_COUNT;
        }

        cardPokemonToReturn = new CardPokemon(arguments[InfoDbPok_Id].toInt(),
                                              arguments[InfoDbPok_Name],
                                              static_cast<AbstractCard::Enum_element>(arguments[InfoDbPok_Element].toInt()),
                                              arguments[InfoDbPok_Life].toUShort(),
                                              listAttacks,
                                              static_cast<CardPokemon::Enum_element>(arguments[InfoDbPok_Weakness].toUShort()),
                                              static_cast<CardPokemon::Enum_CoefWeaknessResistance>(arguments[InfoDbPok_WeaknessCoef].toUShort()),
                                              static_cast<CardPokemon::Enum_element>(arguments[InfoDbPok_Resistance].toUShort()),
                                              static_cast<CardPokemon::Enum_CoefWeaknessResistance>(arguments[InfoDbPok_ResistanceCoef].toUShort()),
                                              arguments[InfoDbPok_IdSubevolution] == "" ? -1 : arguments[InfoDbPok_IdSubevolution].toShort(),
                                              arguments[InfoDbPok_CostRetreat].toUShort());
    }

    return cardPokemonToReturn;
}

CardEnergy* Database::newCardEnergy(const QString &infoCsv)
{
    CardEnergy* cardEnergyToReturn = NULL;
    QStringList arguments = infoCsv.split(";");

    if(arguments[InfoDbNrj_Useable] == "1")
    {
        cardEnergyToReturn = new CardEnergy(arguments[InfoDbNrj_Id].toInt(),
                                            arguments[InfoDbNrj_Name],
                                            static_cast<AbstractCard::Enum_element>(arguments[InfoDbNrj_Id].toInt()-INDEX_START_ENERGIES),
                                            arguments[InfoDbNrj_Quantity].toUShort());
    }

    return cardEnergyToReturn;
}

CardAction *Database::newCardTrainer(const QString &infoCsv)
{
    CardAction* cardTrainerToReturn = NULL;
    QStringList arguments = infoCsv.split(";");
    bool ok;

    if(arguments[InfoDbTrainer_Useable] == "1")
    {
        int idAction = arguments[InfoDbTrainer_Type].toInt(&ok);

        if(ok)
        {
            cardTrainerToReturn = new CardAction(arguments[InfoDbTrainer_Id].toInt(),
                                                  arguments[InfoDbTrainer_Name],
                                                  arguments[InfoDbTrainer_Description],
                                                  ActionCreationFactory::createAction(static_cast<AbstractAction::Enum_typeOfAction>(idAction)));
                                                                                      //QVariant::fromValue(arguments[InfoDbTrainer_Argument1]),
                                                                                      //QVariant::fromValue(arguments[InfoDbTrainer_Argument2])));
        }

    }

    return cardTrainerToReturn;
}

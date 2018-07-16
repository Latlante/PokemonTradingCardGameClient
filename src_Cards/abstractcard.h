#ifndef ABSTRACTCARD_H
#define ABSTRACTCARD_H

#include <QObject>
#include <QDebug>
#include <QUrl>

class Player;

class AbstractCard : public QObject
{
    Q_OBJECT
public:
	enum Enum_typeOfCard 
	{
		TypeOfCard_Pokemon = 0,
		TypeOfCard_Energy,
        TypeOfCard_Action,
        TypeOfCard_Whatever
	};
    Q_ENUMS(Enum_typeOfCard)
	
	enum Enum_element
	{
        Element_None = 0,
		Element_Dark = 1,
        Element_Dragon = 2,
		Element_Electric = 3,
		Element_Fighting = 4,
		Element_Fire = 5,
        Element_Fairy = 6,
        //Element_Ghost = 7,
		Element_Grass = 8,
        Element_Metal = 9,
		Element_Normal = 10,		
        //Element_Poison = 11,
		Element_Psychic = 12,
        //Element_Rock = 13,
        Element_Water = 14,
        Element_COUNT,
        Element_Whatever = 255
	};

    AbstractCard(unsigned short id, const QString& name);

    virtual ~AbstractCard();

    static void declareQML();
    static QString imageByDefault();
	
    Q_INVOKABLE virtual Enum_typeOfCard type() = 0;
    Q_INVOKABLE virtual QUrl image() = 0;
    virtual AbstractCard* clone() = 0;

    Player* owner();
    void setOwner(Player* play);
    Q_INVOKABLE virtual int id();
    Q_INVOKABLE virtual const QString name();
	void setName(const QString& name);

    Q_INVOKABLE QString elementToString(Enum_element element);

signals:
    void dataChanged();

protected:
    Player* m_owner;
	unsigned short m_id;
	QString m_name;

    AbstractCard();
};

#endif // ABSTRACTCARD_H

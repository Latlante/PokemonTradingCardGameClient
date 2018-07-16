#ifndef CARDACTION_H
#define CARDACTION_H

#include "abstractcard.h"

class AbstractAction;
class CardPokemon;

class CardAction : public AbstractCard
{
public:
    CardAction(unsigned short id, const QString& name, const QString& description, AbstractAction* action);
    CardAction(const CardAction& card);
    ~CardAction();

    static void declareQML();
	
    Q_INVOKABLE AbstractCard::Enum_typeOfCard type() override;
    Q_INVOKABLE QUrl image() override;
    AbstractCard* clone() override;

    Q_INVOKABLE const QString description();
    AbstractAction* action();
	
    void executeAction(CardPokemon* pokemonAttached);

    CardAction& operator =(const CardAction& source);

private:
	QString m_description;
    AbstractAction* m_action;
};

#endif // CARDACTION_H

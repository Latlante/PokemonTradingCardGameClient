#include "cardaction.h"
#include <QUrl>
#include <QtQml/qqml.h>

#include "src_Cards/cardpokemon.h"

CardAction::CardAction(unsigned short id, const QString& name, const QString& description) :
    AbstractCard(id, name),
    m_description(description)
{
	
}

CardAction::CardAction(const CardAction &card) :
    AbstractCard()
{
    *this = card;
}

CardAction::~CardAction()
{
    //delete m_action;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CardAction::declareQML()
{
    qmlRegisterUncreatableType<CardAction>("model", 1, 0, "CardAction", "CardAction error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractCard::Enum_typeOfCard CardAction::type()
{
	return AbstractCard::TypeOfCard_Action;
}

QUrl CardAction::image()
{
    const QString path = "trainer/cards/" + QString::number(id()) + ".png";

    return path;
}

AbstractCard* CardAction::clone()
{
    return new CardAction(*this);
}

const QString CardAction::description()
{
	return m_description;
}

AbstractAction* CardAction::action()
{
    return nullptr;
}

void CardAction::executeAction(CardPokemon *pokemonAttached)
{
}

CardAction& CardAction::operator =(const CardAction& source)
{
    m_id = source.m_id;
    m_name = source.m_name;
    m_description = source.m_description;

    return *this;
}

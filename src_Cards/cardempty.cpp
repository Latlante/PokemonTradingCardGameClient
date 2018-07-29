#include "cardempty.h"
#include <QUrl>
#include <QtQml/qqml.h>

CardEmpty::CardEmpty(QObject *parent) :
    AbstractCard(0, "")
{

}

CardEmpty::CardEmpty(const CardEmpty &card) :
    AbstractCard()
{
    *this = card;
}

CardEmpty::~CardEmpty()
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CardEmpty::declareQML()
{
    qmlRegisterUncreatableType<CardEmpty>("model", 1, 0, "CardEmpty", "CardEmpty error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractCard::Enum_typeOfCard CardEmpty::type()
{
    return AbstractCard::TypeOfCard_Whatever;
}

QUrl CardEmpty::image()
{
    return AbstractCard::imageByDefault();
}

AbstractCard* CardEmpty::clone()
{
    return new CardEmpty(*this);
}

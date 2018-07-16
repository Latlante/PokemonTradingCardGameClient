#include "cardenergy.h"
#include <QUrl>
#include <QtQml/qqml.h>

CardEnergy::CardEnergy(unsigned short id, const QString& name, AbstractCard::Enum_element element, unsigned short quantity) :
    AbstractCard(id, name),
	m_element(element),
	m_quantity(quantity)
{
	
}

CardEnergy::CardEnergy(const CardEnergy &card) :
    AbstractCard()
{
    *this = card;
}

CardEnergy::~CardEnergy()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CardEnergy::declareQML()
{
    qmlRegisterUncreatableType<CardEnergy>("model", 1, 0, "CardEnergy", "CardEnergy error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractCard::Enum_typeOfCard CardEnergy::type()
{
    return AbstractCard::TypeOfCard_Energy;
}

QUrl CardEnergy::image()
{
    const QString path = "energies/cards/" + QString::number(static_cast<int>(element())) + ".png";
    //qDebug() << __PRETTY_FUNCTION__ << "path image:" << path;

    //return QPixmap(path);
    //return QUrl::fromLocalFile(path);
    return path;
}

AbstractCard* CardEnergy::clone()
{
    return new CardEnergy(*this);
}

AbstractCard::Enum_element CardEnergy::element()
{
	return m_element;
}

QString CardEnergy::elementFormatString()
{
    return elementToString(element());
}

void CardEnergy::setElement(AbstractCard::Enum_element element)
{
	m_element = element;
}

unsigned short CardEnergy::quantity()
{
	return m_quantity;
}

void CardEnergy::setQuantity(unsigned short quantity)
{
	m_quantity = quantity;
}

CardEnergy& CardEnergy::operator =(const CardEnergy& source)
{
    m_id = source.m_id;
    m_name = source.m_name;
    m_element = source.m_element;
    m_quantity = source.m_quantity;

    return *this;
}

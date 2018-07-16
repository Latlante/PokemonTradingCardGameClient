#include "abstractpacket.h"

#include <QDebug>
#include "src_Cards/abstractcard.h"

AbstractPacket::AbstractPacket(const QString &namePacket, QList<AbstractCard*> listCards) :
    QAbstractListModel(NULL),
    m_listCards(listCards),
    m_name(namePacket)
{
}

AbstractPacket::~AbstractPacket()
{
	while (0 < m_listCards.count())
	{
		AbstractCard* card = m_listCards.takeFirst();
		delete card;
	}
}
	
/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void AbstractPacket::declareQML()
{
    qmlRegisterUncreatableType<AbstractPacket>("model", 1, 0, "AbstractPacket", "AbstractPacket cannot be created.");
}

QString AbstractPacket::name()
{
    return m_name;
}

bool AbstractPacket::isFull()
{
    bool full = false;

    if ((maxCards() >= 0) && (countCard() >= maxCards()))
        full = true;

    return full;
}

bool AbstractPacket::isEmpty()
{
    return m_listCards.isEmpty();
}

AbstractCard* AbstractPacket::card(int index)
{
    AbstractCard* card = NULL;

    if ((index >= 0) && (index < countCard()))
    {
        card = m_listCards[index];
    }

    return card;
}

/************************************************************
*****			FONCTIONS SLOT PROTEGEES				*****
************************************************************/
void AbstractPacket::updateAllData()
{
    emit dataChanged(index(0, 0), index(rowCount()-1, 0));
}

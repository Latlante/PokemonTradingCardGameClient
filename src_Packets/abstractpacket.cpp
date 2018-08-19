#include "abstractpacket.h"

#include <QDebug>
#include "src_Cards/abstractcard.h"

AbstractPacket::AbstractPacket(const QString &namePacket) :
    QAbstractListModel(nullptr),
    m_name(namePacket)
{
}

AbstractPacket::~AbstractPacket()
{
}
	
/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void AbstractPacket::declareQML()
{
    qmlRegisterUncreatableType<AbstractPacket>("model", 1, 0, "AbstractPacket", "AbstractPacket cannot be created.");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QString AbstractPacket::name() const
{
    return m_name;
}

bool AbstractPacket::isEmpty()
{
    return countCard() == 0;
}

AbstractCard* AbstractPacket::card(int index)
{
    AbstractCard* card = nullptr;

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

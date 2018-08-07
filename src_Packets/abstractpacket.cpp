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
QString AbstractPacket::name()
{
    return m_name;
}

bool AbstractPacket::isEmpty()
{
    return countCard() == 0;
}

/************************************************************
*****			FONCTIONS SLOT PROTEGEES				*****
************************************************************/
void AbstractPacket::updateAllData()
{
    emit dataChanged(index(0, 0), index(rowCount()-1, 0));
}

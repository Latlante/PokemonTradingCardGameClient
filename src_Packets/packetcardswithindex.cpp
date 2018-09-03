#include "packetcardswithindex.h"

#include <QDebug>

PacketCardsWithIndex::PacketCardsWithIndex(const QString &namePacket) :
    PacketGeneric(namePacket)
{

}

PacketCardsWithIndex::~PacketCardsWithIndex()
{
    qDebug() << "PacketCardsWithIndex destroy";
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketCardsWithIndex::declareQML()
{
    qmlRegisterUncreatableType<PacketCardsWithIndex>("model", 1, 0, "PacketCardsWithIndex", "PacketCardsWithIndex cannot be created.");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractPacket::TypeOfPacket PacketCardsWithIndex::type()
{
    return AbstractPacket::Packet_Generic;
}

bool PacketCardsWithIndex::addNewCard(AbstractCard* newCard, int index)
{
    if(PacketGeneric::addNewCard(newCard))
    {
        m_listIndexByCard.insert(newCard, index);
        return true;
    }

    return false;
}

int PacketCardsWithIndex::indexFromCard(AbstractCard* card)
{
    return m_listIndexByCard.value(card, -1);
}

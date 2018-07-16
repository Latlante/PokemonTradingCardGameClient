#include "packettrash.h"

PacketTrash::PacketTrash(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(namePacket, listCards)
{
	
}

PacketTrash::~PacketTrash()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketTrash::declareQML()
{
    qmlRegisterUncreatableType<PacketTrash>("model", 1, 0, "PacketTrash", "PacketTrash error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketTrash::maxCards() const
{
    return MAXCARDS_TRASH;
}

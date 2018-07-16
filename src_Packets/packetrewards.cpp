#include "packetrewards.h"

PacketRewards::PacketRewards(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(namePacket, listCards)
{
	
}

PacketRewards::~PacketRewards()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketRewards::declareQML()
{
    qmlRegisterUncreatableType<PacketRewards>("model", 1, 0, "PacketRewards", "PacketRewards error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketRewards::maxCards() const
{
    return MAXCARDS_REWARD;
}

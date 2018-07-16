#ifndef PACKETREWARDS_H
#define PACKETREWARDS_H

#include "abstractpacketdynamic.h"

class PacketRewards : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketRewards(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~PacketRewards();

    static void declareQML();
	
    int maxCards() const override;

};

#endif // PACKETREWARDS_H

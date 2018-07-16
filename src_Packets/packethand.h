#ifndef PACKETHAND_H
#define PACKETHAND_H

#include "abstractpacketdynamic.h"

class PacketHand : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketHand(const QString& namePacket, QList<AbstractCard *> listCards = QList<AbstractCard*>());
    virtual ~PacketHand();

    static void declareQML();
	
    int maxCards() const override;

    bool isFirstHandOk();
};

#endif // PACKETHAND_H

#ifndef PACKETTRASH_H
#define PACKETTRASH_H

#include "abstractpacketdynamic.h"

class PacketTrash : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketTrash(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~PacketTrash();

    static void declareQML();
	
    int maxCards() const override;

};

#endif // PACKETTRASH_H

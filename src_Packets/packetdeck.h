#ifndef PACKETDECK_H
#define PACKETDECK_H

#include "abstractpacketdynamic.h"

class AbsractCard;

class PacketDeck : public AbstractPacketDynamic
{
    Q_OBJECT

public:
    PacketDeck(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~PacketDeck();

    static void declareQML();
	
    int maxCards() const override;
	void mixCards();
    AbstractCard *drawCard();
    QList<AbstractCard *> drawCards(int count);

};

#endif // PACKETDECK_H

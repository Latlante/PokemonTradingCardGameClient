#ifndef PACKETDYNAMICCUSTOM_H
#define PACKETDYNAMICCUSTOM_H

#include "abstractpacketdynamic.h"

class PacketDynamicCustom : public AbstractPacketDynamic
{
    Q_OBJECT
public:
    explicit PacketDynamicCustom(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());

    static void declareQML();

    void setMaxCards(int max);
    int maxCards() const override;

private:
    int m_maxCards;
};

#endif // PACKETDYNAMICCUSTOM_H

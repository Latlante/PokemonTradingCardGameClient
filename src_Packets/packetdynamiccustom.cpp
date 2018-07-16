#include "packetdynamiccustom.h"

#include "src_Cards/abstractcard.h"

PacketDynamicCustom::PacketDynamicCustom(const QString &namePacket, QList<AbstractCard *> listCards) :
    AbstractPacketDynamic(namePacket, listCards),
    m_maxCards(-1)
{

}
/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketDynamicCustom::declareQML()
{
    qmlRegisterUncreatableType<PacketDynamicCustom>("model", 1, 0, "PacketDynamicCustom", "PacketDynamicCustom error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketDynamicCustom::maxCards() const
{
    return m_maxCards;
}

void PacketDynamicCustom::setMaxCards(int max)
{
    m_maxCards = max;
}

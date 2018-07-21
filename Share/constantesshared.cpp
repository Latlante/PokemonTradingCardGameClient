#include "constantesshared.h"

ConstantesShared::ConstantesShared()
{

}

ConstantesShared::EnumPacket ConstantesShared::EnumPacketFromName(const QString &name)
{
    EnumPacket packet = PACKET_None;

    if(name == "Bench")
        packet = PACKET_Bench;
    else if(name == "Deck")
        packet = PACKET_Deck;
    else if(name == "Fight")
        packet = PACKET_Fight;
    else if(name == "Hand")
        packet = PACKET_Hand;
    else if(name == "Rewards")
        packet = PACKET_Rewards;
    else if(name == "Trash")
        packet = PACKET_Trash;

    return packet;
}

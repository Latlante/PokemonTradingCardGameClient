#ifndef CONSTANTESSHARED_H
#define CONSTANTESSHARED_H

#include <QObject>

class ConstantesShared
{
public:
    enum GamePhase
    {
        PHASE_Identification = 1,
        PHASE_ListOfGameForThePlayer = 2,
        PHASE_GetAllInfoOnInstance = 3,
        PHASE_ListOfAllPlayers = 10,
        PHASE_CreateANewGame = 11,
        PHASE_RemoveAGame = 12,
        PHASE_GetAllInfoOnGame = 20,
        PHASE_SelectCards = 31,
        PHASE_InitReady = 32,
        PHASE_MoveACard = 40,
        PHASE_Attack_Retreat = 41,
        PHASE_SkipTheTurn = 42,
        PHASE_NotifNewGameCreated = 100,
        PHASE_NotifPlayerIsReady,
        PHASE_NotifEndOfTurn,
        PHASE_NotifCardMoved,
        PHASE_NotifDataPokemonChanged,
        PHASE_NotifPokemonSwitched,
        PHASE_NotifEnergyAdded,
        PHASE_NotifEnergyRemoved
    };

    enum EnumPacket
    {
        PACKET_None = 0,
        PACKET_Bench,
        PACKET_Deck,
        PACKET_Fight,
        PACKET_Hand,
        PACKET_Rewards,
        PACKET_Trash
    };

    ConstantesShared();

    static EnumPacket EnumPacketFromName(const QString& name);
};

#endif // CONSTANTESSHARED_H

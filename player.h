#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVariant>

#include "Share/constantesshared.h"

class AbstractCard;
class CardAction;
class CardEnergy;
class CardPokemon;
class AbstractPacket;
class PacketGeneric;
class PacketHiddenCards;
class PacketPokemon;

class Player : public QObject
{
	Q_OBJECT
    Q_PROPERTY(bool initReady READ initReady WRITE setInitReady NOTIFY initReadyChanged)
    Q_PROPERTY(bool canPlay READ canPlay WRITE setCanPlay NOTIFY canPlayChanged)
	
public:
    Player(unsigned int uid, QString name, int countDeck, QObject *parent = nullptr);
	~Player();

    static void declareQML();
	
    Q_INVOKABLE unsigned int uid();
    Q_INVOKABLE const QString name();
    Q_INVOKABLE PacketPokemon* bench();
    Q_INVOKABLE PacketGeneric* deck();
    Q_INVOKABLE PacketPokemon* fight();
    Q_INVOKABLE PacketGeneric* hand();
    Q_INVOKABLE PacketGeneric* rewards();
    Q_INVOKABLE PacketGeneric* trash();
	
    bool isLoser();

    bool initReady();
    void setInitReady(bool ready);

    bool canPlay();
    void setCanPlay(bool status);

    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, int index);
    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, AbstractCard* cardToMove);
    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, int index, AbstractCard* cardToMove);
    AbstractPacket* packetFromEnumPacket(ConstantesShared::EnumPacket ePacket);

signals:
    void endOfTurn();
    void canPlayChanged();
    void initReadyChanged();

private:
    unsigned int m_uid;
	QString m_name;

    PacketPokemon* m_bench;
    PacketGeneric* m_deck;
    PacketPokemon* m_fight;
    PacketGeneric* m_hand;
    PacketGeneric* m_rewards;
    PacketGeneric* m_trash;

    //For the round
    bool m_initReady;
    bool m_canPlay;


};

#endif // PLAYER_H

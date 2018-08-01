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
class PacketDeck;
class PacketRewards;
class PacketHand;
class BenchArea;
class FightArea;
class PacketTrash;

class Player : public QObject
{
	Q_OBJECT
    Q_PROPERTY(bool initReady READ initReady WRITE setInitReady NOTIFY initReadyChanged)
    Q_PROPERTY(bool canPlay READ canPlay WRITE setCanPlay NOTIFY canPlayChanged)
	
public:
    Player(unsigned int uid, QString name, QList<AbstractCard*> listCards, QObject *parent = nullptr);
	~Player();

    static void declareQML();
	
    Q_INVOKABLE unsigned int uid();
    Q_INVOKABLE const QString name();
    Q_INVOKABLE BenchArea* bench();
    Q_INVOKABLE PacketDeck* deck();
    Q_INVOKABLE FightArea* fight();
    Q_INVOKABLE PacketHand* hand();
    Q_INVOKABLE PacketRewards* rewards();
    Q_INVOKABLE PacketTrash* trash();
	
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

    BenchArea* m_bench;
	PacketDeck* m_deck;
    FightArea* m_fight;
    PacketHand* m_hand;
	PacketRewards* m_rewards;
    PacketTrash* m_trash;

    //For the round
    bool m_initReady;
    bool m_canPlay;


};

#endif // PLAYER_H

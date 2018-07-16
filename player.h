#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVariant>

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
	Player(QString name, QList<AbstractCard*> listCards, QObject *parent = NULL);
	~Player();

    static void declareQML();
	
    Q_INVOKABLE const QString name();
    Q_INVOKABLE BenchArea* bench();
    Q_INVOKABLE PacketDeck* deck();
    Q_INVOKABLE FightArea* fight();
    Q_INVOKABLE PacketHand* hand();
    Q_INVOKABLE PacketRewards* rewards();
    Q_INVOKABLE PacketTrash* trash();
	
	void newTurn();
    void turnFinished();
    bool isPlaying();
	void drawOneCard();
    void drawOneReward(AbstractCard *cardReward);
    bool isLoser();

    bool initReady();
    void setInitReady(bool ready);
    Q_INVOKABLE void checkIfInitReady();

    bool canPlay();
    void setCanPlay(bool status);

    bool moveCardFromDeckToHand(AbstractCard* cardDeckToMove = nullptr);
    bool moveCardFromDeckToReward(AbstractCard* cardDeckToMove = nullptr);
    Q_INVOKABLE bool moveCardFromHandToBench(int indexHand, int indexBench);
    bool moveCardFromHandToDeck(AbstractCard* cardHandToMove);
    Q_INVOKABLE bool moveCardFromHandToFight(int indexHand);
    bool moveCardFromHandToTrash(AbstractCard *cardHandToMove);
    bool moveCardFromBenchToFight(CardPokemon *pokemonToMove);
    bool moveCardFromBenchToTrash(int index);
    bool moveCardFromFightToTrash(int index);
    bool moveCardFromRewardToHand(AbstractCard *cardReward);
    bool moveCardFromTrashToHand(AbstractCard *cardTrash);
    bool moveAllCardFromHandToDeck();

    bool swapCardsBetweenBenchAndFight(CardPokemon *pokemonBenchToSwap);


signals:
    void endOfTurn();
    void canPlayChanged();
    void initReadyChanged();

private:
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
    bool m_energyPlayedForThisRound;

    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, int index);
    bool moveCardFromPacketToAnother(AbstractPacket* source, AbstractPacket* destination, AbstractCard* cardToMove);

};

#endif // PLAYER_H

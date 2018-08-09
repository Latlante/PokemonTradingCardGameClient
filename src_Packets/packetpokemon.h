#ifndef PACKETPOKEMON_H
#define PACKETPOKEMON_H

#include "abstractpacket.h"

class CardPokemon;

class PacketPokemon : public AbstractPacket
{
    Q_OBJECT
    Q_PROPERTY(CardPokemon* pokemonFighter READ pokemonFighter NOTIFY pokemonFighterChanged)
public:
    enum Roles
    {
        ROLE_CARD = Qt::UserRole,
        ROLE_ISCARD,
        ROLE_NAME,
        ROLE_IMAGECARD
    };

    explicit PacketPokemon(const QString &namePacket, QList<AbstractCard*> listCards = {});
    virtual ~PacketPokemon() override;

    static void declareQML();

    Q_INVOKABLE CardPokemon* pokemon(int index);
    CardPokemon* pokemonFighter();

    AbstractPacket::TypeOfPacket type() final;

    int countCard() const final;
    bool addNewCard(AbstractCard* newCard = nullptr) final;
    AbstractCard* takeACard(int indexCard = -1) final;
    bool remove(AbstractCard* card = nullptr) final;

    QVariant data(const QModelIndex &index, int role) const final;
    int rowCount(const QModelIndex& = QModelIndex()) const final;

protected:
    QHash<int, QByteArray> roleNames() const final;

signals:
    void pokemonFighterChanged();

private:

};

#endif // PACKETPOKEMON_H

#ifndef PACKETPOKEMON_H
#define PACKETPOKEMON_H

#include "abstractpacket.h"

class PacketPokemon : public AbstractPacket
{
    Q_OBJECT
public:
    enum Roles
    {
        ROLE_CARD = Qt::UserRole,
        ROLE_ISCARD,
        ROLE_NAME,
        ROLE_IMAGECARD
    };

    explicit PacketPokemon(const QString &namePacket, QList<AbstractCard*> listCards);

    static void declareQML();

    AbstractPacket::TypeOfPacket type() final;

    int countCard() const final;
    bool addNewCard(AbstractCard* newCard = nullptr) final;
    AbstractCard* takeACard(int indexCard = -1) final;

    QVariant data(const QModelIndex &index, int role) const final;
    int rowCount(const QModelIndex& = QModelIndex()) const final;

protected:
    QHash<int, QByteArray> roleNames() const final;

private:
    QList<AbstractCard*> m_listCards;
};

#endif // PACKETPOKEMON_H
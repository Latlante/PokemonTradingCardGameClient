#ifndef PACKETHIDDENCARDS_H
#define PACKETHIDDENCARDS_H

#include "abstractpacket.h"

class PacketHiddenCards : public AbstractPacket
{
    Q_OBJECT
public:
    explicit PacketHiddenCards(const QString &namePacket, int quantityOfCards = 0);
    virtual ~PacketHiddenCards() final;

    static void declareQML();

    AbstractPacket::TypeOfPacket type() final;

    int countCard() const final;
    bool addNewCard(AbstractCard* newCard = nullptr) final;
    AbstractCard* takeACard(int indexCard = -1) final;
    bool remove(AbstractCard* card) final;

    QVariant data(const QModelIndex &index, int role) const final;
    int rowCount(const QModelIndex& = QModelIndex()) const final;

protected:
    QHash<int, QByteArray> roleNames() const final;

private:
    int m_quantityOfCards;
};

#endif // PACKETHIDDENCARDS_H

#ifndef PACKETCARDSWITHINDEX_H
#define PACKETCARDSWITHINDEX_H

#include "packetgeneric.h"

class PacketCardsWithIndex : public PacketGeneric
{
    Q_OBJECT
public:
    explicit PacketCardsWithIndex(const QString &namePacket);
    virtual ~PacketCardsWithIndex() override;

    static void declareQML();

    AbstractPacket::TypeOfPacket type() final;

    bool addNewCard(AbstractCard* newCard, int index);

    int indexCard(int index);
    int indexCard(AbstractCard* abCard);
    QList<AbstractCard*> listCards();
    QList<int> listIndexCards();

protected:
    QMap<int /*index in the list*/, int /*index of the card*/> m_listIndexByCard;

private:
    //bool addNewCard(AbstractCard* newCard = nullptr) override;
};

#endif // PACKETCARDSWITHINDEX_H

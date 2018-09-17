#ifndef PACKETGENERIC_H
#define PACKETGENERIC_H

#include "abstractpacket.h"

class PacketGeneric : public AbstractPacket
{
    Q_OBJECT
public:
    enum Roles
    {
        ROLE_CARD = Qt::UserRole,
        ROLE_IMAGECARD
    };

    explicit PacketGeneric(const QString &namePacket, int quantityOfCards = 0);
    virtual ~PacketGeneric() override;

    static void declareQML();

    AbstractPacket::TypeOfPacket type() override;

    int countCard() const final;
    void setCountCard(int count);
    bool addNewCard(AbstractCard* newCard = nullptr) override;
    void insertNewCard(int index, AbstractCard* newCard);
    AbstractCard* takeACard(int indexCard = -1) final;
    bool remove(AbstractCard* card = nullptr) final;

    int indexOf(AbstractCard* card);

    QVariant data(const QModelIndex &index, int role) const final;
    int rowCount(const QModelIndex& = QModelIndex()) const final;

protected:
    QHash<int, QByteArray> roleNames() const final;

private:
};

#endif // PACKETGENERIC_H

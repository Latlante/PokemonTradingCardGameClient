#ifndef ABSTRACTPACKETSTATIC_H
#define ABSTRACTPACKETSTATIC_H

#include "abstractpacket.h"

class AbstractCard;

class AbstractPacketStatic : public AbstractPacket
{
    Q_OBJECT
public:
    enum PacketStaticRole {
        PacStaticRole_Card = Qt::UserRole,
        PacStaticRole_IsCard,
        PacStaticRole_Name,
        PacStaticRole_ImageCard
    };

    explicit AbstractPacketStatic(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());

    //virtual int maxCards() const = 0;
    virtual int countCard() const override;
    virtual bool addNewCard(AbstractCard *newCard) override;
    virtual AbstractCard* takeACard(int indexCard) override;
    virtual bool removeFromPacketWithoutDelete(AbstractCard* card) override;

    virtual QVariant data(const QModelIndex &index, int role) const final;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const final;

protected:
    QHash<int, QByteArray> roleNames() const final;
};

#endif // ABSTRACTPACKETSTATIC_H

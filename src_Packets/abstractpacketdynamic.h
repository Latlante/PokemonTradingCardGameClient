#ifndef ABSTRACTPACKETDYNAMIC_H
#define ABSTRACTPACKETDYNAMIC_H

#include "abstractpacket.h"

class AbstractCard;

class AbstractPacketDynamic : public AbstractPacket
{
    Q_OBJECT
public:
    enum PacketDynamicRole {
        PacDynamicRole_Card = Qt::UserRole,
        PacDynamicRole_Name,
        PacDynamicRole_ImageCard
    };

    explicit AbstractPacketDynamic(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());

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

#endif // ABSTRACTPACKETDYNAMIC_H

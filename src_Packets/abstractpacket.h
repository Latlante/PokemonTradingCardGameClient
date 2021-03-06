#ifndef ABSTRACTPACKET_H
#define ABSTRACTPACKET_H

#include <QAbstractListModel>
#include <QtQml/qqml.h>
#include "common/utils.h"

class AbstractCard;

class AbstractPacket : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int countCard READ countCard NOTIFY countChanged)

public:
    enum TypeOfPacket
    {
        Packet_HiddenCard = 0,
        Packet_Generic,
        Packet_Pokemon,
        Packet_CardsWithIndex
    };

    AbstractPacket(const QString& namePacket);
    virtual ~AbstractPacket();
	
    static void declareQML();

    QString name() const;
    virtual TypeOfPacket type() = 0;
	
    virtual int countCard() const = 0;
    virtual bool addNewCard(AbstractCard* newCard = nullptr) = 0;
    virtual AbstractCard* card(int index);
    virtual AbstractCard*takeACard(int indexCard = -1) = 0;
    virtual bool remove(AbstractCard* card = nullptr) = 0;

    bool isEmpty();

    virtual QVariant data(const QModelIndex &index, int role) const = 0;
    virtual int rowCount(const QModelIndex& = QModelIndex()) const = 0;

signals:
    void countChanged();

protected:
    QVector<AbstractCard*> m_listCards;

protected slots:
    void updateAllData();

private:
    QString m_name;

};

#endif // ABSTRACTPACKET_H

#include "packethiddencards.h"
#include <QDebug>
#include <QtQml/qqml.h>

#include "src_Cards/abstractcard.h"

PacketHiddenCards::PacketHiddenCards(const QString &namePacket, int quantityOfCards) :
    AbstractPacket(namePacket),
    m_quantityOfCards(quantityOfCards)
{

}

PacketHiddenCards::~PacketHiddenCards()
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketHiddenCards::declareQML()
{
    qmlRegisterUncreatableType<PacketHiddenCards>("model", 1, 0, "PacketHiddenCards", "PacketHiddenCards cannot be created.");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractPacket::TypeOfPacket PacketHiddenCards::type()
{
    return AbstractPacket::Packet_HiddenCard;
}

int PacketHiddenCards::countCard() const
{
    return rowCount();
}

bool PacketHiddenCards::addNewCard(AbstractCard* newCard)
{
    Q_UNUSED(newCard)

    m_quantityOfCards += 1;
}

AbstractCard* PacketHiddenCards::takeACard(int indexCard)
{
    Q_UNUSED(indexCard)

    m_quantityOfCards -= 1;
}

QVariant PacketHiddenCards::data(const QModelIndex &index, int role) const
{
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    switch(role)
    {
    case Qt::UserRole:         return AbstractCard::imageByDefault();

    }

    return QVariant();
}

int PacketHiddenCards::rowCount(const QModelIndex&) const
{
    return m_quantityOfCards;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> PacketHiddenCards::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "imageCard";

    return roles;
}
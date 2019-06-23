#include "packetgeneric.h"

#include <QDebug>

#include "src_Cards/abstractcard.h"

PacketGeneric::PacketGeneric(const QString &namePacket, int quantityOfCards) :
    AbstractPacket(namePacket)
{
    for(int i=0;i<quantityOfCards;++i)
        m_listCards.append(nullptr);
}

PacketGeneric::~PacketGeneric()
{
    qDebug() << "PacketPokemon destroy";
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketGeneric::declareQML()
{
    qmlRegisterUncreatableType<PacketGeneric>("model", 1, 0, "PacketGeneric", "PacketGeneric cannot be created.");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractPacket::TypeOfPacket PacketGeneric::type()
{
    return AbstractPacket::Packet_Generic;
}

int PacketGeneric::countCard() const
{
    return rowCount();
}

void PacketGeneric::setCountCard(int count)
{
    if(m_listCards.count() < count)
    {
        for(int i=m_listCards.count();i<count;++i)
            addNewCard();

        emit countChanged();
    }
    else if(m_listCards.count() > count)
    {
        for(int i=m_listCards.count();i>count;--i)
            remove();

        emit countChanged();
    }
}

bool PacketGeneric::addNewCard(AbstractCard* newCard)
{
    /*qDebug() << __PRETTY_FUNCTION__;

    if(newCard != nullptr)
        qDebug() << __PRETTY_FUNCTION__ << newCard->name();*/

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listCards.append(newCard);
    endInsertRows();

    emit countChanged();

    return true;
}

void PacketGeneric::insertNewCard(int index, AbstractCard* newCard)
{
    if(index < rowCount())
    {
        beginInsertRows(QModelIndex(), index, index);
        m_listCards.insert(index, newCard);
        endInsertRows();
    }
    else
    {
        beginInsertRows(QModelIndex(), rowCount(), index);
        m_listCards.resize(index);
        m_listCards.push_back(newCard);
        endInsertRows();
    }

    emit countChanged();
}

AbstractCard* PacketGeneric::takeACard(int indexCard)
{
    AbstractCard *cardToReturn = nullptr;

    if((indexCard >= 0) && (indexCard < countCard()))
    {
        beginRemoveRows(QModelIndex(), indexCard, indexCard);
        cardToReturn = m_listCards.takeAt(indexCard);
        endRemoveRows();

        emit countChanged();
    }

    return cardToReturn;
}

bool PacketGeneric::remove(AbstractCard* card)
{
    bool success = false;

    if(m_listCards.contains(card))
    {
        int indexCard = m_listCards.indexOf(card);

        beginRemoveRows(QModelIndex(), indexCard, indexCard);
        m_listCards.removeOne(card);
        endRemoveRows();

        emit countChanged();
        success = true;
    }

    return success;
}

int PacketGeneric::indexOf(AbstractCard* card)
{
    return m_listCards.indexOf(card);
}

QVariant PacketGeneric::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__ << name() << index << rowCount() << role;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    //Dans la liste
    if(iRow < countCard())
    {
        if(m_listCards[iRow] != nullptr)
        {
            switch(role)
            {
            case PacketGeneric::ROLE_CARD:            return QVariant::fromValue<AbstractCard*>(m_listCards[iRow]);
            case PacketGeneric::ROLE_IMAGECARD:       return m_listCards[iRow]->image();
            }
        }
        else
        {
            switch(role)
            {
            case PacketGeneric::ROLE_CARD:            return QVariant::Invalid;
            case PacketGeneric::ROLE_IMAGECARD:       return AbstractCard::imageByDefault();
            }
        }

    }

    return QVariant::Invalid;
}

int PacketGeneric::rowCount(const QModelIndex&) const
{
    return m_listCards.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> PacketGeneric::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PacketGeneric::ROLE_CARD] = "card";
    roles[PacketGeneric::ROLE_IMAGECARD] = "imageCard";

    return roles;
}

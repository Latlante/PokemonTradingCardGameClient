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
    m_listCards.append(newCard);
    emit countChanged();
}

AbstractCard* PacketGeneric::takeACard(int indexCard)
{
    AbstractCard *cardToReturn = nullptr;

    if((indexCard >= 0) && (indexCard < countCard()))
    {
        cardToReturn = m_listCards.takeAt(indexCard);
        emit countChanged();
    }

    return cardToReturn;
}

bool PacketGeneric::remove(AbstractCard* card)
{
    bool success = false;

    if(m_listCards.contains(card))
    {
        m_listCards.removeOne(card);
        emit countChanged();
        success = true;
    }

    return success;
}

QVariant PacketGeneric::data(const QModelIndex &index, int role) const
{
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
            return QVariant::Invalid;
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

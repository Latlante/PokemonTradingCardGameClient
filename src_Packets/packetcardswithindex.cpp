#include "packetcardswithindex.h"

#include <QDebug>

PacketCardsWithIndex::PacketCardsWithIndex(const QString &namePacket) :
    PacketGeneric(namePacket)
{

}

PacketCardsWithIndex::~PacketCardsWithIndex()
{
    qDebug() << "PacketCardsWithIndex destroy";
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketCardsWithIndex::declareQML()
{
    qmlRegisterUncreatableType<PacketCardsWithIndex>("model", 1, 0, "PacketCardsWithIndex", "PacketCardsWithIndex cannot be created.");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractPacket::TypeOfPacket PacketCardsWithIndex::type()
{
    return AbstractPacket::Packet_Generic;
}

bool PacketCardsWithIndex::addNewCard(AbstractCard* newCard, int index)
{
    if(PacketGeneric::addNewCard(newCard))
    {
        m_listIndexByCard.insert(indexOf(newCard), index);
        return true;
    }

    return false;
}

int PacketCardsWithIndex::indexCard(int index)
{
    return m_listIndexByCard.value(index);
}

int PacketCardsWithIndex::indexCard(AbstractCard *abCard)
{
    return indexCard(indexOf(abCard));
}

QList<AbstractCard*> PacketCardsWithIndex::listCards()
{
    QList<AbstractCard*> listCards;

    for(int i=0;i<countCard();++i)
        listCards.push_back(card(i));

    return listCards;
}

QList<int> PacketCardsWithIndex::listIndexCards()
{
    QList<int> listIndex;
    QList<int> listKeys = m_listIndexByCard.keys();

    foreach(int key, listKeys)
        listIndex.push_back(m_listIndexByCard.value(key));

    return listIndex;
}

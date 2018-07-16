#include "packethand.h"
#include <QDebug>

#include "src_Cards/abstractcard.h"
#include "src_Cards/cardpokemon.h"

PacketHand::PacketHand(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketDynamic(namePacket, listCards)
{
	
}

PacketHand::~PacketHand()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void PacketHand::declareQML()
{
    qmlRegisterUncreatableType<PacketHand>("model", 1, 0, "PacketHand", "PacketHand error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int PacketHand::maxCards() const
{
    return MAXCARDS_HAND;
}

//La main doit contenir au moins un pokémon de base
bool PacketHand::isFirstHandOk()
{
    bool atLeastOneBaseFound = false;
    int index = 0;

    while((index < countCard()) && (atLeastOneBaseFound == false))
    {
        AbstractCard* aCard = card(index);
        if(aCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(aCard);

            if(pokemon->isBase())
                atLeastOneBaseFound = true;
        }

        index++;
    }

    return atLeastOneBaseFound;
}

#include "fightarea.h"

#include <QDebug>
#include "src_Cards/cardpokemon.h"
#include "utils.h"

FightArea::FightArea(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketStatic(namePacket, listCards)
{
    connect(this, &FightArea::countChanged, this, &FightArea::onCountChanged);
}

FightArea::~FightArea()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void FightArea::declareQML()
{
    qmlRegisterUncreatableType<FightArea>("model", 1, 0, "FightArea", "FightArea error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int FightArea::maxCards() const
{
    return MAXCARDS_FIGHT;
}

CardPokemon* FightArea::pokemonFighter()
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    return pokemonFighting(0);
}

CardPokemon* FightArea::pokemonFighting(int index)
{
#ifdef TRACAGE_PRECIS
    qDebug() << __PRETTY_FUNCTION__;
#endif

    CardPokemon* pokemon = NULL;

    AbstractCard* abCard = card(index);

    if(abCard != NULL)
    {
        pokemon = static_cast<CardPokemon*>(abCard);
    }

    return pokemon;
}

/************************************************************
*****			FONCTIONS SLOTS PRIVEES					*****
************************************************************/
void FightArea::onCountChanged()
{
    emit pokemonFighterChanged();
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/


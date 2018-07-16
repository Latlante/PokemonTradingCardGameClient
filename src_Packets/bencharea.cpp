#include "bencharea.h"

#include <QDebug>
#include <QPixmap>
#include <QVariant>
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardpokemon.h"
#include "src_Models/modellistenergies.h"

BenchArea::BenchArea(const QString &namePacket, QList<AbstractCard*> listCards) :
    AbstractPacketStatic(namePacket, listCards)
{
	
}

BenchArea::~BenchArea()
{
	
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void BenchArea::declareQML()
{
    qmlRegisterUncreatableType<BenchArea>("model", 1, 0, "BenchArea", "BenchArea error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int BenchArea::maxCards() const
{
    return MAXCARDS_BENCH;
}

CardPokemon* BenchArea::cardPok(int index)
{
    AbstractCard* aCard = card(index);

    return dynamic_cast<CardPokemon*>(aCard);
}

ModelListEnergies* BenchArea::modelFromCardPokemon(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << index;

    ModelListEnergies* modelToReturn = nullptr;

    if((index >= 0) && (index < countCard()))
    {
        CardPokemon* cardPok = dynamic_cast<CardPokemon*>(m_listCards[index]);

        if(cardPok != nullptr)
            modelToReturn = cardPok->modelListOfEnergies();
    }

    return modelToReturn;
}

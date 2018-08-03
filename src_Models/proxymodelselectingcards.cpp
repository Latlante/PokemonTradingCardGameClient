#include "proxymodelselectingcards.h"
#include <QtQml/qqml.h>

#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Models/modelselectingcards.h"

ProxyModelSelectingCards::ProxyModelSelectingCards(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_filter(SelCardsFilter_AllCards)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ProxyModelSelectingCards::declareQML()
{
    qmlRegisterUncreatableType<ProxyModelSelectingCards>("model", 1, 0, "ProxyModelSelectingCards", "ProxyModelSelectingCards error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
ProxyModelSelectingCards::SelectingCardsFilter ProxyModelSelectingCards::filter() const
{
    return m_filter;
}

void ProxyModelSelectingCards::setFilter(SelectingCardsFilter filter)
{
    if(m_filter != filter)
    {
        m_filter = filter;
        invalidateFilter();
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
bool ProxyModelSelectingCards::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool accept = false;
    AbstractCard* abCard = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), ModelSelectingCards::SelCards_Card).value<AbstractCard*>();

    switch(filter())
    {
    case SelCardsFilter_AllCards:
        accept = true;
        break;

    case SelCardsFilter_CardsPokemonOnly:
        accept = abCard->type() == AbstractCard::TypeOfCard_Pokemon ? true : false;
        break;

    case SelCardsFilter_CardsElementElectricOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Electric ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Electric ? true : false;
        }
        break;

    case SelCardsFilter_CardsElementFightingOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Fighting ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Fighting ? true : false;
        }
        break;

    case SelCardsFilter_CardsElementFireOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Fire ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Fire ? true : false;
        }
        break;

    case SelCardsFilter_CardsElementGrassOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Grass ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Grass ? true : false;
        }
        break;

    case SelCardsFilter_CardsElementNormalOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Normal ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Normal ? true : false;
        }
        break;

    case SelCardsFilter_CardsElementPsyOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Psychic ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Psychic ? true : false;
        }
        break;

    case SelCardsFilter_CardsElementWaterOnly:
        if(abCard->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(abCard);
            accept = pokemon->element() == AbstractCard::Element_Water ? true : false;
        }
        else if(abCard->type() == AbstractCard::TypeOfCard_Energy)
        {
            CardEnergy* energy = static_cast<CardEnergy*>(abCard);
            accept = energy->element() == AbstractCard::Element_Water ? true : false;
        }
        break;

    case SelCardsFilter_CardsEnergiesOnly:
        accept = abCard->type() == AbstractCard::TypeOfCard_Energy ? true : false;
        break;

    case SelCardsFilter_CardsTrainersOnly:
        accept = abCard->type() == AbstractCard::TypeOfCard_Action ? true : false;
        break;

    default:
        accept = false;
        break;
    }

    return accept;
}

bool ProxyModelSelectingCards::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    /*QString nameLeft = sourceModel()->data(source_left, ModelSelectingCards::SelCards_Name).toString();
    QString nameRight = sourceModel()->data(source_right, ModelSelectingCards::SelCards_Name).toString();

    return nameLeft.toLower() < nameRight.toLower();*/
    //return QSortFilterProxyModel::lessThan(source_left, source_right);
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

#include "modelselectingcards.h"
#include <QDebug>
#include <QtQml/qqml.h>
#include "common/database.h"
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Packets/packetdeck.h"
#include "utils.h"

ModelSelectingCards::ModelSelectingCards(QObject *parent) :
    QAbstractListModel(parent),
    m_listCardsSelected(QList<InfoCard*>()),
    m_lastPlayer(false)
{
    initListCards();
}

ModelSelectingCards::~ModelSelectingCards()
{
    cleanListCards();
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelSelectingCards::declareQML()
{
    qmlRegisterUncreatableType<ModelSelectingCards>("model", 1, 0, "ModelSelectingCards", "ModelSelectingCards error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int ModelSelectingCards::maxCards()
{
    return MAXCARDS_DECK;
}

void ModelSelectingCards::applyAFilter(int filter)
{
    m_listCardsFiltered.clear();

    if(filter == static_cast<int>(SelCardsFilter_AllCards))
    {
        m_listCardsFiltered = m_listCardsSelected;
    }
    else if(filter == static_cast<int>(SelCardsFilter_CardsPokemonOnly))
    {
        foreach(InfoCard* info, m_listCardsSelected)
        {
            if(info != nullptr)
            {
                AbstractCard* card = info->card;

                if(card != nullptr)
                {
                    if(card->type() == AbstractCard::TypeOfCard_Pokemon)
                        m_listCardsFiltered.append(info);
                }
            }
        }
    }
    else if((filter == static_cast<int>(SelCardsFilter_CardsElementElectricOnly)) ||
            (filter == static_cast<int>(SelCardsFilter_CardsElementFightingOnly)) ||
            (filter == static_cast<int>(SelCardsFilter_CardsElementFireOnly)) ||
            (filter == static_cast<int>(SelCardsFilter_CardsElementGrassOnly)) ||
            (filter == static_cast<int>(SelCardsFilter_CardsElementNormalOnly)) ||
            (filter == static_cast<int>(SelCardsFilter_CardsElementPsyOnly)) ||
            (filter == static_cast<int>(SelCardsFilter_CardsElementWaterOnly)))
    {
        AbstractCard::Enum_element elementPokemon;

        switch(static_cast<SelectingCardsFilter>(filter))
        {
        case SelCardsFilter_CardsElementElectricOnly:
            elementPokemon = AbstractCard::Element_Electric;
            break;
        case SelCardsFilter_CardsElementFightingOnly:
            elementPokemon = AbstractCard::Element_Fighting;
            break;
        case SelCardsFilter_CardsElementFireOnly:
            elementPokemon = AbstractCard::Element_Fire;
            break;
        case SelCardsFilter_CardsElementGrassOnly:
            elementPokemon = AbstractCard::Element_Grass;
            break;
        case SelCardsFilter_CardsElementNormalOnly:
            elementPokemon = AbstractCard::Element_Normal;
            break;
        case SelCardsFilter_CardsElementPsyOnly:
            elementPokemon = AbstractCard::Element_Psychic;
            break;
        case SelCardsFilter_CardsElementWaterOnly:
            elementPokemon = AbstractCard::Element_Water;
            break;
        default:
            elementPokemon = AbstractCard::Element_Whatever;
        }

        foreach(InfoCard* info, m_listCardsSelected)
        {
            if(info != nullptr)
            {
                AbstractCard* card = info->card;

                if(card != nullptr)
                {
                    if(card->type() == AbstractCard::TypeOfCard_Pokemon)
                    {
                        CardPokemon* pokemon = static_cast<CardPokemon*>(card);

                        if(pokemon->element() == elementPokemon)
                            m_listCardsFiltered.append(info);
                    }
                }
            }
        }
    }
    else if(filter == static_cast<int>(SelCardsFilter_CardsEnergiesOnly))
    {
        foreach(InfoCard* info, m_listCardsSelected)
        {
            if(info != nullptr)
            {
                AbstractCard* card = info->card;

                if(card != nullptr)
                {
                    if(card->type() == AbstractCard::TypeOfCard_Energy)
                        m_listCardsFiltered.append(info);
                }
            }
        }
    }
    else if(filter == static_cast<int>(SelCardsFilter_CardsTrainersOnly))
    {
        foreach(InfoCard* info, m_listCardsSelected)
        {
            if(info != nullptr)
            {
                AbstractCard* card = info->card;

                if(card != nullptr)
                {
                    if(card->type() == AbstractCard::TypeOfCard_Action)
                        m_listCardsFiltered.append(info);
                }
            }
        }
    }
}

QList<InfoCard*> ModelSelectingCards::listCardsSelected()
{
    return m_listCardsSelected;
}

QString ModelSelectingCards::name()
{
    return m_name;
}

void ModelSelectingCards::setName(const QString &name)
{
    if(m_name != name)
    {
        m_name = name;
        emit nameChanged();
    }
}

bool ModelSelectingCards::isLastPlayer()
{
    return m_lastPlayer;
}

void ModelSelectingCards::setLastPlayer(bool lastPlayer)
{
    if(m_lastPlayer != lastPlayer)
    {
        m_lastPlayer = lastPlayer;
        emit lastPlayerChanged();
    }
}

void ModelSelectingCards::changeQuantityCard(int id, int quantity)
{
    if((id >= 0) && (id < m_listCardsSelected.count()))
    {
        InfoCard* info = m_listCardsSelected[id];
        int delta = quantity - info->quantity;

        if((info->quantity != quantity) &&
            (quantity >= 0) &&
            (canAcceptXNewCards(delta) == true))
        {
            info->quantity = quantity;
            m_listCardsSelected.replace(id, info);

            emit countTotalQuantityChanged();
            emit dataChanged(index(id, 0), index(id, 0));
        }
    }
}

QVariant ModelSelectingCards::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__ << index << role;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    switch(role)
    {
    case SelCards_Card:         return QVariant::fromValue<AbstractCard*>(m_listCardsFiltered[iRow]->card);
    case SelCards_Name:         return m_listCardsFiltered[iRow]->card->name();
    case SelCards_ImageCard:    return m_listCardsFiltered[iRow]->card->image();
    case SelCards_Quantity:     return m_listCardsFiltered[iRow]->quantity;
    }

    return QVariant();
}

int ModelSelectingCards::rowCount(const QModelIndex &) const
{
    return m_listCardsFiltered.count();
}

int ModelSelectingCards::rowCountById(int id) const
{
    return m_listCardsSelected[id]->quantity;
}

void ModelSelectingCards::clear()
{
    for(int i=0;i<m_listCardsSelected.count();++i)
    {
        InfoCard* info = m_listCardsSelected[i];
        info->quantity = 0;
        m_listCardsSelected[i] = info;
    }

    QVector<int> listRole = QVector<int>() << SelCards_Quantity;
    emit dataChanged(index(0, 0), index(rowCount()-1, 0), listRole);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelSelectingCards::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SelCards_Card] = "card";
    roles[SelCards_Name] = "name";
    roles[SelCards_ImageCard] = "imageCard";
    roles[SelCards_Quantity] = "quantity";

    return roles;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void ModelSelectingCards::initListCards()
{
    cleanListCards();

    Database db;

    //Pokemon
    QList<int> listId = db.listIdAllCardsPokemon();
    listId += db.listIdAllCardsEnergies();
    listId += db.listIdAllCardsTrainers();

    qDebug() << __PRETTY_FUNCTION__ << "listCards:" << listId;

    foreach(int id, listId)
    {
        AbstractCard* card = db.cardById(id);

        if(card != NULL)
        {
            InfoCard* info = new InfoCard;
            info->card = card;
            info->quantity = 0;

            m_listCardsSelected.append(info);
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_listCardsFiltered.append(info);
            endInsertRows();
        }
    }
}

void ModelSelectingCards::cleanListCards()
{
    while(m_listCardsSelected.count() > 0)
    {
        beginRemoveRows(QModelIndex(), rowCount()-1, rowCount());
        InfoCard* info = m_listCardsSelected.takeFirst();
        delete info->card;
        info->card = nullptr;
        delete info;
        info = nullptr;
        endRemoveRows();
    }

    m_listCardsFiltered.clear();
}

int ModelSelectingCards::countTotalQuantity()
{
    int count = 0;

    foreach(InfoCard* info, m_listCardsSelected)
        count += info->quantity;

    return count;
}

bool ModelSelectingCards::canAcceptXNewCards(int quantity)
{
    return (countTotalQuantity() + quantity) <= maxCards();
}

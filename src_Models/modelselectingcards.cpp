#include "modelselectingcards.h"
#include <QDebug>
#include <QtQml/qqml.h>
#include "common/database.h"
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "src_Packets/packetdeck.h"
#include "common/utils.h"

ModelSelectingCards::ModelSelectingCards(QObject *parent) :
    QAbstractListModel(parent),
    m_listCardsSelected(QList<InfoCard>())
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
        foreach(InfoCard info, m_listCardsSelected)
        {
            AbstractCard* card = info.card;

            if(card != nullptr)
            {
                if(card->type() == AbstractCard::TypeOfCard_Pokemon)
                    m_listCardsFiltered.append(info);
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

        foreach(InfoCard info, m_listCardsSelected)
        {
            AbstractCard* card = info.card;

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
    else if(filter == static_cast<int>(SelCardsFilter_CardsEnergiesOnly))
    {
        foreach(InfoCard info, m_listCardsSelected)
        {
            AbstractCard* card = info.card;

            if(card != nullptr)
            {
                if(card->type() == AbstractCard::TypeOfCard_Energy)
                    m_listCardsFiltered.append(info);
            }
        }
    }
    else if(filter == static_cast<int>(SelCardsFilter_CardsTrainersOnly))
    {
        foreach(InfoCard info, m_listCardsSelected)
        {
            AbstractCard* card = info.card;

            if(card != nullptr)
            {
                if(card->type() == AbstractCard::TypeOfCard_Action)
                    m_listCardsFiltered.append(info);
            }
        }
    }
}

QList<InfoCard> ModelSelectingCards::listCardsSelected()
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

int ModelSelectingCards::quantity(int id)
{
    int indexListSelected = indexListSelectedFromIdCard(id);
    return m_listCardsSelected[indexListSelected].quantity;
}

void ModelSelectingCards::setQuantity(int id, int quantity)
{
    //List selected
    int indexListSelected = indexListSelectedFromIdCard(id);

    if(indexListSelected != -1)
    {
        int oldQuantity = m_listCardsSelected[indexListSelected].quantity;

        //try
        m_listCardsSelected[indexListSelected].quantity = quantity;

        if(countTotalQuantity() > maxCards())
        {
            //error
            m_listCardsSelected[indexListSelected].quantity = oldQuantity;
        }
        else
        {
            //ok
            emit countTotalQuantityChanged();
        }
    }

    //List filtered
    int indexListFiltered = indexListFilteredFromIdCard(id);

    if(indexListFiltered != -1)
    {
        int oldQuantity = m_listCardsSelected[indexListFiltered].quantity;

        //try
        m_listCardsSelected[indexListFiltered].quantity = quantity;

        if(countTotalQuantity() > maxCards())
        {
            //error
            m_listCardsSelected[indexListFiltered].quantity = oldQuantity;
        }
        else
        {
            //ok
            emit dataChanged(index(indexListFiltered, 0), index(indexListFiltered, 0));
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
    case SelCards_Card:         return QVariant::fromValue<AbstractCard*>(m_listCardsFiltered[iRow].card);
    case SelCards_Name:         return m_listCardsFiltered[iRow].card->name();
    case SelCards_ImageCard:    return m_listCardsFiltered[iRow].card->image();
    case SelCards_Quantity:     return m_listCardsFiltered[iRow].quantity;
    }

    return QVariant();
}

bool ModelSelectingCards::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return false;
    }

    switch(role)
    {
    case SelCards_Quantity:
        if((m_listCardsFiltered[iRow].card != nullptr) && (value.type() == QVariant::Int))
        {
            setQuantity(m_listCardsFiltered[iRow].card->id(), value.toInt());
            return true;
        }
        break;
    }

    return false;
}

int ModelSelectingCards::rowCount(const QModelIndex &) const
{
    return m_listCardsFiltered.count();
}

int ModelSelectingCards::rowCountById(int id)
{
    int quantity = -1;
    int indexCard = indexListSelectedFromIdCard(id);

    if(indexCard != -1)
        quantity = m_listCardsSelected[indexCard].quantity;

    return quantity;
}

void ModelSelectingCards::clear()
{
    for(int i=0;i<m_listCardsSelected.count();++i)
    {
        InfoCard info = m_listCardsSelected[i];
        info.quantity = 0;
        m_listCardsSelected.replace(i, info);
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
            InfoCard info;
            info.card = card;
            info.quantity = 0;

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
        InfoCard info = m_listCardsSelected.takeFirst();
        delete info.card;
        info.card = nullptr;
        endRemoveRows();
    }

    m_listCardsFiltered.clear();
}

int ModelSelectingCards::countTotalQuantity()
{
    int count = 0;

    foreach(InfoCard info, m_listCardsSelected)
        count += info.quantity;

    return count;
}

int ModelSelectingCards::indexListSelectedFromIdCard(int id)
{
    int indexInList = -1;
    int indexLoop = 0;

    while((indexLoop < m_listCardsSelected.count()) && (indexInList == -1))
    {
        AbstractCard* card = m_listCardsSelected[indexLoop].card;

        if(card != nullptr)
        {
            if(card->id() == id)
                indexInList = indexLoop;
        }

        indexLoop++;
    }

    return indexInList;
}

int ModelSelectingCards::indexListFilteredFromIdCard(int id)
{
    int indexInList = -1;
    int indexLoop = 0;

    while((indexLoop < m_listCardsFiltered.count()) && (indexInList == -1))
    {
        AbstractCard* card = m_listCardsFiltered[indexLoop].card;

        if(card != nullptr)
        {
            if(card->id() == id)
                indexInList = indexLoop;
        }

        indexLoop++;
    }

    return indexInList;
}

#include "proxymodelselectingcards.h"
#include <QtQml/qqml.h>

#include "src_Cards/abstractcard.h"
#include "src_Models/modelselectingcards.h"

ProxyModelSelectingCards::ProxyModelSelectingCards(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ProxyModelSelectingCards::declareQML()
{
    qmlRegisterUncreatableType<ProxyModelSelectingCards>("model", 1, 0, "ProxyModelSelectingCards", "ProxyModelSelectingCards error");
}

bool ProxyModelSelectingCards::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex indexCard = sourceModel()->index(source_row, 0, source_parent);
    AbstractCard* abCard = sourceModel()->data(indexCard, ModelSelectingCards::SelCards_Card).value<AbstractCard*>();

    //return abCard->type() == AbstractCard::TypeOfCard_Pokemon;
    return true;
}

bool ProxyModelSelectingCards::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    /*QString nameLeft = sourceModel()->data(source_left, ModelSelectingCards::SelCards_Name).toString();
    QString nameRight = sourceModel()->data(source_right, ModelSelectingCards::SelCards_Name).toString();

    return nameLeft.toLower() < nameRight.toLower();*/
    //return QSortFilterProxyModel::lessThan(source_left, source_right);
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

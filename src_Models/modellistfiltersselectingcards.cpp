#include "modellistfiltersselectingcards.h"
#include <QDebug>
#include <QtQml/qqml.h>

ModelListFiltersSelectingCards::ModelListFiltersSelectingCards()
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelListFiltersSelectingCards::declareQML()
{
    qmlRegisterUncreatableType<ModelListFiltersSelectingCards>("model", 1, 0, "ModelListFiltersSelectingCards", "ModelListFiltersSelectingCards error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ModelListFiltersSelectingCards::addFilter(const QString &name, int filter)
{
    m_listInfo.append({ name, filter });
}

QVariant ModelListFiltersSelectingCards::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    switch(role)
    {
    case ROLE_NAME:     return m_listInfo[iRow].name;
    case ROLE_FILTER:   return m_listInfo[iRow].filter;
    }

    return QVariant();
}

int ModelListFiltersSelectingCards::rowCount(const QModelIndex &) const
{
    return m_listInfo.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelListFiltersSelectingCards::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ROLE_NAME] = "name";
    roles[ROLE_FILTER] = "filter";

    return roles;
}
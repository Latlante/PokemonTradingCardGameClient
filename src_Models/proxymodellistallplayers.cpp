#include "proxymodellistallplayers.h"
#include <QtQml/qqml.h>

#include "modellistallplayers.h"

ProxyModelListAllPlayers::ProxyModelListAllPlayers(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ProxyModelListAllPlayers::declareQML()
{
    qmlRegisterUncreatableType<ProxyModelListAllPlayers>("model", 1, 0, "ProxyModelListAllPlayers", "ProxyModelListAllPlayers error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool ProxyModelListAllPlayers::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    bool filterAccepts = true;

    ModelListAllPlayers *modelAllPlayers = dynamic_cast<ModelListAllPlayers*>(sourceModel());
    if(modelAllPlayers != nullptr)
    {
        QString currentPlayer = modelAllPlayers->currentPlayer();
        if(currentPlayer == modelAllPlayers->data(modelAllPlayers->index(source_row, 0), Qt::DisplayRole))
            filterAccepts = false;
    }

    return filterAccepts;
}

int ProxyModelListAllPlayers::mapIndexToSource(int proxyIndex)
{
    return QSortFilterProxyModel::mapToSource(index(proxyIndex, 0, QModelIndex())).row();
}

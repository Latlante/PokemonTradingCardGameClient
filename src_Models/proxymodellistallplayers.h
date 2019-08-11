#ifndef PROXYMODELLISTALLPLAYERS_H
#define PROXYMODELLISTALLPLAYERS_H

#include <QSortFilterProxyModel>

class ProxyModelListAllPlayers : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProxyModelListAllPlayers(QObject *parent = nullptr);

    static void declareQML();

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    Q_INVOKABLE int mapIndexToSource(int proxyIndex);

signals:

private:
};

#endif // PROXYMODELLISTALLPLAYERS_H

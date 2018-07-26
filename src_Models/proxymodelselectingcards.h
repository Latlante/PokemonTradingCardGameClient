#ifndef PROXYMODELSELECTINGCARDS_H
#define PROXYMODELSELECTINGCARDS_H

#include <QSortFilterProxyModel>

class ProxyModelSelectingCards : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProxyModelSelectingCards(QObject *parent = nullptr);

    static void declareQML();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

signals:

public slots:
};

#endif // PROXYMODELSELECTINGCARDS_H

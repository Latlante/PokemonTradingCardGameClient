#ifndef PROXYMODELSELECTINGCARDS_H
#define PROXYMODELSELECTINGCARDS_H

#include <QSortFilterProxyModel>

class ProxyModelSelectingCards : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum SelectingCardsFilter
    {
        SelCardsFilter_AllCards = 0,
        SelCardsFilter_CardsPokemonOnly,
        SelCardsFilter_CardsElementElectricOnly,
        SelCardsFilter_CardsElementFightingOnly,
        SelCardsFilter_CardsElementFireOnly,
        SelCardsFilter_CardsElementGrassOnly,
        SelCardsFilter_CardsElementNormalOnly,
        SelCardsFilter_CardsElementPsyOnly,
        SelCardsFilter_CardsElementWaterOnly,
        SelCardsFilter_CardsEnergiesOnly,
        SelCardsFilter_CardsTrainersOnly
    };

    explicit ProxyModelSelectingCards(QObject *parent = nullptr);

    static void declareQML();

    SelectingCardsFilter filter() const;
    void setFilter(SelectingCardsFilter filter);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

signals:

private:
    SelectingCardsFilter m_filter;
};

#endif // PROXYMODELSELECTINGCARDS_H

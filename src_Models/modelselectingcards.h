#ifndef MODELSELECTINGCARDS_H
#define MODELSELECTINGCARDS_H

#include <QAbstractListModel>
#include <QVariant>

class AbstractCard;
class CardAction;
class CardEnergy;
class CardPokemon;

struct InfoCard
{
    AbstractCard* card;
    unsigned short quantity;
};

class ModelSelectingCards : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(QList<InfoCard*> listCardsSelected READ listCardsSelected NOTIFY listCardsSelectedChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool lastPlayer READ isLastPlayer NOTIFY lastPlayerChanged)
    Q_PROPERTY(int countTotalQuantity READ countTotalQuantity NOTIFY countTotalQuantityChanged)

public:
    enum SelectingCardsRole
    {
        SelCards_Card = Qt::UserRole,
        SelCards_Name,
        SelCards_ImageCard,
        SelCards_Quantity
    };

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

    explicit ModelSelectingCards(QObject *parent = nullptr);
    ~ModelSelectingCards();

    static void declareQML();
    Q_INVOKABLE int maxCards();
    Q_INVOKABLE void applyAFilter(int filter);

    QList<InfoCard *> listCardsSelected();
    QString name();
    void setName(const QString& name);
    bool isLastPlayer();
    void setLastPlayer(bool lastPlayer);

    void changeQuantityCard(int id, int quantity);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;
    int rowCountById(int id) const;

    void clear();

signals:
    void listCardsSelectedChanged();
    void nameChanged();
    void lastPlayerChanged();
    void countTotalQuantityChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QString m_name;
    QList<InfoCard*> m_listCardsSelected;
    QList<InfoCard*> m_listCardsFiltered;
    bool m_lastPlayer;
    //QHash<AbstractCard*, unsigned short> m_listCardsSelected;

    void initListCards();
    void cleanListCards();
    int countTotalQuantity();
    bool canAcceptXNewCards(int quantity);
};

#endif // MODELSELECTINGCARDS_H

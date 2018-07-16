#ifndef MODELPOPUPSELECTENERGYINPOKEMON_H
#define MODELPOPUPSELECTENERGYINPOKEMON_H

#include <QAbstractListModel>
#include "src_Cards/abstractcard.h"

class CardEnergy;
class CardPokemon;

class ModelPopupSelectEnergyInPokemon : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(unsigned short numberOfEnergiesSelected READ numberOfEnergiesSelected NOTIFY numberOfEnergiesSelectedChanged)
    Q_PROPERTY(bool isMaximumEnergiesSelected READ isMaximumEnergiesSelected NOTIFY numberOfEnergiesSelectedChanged)

public:
    struct SelectionCards
    {
        CardEnergy* card;
        bool selected;
    };

    enum SelectCardsRoles
    {
        SelectCardsRole_ImageCard = Qt::UserRole,
        SelectCardsRole_Selected
    };

    ModelPopupSelectEnergyInPokemon(QObject *parent = nullptr);

    static void declareQML();

    void setElementFilter(AbstractCard::Enum_element element);

    void addListEnergyFromPokemon(CardPokemon* pokemon);

    Q_INVOKABLE unsigned short numberOfEnergiesToSelect();
    void setNumberOfEnergiesToSelect(unsigned short numberToSelect);
    unsigned short numberOfEnergiesSelected();
    bool isMaximumEnergiesSelected();

    QList<AbstractCard *> listIndexEnergiesSelected();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;

signals:
    void numberOfEnergiesSelectedChanged();
    void finished();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<SelectionCards> m_listEnergies;
    unsigned short m_numberOfEnergiesToSelect;
    AbstractCard::Enum_element m_elementFilter;

    void cleanPacket();
};

#endif // MODELPOPUPSELECTENERGYINPOKEMON_H

#ifndef MODELLISTFILTERSSELECTINGCARDS_H
#define MODELLISTFILTERSSELECTINGCARDS_H

#include <QAbstractListModel>
#include <QVariant>

class ModelListFiltersSelectingCards : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList listInfo READ listInfo NOTIFY listInfoChanged)
public:
    enum FilterRole
    {
        ROLE_NAME = Qt::UserRole,
        ROLE_FILTER
    };
    Q_ENUMS(FilterRole)

    ModelListFiltersSelectingCards();

    static void declareQML();

    void addFilter(const QString &name, int filter);
    QStringList listInfo();
    Q_INVOKABLE int filter(int index);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

signals:
    void listInfoChanged();

private:
    struct InfoFilter
    {
        QString name;
        int filter;
    };
    QList<InfoFilter> m_listInfo;
};

#endif // MODELLISTFILTERSSELECTINGCARDS_H

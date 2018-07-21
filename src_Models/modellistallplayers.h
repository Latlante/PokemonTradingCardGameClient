#ifndef MODELLISTALLPLAYERS_H
#define MODELLISTALLPLAYERS_H

#include <QAbstractListModel>
#include <QVariant>

class ModelListAllPlayers : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModelListAllPlayers(QObject *parent = nullptr);

    static void declareQML();

    void addNewPlayer(const QString& name);
    void addListNewPlayers(const QStringList& listPlayers);
    void removeOnePlayer(const QString& name);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &parent) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList m_listOfPlayers;
};

#endif // MODELLISTALLPLAYERS_H

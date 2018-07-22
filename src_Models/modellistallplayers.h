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

    QString namePlayerFromId(int id) const;

    void addNewPlayer(int idPlayer, const QString& name);
    void addListNewPlayers(const QStringList& listPlayers);
    void removeOnePlayer(const QString& name);
    void clear();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    //virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList m_listOfPlayers;
};

#endif // MODELLISTALLPLAYERS_H

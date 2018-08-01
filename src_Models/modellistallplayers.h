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

    QString namePlayerFromId(unsigned int uid) const;

    void addNewPlayer(unsigned int idPlayer, const QString& name);
    void removeOnePlayer(int index);
    void clear();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    //virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct InfoPlayer
    {
        unsigned int uid;
        QString name;
    };
    QList<InfoPlayer> m_listOfPlayers;
};

#endif // MODELLISTALLPLAYERS_H

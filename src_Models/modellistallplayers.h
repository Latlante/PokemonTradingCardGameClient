#ifndef MODELLISTALLPLAYERS_H
#define MODELLISTALLPLAYERS_H

#include <QAbstractListModel>
#include <QVariant>

class ModelListAllPlayers : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)

public:
    explicit ModelListAllPlayers(QObject *parent = nullptr);

    static void declareQML();

    QString namePlayerFromId(unsigned int uid) const;
    unsigned int uidFromNamePlayer(const QString &name);
    QString currentPlayer() const;
    void setCurrentPlayer(const QString& currentPlayer);

    void addNewPlayer(unsigned int idPlayer, const QString& name);
    void removeOnePlayer(int index);
    void clear();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    //virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

signals:
    void currentPlayerChanged();

private:
    struct InfoPlayer
    {
        unsigned int uid;
        QString name;
    };
    QList<InfoPlayer> m_listOfPlayers;
    QString m_currentPlayer;
};

#endif // MODELLISTALLPLAYERS_H

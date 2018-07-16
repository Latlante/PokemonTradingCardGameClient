#ifndef LISTPLAYERS_H
#define LISTPLAYERS_H

#include <QAbstractListModel>
#include <QVariant>

class ListPlayers : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList listOfPlayers READ listOfPlayers NOTIFY listOfPlayersChanged)

public:
    explicit ListPlayers(QObject *parent = nullptr);

    static void declareQML();

    QStringList listOfPlayers();


    Q_INVOKABLE void addNewPlayer();
    Q_INVOKABLE void removeOnePlayer();
    Q_INVOKABLE void setNumberOfPlayers(unsigned int number);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;

signals:
    void listOfPlayersChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList m_listOfPlayers;
};

#endif // LISTPLAYERS_H

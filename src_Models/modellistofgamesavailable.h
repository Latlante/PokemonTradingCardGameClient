#ifndef MODELLISTOFGAMESAVAILABLE_H
#define MODELLISTOFGAMESAVAILABLE_H

#include <QAbstractListModel>
#include <QMap>
#include <QVariant>

class ModelListOfGamesAvailable : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum GameRole
    {
        ROLE_ID_GAME = Qt::UserRole,
        ROLE_NAME_GAME,
        ROLE_NAME_OPPONENT
    };

    explicit ModelListOfGamesAvailable(QObject *parent = nullptr);

    static void declareQML();

    int id(int index);
    QString name(int index) const;
    QString opponent(int index) const;

    void addNewGame(int idGame, const QString& nameGame, const QString& opponent);
    void removeGame(int index);
    int count();
    void clear();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

signals:
    void countChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct GameInfo
    {
        int id;
        QString name;
        QString opponent;
    };
    QList<GameInfo> m_listGames;

};

#endif // MODELLISTOFGAMESAVAILABLE_H

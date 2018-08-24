#ifndef MODELLISTTESTANIMATIONS_H
#define MODELLISTTESTANIMATIONS_H

#include <QAbstractListModel>
#include <QVariant>

class ModelListTestAnimations : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleInfo
    {
        ROLE_NAME = Qt::UserRole,
        ROLE_LINK
    };

    explicit ModelListTestAnimations(QObject *parent = nullptr);

    static void declareQML();

    void addAnimation(const QString &name, const QString &link);
    void removeAnimation(int index);
    void clear();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct InfoAnimation
    {
        QString name;
        QString link;
    };
    QList<InfoAnimation> m_listInfoAnimations;
};

#endif // MODELLISTTESTANIMATIONS_H

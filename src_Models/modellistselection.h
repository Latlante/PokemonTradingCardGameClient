#ifndef MODELLISTSELECTION_H
#define MODELLISTSELECTION_H

#include <QAbstractListModel>

class ModelListSelection : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString nameSelection READ nameSelection WRITE setNameSelection NOTIFY nameSelectionChanged)
public:
    explicit ModelListSelection(QObject *parent = nullptr);

    static void declareQML();

    QString nameSelection();
    void setNameSelection(const QString& name);

    void addElement(const QString& element);
    void removeElement(int index);
    Q_INVOKABLE QString element(int index);

    void clear();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

signals:
    void nameSelectionChanged();

private:
    QString m_nameSelection;
    QStringList m_listSelection;
};

#endif // MODELLISTSELECTION_H

#include "modellistallplayers.h"
#include <QtQml/qqml.h>

ModelListAllPlayers::ModelListAllPlayers(QObject *parent) :
    QAbstractListModel(parent)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelListAllPlayers::declareQML()
{
    qmlRegisterUncreatableType<ModelListAllPlayers>("model", 1, 0, "ModelListAllPlayers", "ModelListAllPlayers error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ModelListAllPlayers::addNewPlayer(const QString &name)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listOfPlayers.append(name);
    endInsertRows();
}

void ModelListAllPlayers::addListNewPlayers(const QStringList &listPlayers)
{
    foreach(QString name, listPlayers)
        addNewPlayer(name);
}

void ModelListAllPlayers::removeOnePlayer(const QString &name)
{
    int index = m_listOfPlayers.indexOf(name);

    if(index >= 0)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listOfPlayers.removeAt(index);
        endRemoveRows();
    }
}

QVariant ModelListAllPlayers::data(const QModelIndex &index, int role) const
{
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad column num : " << iRow;
        return QVariant();
    }

    if (Qt::DisplayRole == role)
    {
        return m_listOfPlayers[iRow];
    }

    return QVariant();
}

int ModelListAllPlayers::rowCount(const QModelIndex &) const
{
    return m_listOfPlayers.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelListAllPlayers::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";

    return roles;
}

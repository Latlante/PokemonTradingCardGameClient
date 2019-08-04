#include "modellistallplayers.h"
#include <QDebug>
#include <QtQml/qqml.h>

ModelListAllPlayers::ModelListAllPlayers(QObject *parent) :
    QAbstractListModel(parent),
    m_listOfPlayers(),
    m_currentPlayer("")
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
QString ModelListAllPlayers::namePlayerFromId(unsigned int uid) const
{
    QString namePlayer = "";
    int indexLoop = 0;

    while((indexLoop < m_listOfPlayers.count()) && (namePlayer == ""))
    {
        if(m_listOfPlayers[indexLoop].uid == uid)
            namePlayer = m_listOfPlayers[indexLoop].name;

        indexLoop++;
    }

    return namePlayer;
}

unsigned int ModelListAllPlayers::uidFromNamePlayer(const QString &name)
{
    unsigned int uidPlayer = 0;
    int indexLoop = 0;

    while((indexLoop < m_listOfPlayers.count()) && (uidPlayer == 0))
    {
        if(m_listOfPlayers[indexLoop].name == name)
            uidPlayer = m_listOfPlayers[indexLoop].uid;

        indexLoop++;
    }

    return uidPlayer;
}

QString ModelListAllPlayers::currentPlayer() const
{
    return m_currentPlayer;
}

void ModelListAllPlayers::setCurrentPlayer(const QString &currentPlayer)
{
    if(m_currentPlayer != currentPlayer)
    {
        m_currentPlayer = currentPlayer;
        emit currentPlayerChanged();
    }
}

void ModelListAllPlayers::addNewPlayer(unsigned int idPlayer, const QString &name)
{
    qDebug() << __PRETTY_FUNCTION__ << idPlayer << name;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listOfPlayers.append({ idPlayer, name });
    endInsertRows();
}

void ModelListAllPlayers::removeOnePlayer(int index)
{
    if((index >= 0) && (index < m_listOfPlayers.count()))
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listOfPlayers.removeAt(index);
        endRemoveRows();
    }
}

void ModelListAllPlayers::clear()
{
    m_listOfPlayers.clear();
}

QVariant ModelListAllPlayers::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    if (Qt::DisplayRole == role)
    {
        return m_listOfPlayers[iRow].name;
    }

    return QVariant();
}

/*Qt::ItemFlags ModelListAllPlayers::flags(const QModelIndex &index) const
{
    Qt::ItemFlags currentFlags = QAbstractListModel::flags(index);

    currentFlags |= Qt::ItemIsSelectable;

    return currentFlags;
}*/

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

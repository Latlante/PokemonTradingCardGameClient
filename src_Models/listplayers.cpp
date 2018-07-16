#include "listplayers.h"
#include <QDebug>
#include <QtQml/qqml.h>

ListPlayers::ListPlayers(QObject *parent) : QAbstractListModel(parent)
{
    //addNewPlayer();
    //addNewPlayer();
    setNumberOfPlayers(2);

    setData(index(0, 0), "Corentin", Qt::DisplayRole);
    setData(index(1, 0), "Alice", Qt::DisplayRole);
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ListPlayers::declareQML()
{
    qmlRegisterUncreatableType<ListPlayers>("model", 1, 0, "ListPlayers", "BenchArea error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QStringList ListPlayers::listOfPlayers()
{
    return m_listOfPlayers;
}

void ListPlayers::addNewPlayer()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listOfPlayers.append("Player " + QString::number(m_listOfPlayers.count()+1));
    endInsertRows();
}

void ListPlayers::removeOnePlayer()
{
    beginRemoveRows(QModelIndex(), rowCount()-1, rowCount());
    m_listOfPlayers.removeLast();
    endRemoveRows();
}

void ListPlayers::setNumberOfPlayers(unsigned int number)
{
    const int currentCount = rowCount();
    //Ajout de joueurs
    if(number > currentCount)
    {
        int delta = number - currentCount;

        for(int i=0;i<delta;++i)
        {
            //m_listOfPlayers.append("Player " + QString::number(currentCount+delta));
            addNewPlayer();
        }
    }
    else if(number < currentCount)
    {
        int delta = currentCount - number;

        for(int i=0;i<delta;++i)
        {
            //m_listOfPlayers.append("Player " + QString::number(currentCount+delta));
            removeOnePlayer();
        }
    }
}

QVariant ListPlayers::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__ << index << role;
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

bool ListPlayers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!hasIndex(index.row(), index.column(), index.parent()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << index.row();
        return false;
    }

    if (Qt::DisplayRole == role)
    {
        m_listOfPlayers[index.row()] = value.toString();

        emit dataChanged(index, index);
    }

    return true;
}

int ListPlayers::rowCount(const QModelIndex &) const
{
    return m_listOfPlayers.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ListPlayers::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";

    return roles;
}

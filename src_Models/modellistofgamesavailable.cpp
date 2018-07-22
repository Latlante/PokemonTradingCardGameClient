#include "modellistofgamesavailable.h"
#include <QDebug>
#include <QtQml/qqml.h>

ModelListOfGamesAvailable::ModelListOfGamesAvailable(QObject *parent) :
    QAbstractListModel(parent)
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelListOfGamesAvailable::declareQML()
{
    qmlRegisterUncreatableType<ModelListOfGamesAvailable>("model", 1, 0, "ModelListOfGamesAvailable", "ModelListOfGamesAvailable error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int ModelListOfGamesAvailable::id(int index)
{
    int id = 0;
    if((index >= 0) && (index < m_listGames.count()))
        id = m_listGames[index].id;

    return id;
}

QString ModelListOfGamesAvailable::name(int index) const
{
    QString name = "";
    if((index >= 0) && (index < m_listGames.count()))
        name = m_listGames[index].name;

    return name;
}

QString ModelListOfGamesAvailable::opponent(int index) const
{
    QString opponent = "";
    if((index >= 0) && (index < m_listGames.count()))
        opponent = m_listGames[index].name;

    return opponent;
}

void ModelListOfGamesAvailable::addNewGame(int idGame, const QString& nameGame, const QString& opponent)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listGames.append({ idGame, nameGame, opponent });
    endInsertRows();

    emit countChanged();
}

void ModelListOfGamesAvailable::removeGame(int index)
{
    if(index < m_listGames.count())
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listGames.removeAt(index);
        endRemoveRows();

        emit countChanged();
    }
}

int ModelListOfGamesAvailable::count()
{
    return m_listGames.count();
}

void ModelListOfGamesAvailable::clear()
{
    m_listGames.clear();
}

QVariant ModelListOfGamesAvailable::data(const QModelIndex &index, int role) const
{
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    switch(role)
    {
    case ROLE_ID_GAME:          return m_listGames[iRow].id;
    case ROLE_NAME_GAME:        return m_listGames[iRow].name;
    case ROLE_NAME_OPPONENT:    return m_listGames[iRow].opponent;
    }

    return QVariant();
}

int ModelListOfGamesAvailable::rowCount(const QModelIndex &) const
{
    return m_listGames.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelListOfGamesAvailable::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ROLE_ID_GAME] = "idGame";
    roles[ROLE_NAME_GAME] = "nameGame";
    roles[ROLE_NAME_OPPONENT] = "nameOpponent";

    return roles;
}

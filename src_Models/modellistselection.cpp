#include "modellistselection.h"
#include <QDebug>
#include <QtQml/qqml.h>

ModelListSelection::ModelListSelection(QObject *parent) :
    QAbstractListModel(parent),
    m_listSelection({})
{

}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelListSelection::declareQML()
{
    qmlRegisterUncreatableType<ModelListSelection>("model", 1, 0, "ModelListSelection", "ModelListSelection error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QString ModelListSelection::nameSelection()
{
    return m_nameSelection;
}

void ModelListSelection::setNameSelection(const QString &name)
{
    if(m_nameSelection != name)
    {
        m_nameSelection = name;
        emit nameSelectionChanged();
    }
}

void ModelListSelection::addElement(const QString &element)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listSelection.append(element);
    endInsertRows();
}

void ModelListSelection::removeElement(int index)
{
    if((index >= 0) && (index < m_listSelection.count()))
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listSelection.removeAt(index);
        endRemoveRows();
    }
}

QString ModelListSelection::element(int index)
{
    QString element = "";
    if((index >=0) && (index < m_listSelection.count()))
        element = m_listSelection[index];

    return element;
}

void ModelListSelection::clear()
{
    m_listSelection.clear();
}

QVariant ModelListSelection::data(const QModelIndex &index, int role) const
{
    qDebug() << __PRETTY_FUNCTION__ << index << role;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    if (Qt::DisplayRole == role)
    {
        //qDebug() << __PRETTY_FUNCTION__ << m_listOfPlayers[iRow];
        return m_listSelection[iRow];
    }

    return QVariant();
}

int ModelListSelection::rowCount(const QModelIndex &) const
{
    return m_listSelection.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelListSelection::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";

    return roles;
}

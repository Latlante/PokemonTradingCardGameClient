#include "modellisttestanimations.h"
#include <QDebug>
#include <QtQml/qqml.h>

ModelListTestAnimations::ModelListTestAnimations(QObject *parent) :
    QAbstractListModel(parent)
{
    addAnimation("P Rotatif", "qrc:/qml/Tests/Test_P_Loader.qml");
    addAnimation("Pièce", "qrc:/qml/Tests/Test_RotationCoin.qml");
    addAnimation("Déplacement de carte", "qrc:/qml/Tests/Test_MovingCard.qml");
    addAnimation("Paquet simple", "");
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void ModelListTestAnimations::declareQML()
{
    qmlRegisterUncreatableType<ModelListTestAnimations>("model", 1, 0, "ModelListTestAnimations", "ModelListTestAnimations error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ModelListTestAnimations::addAnimation(const QString &name, const QString &link)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_listInfoAnimations.append({ name, link });
    endInsertRows();
}

void ModelListTestAnimations::removeAnimation(int index)
{
    if((index >= 0) && (index < rowCount()))
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listInfoAnimations.removeAt(index);
        endRemoveRows();
    }
}

void ModelListTestAnimations::clear()
{
    while(rowCount() > 0)
    {
        removeAnimation(0);
    }
}

QVariant ModelListTestAnimations::data(const QModelIndex &index, int role) const
{
    //qDebug() << __PRETTY_FUNCTION__;
    int iRow = index.row();
    if ((iRow < 0) || (iRow >= rowCount()))
    {
        qCritical() << __PRETTY_FUNCTION__ << "bad row num : " << iRow;
        return QVariant();
    }

    switch(role)
    {
    case ROLE_NAME:     return m_listInfoAnimations[iRow].name;
    case ROLE_LINK:     return m_listInfoAnimations[iRow].link;
    }

    return QVariant();
}

int ModelListTestAnimations::rowCount(const QModelIndex &) const
{
    return m_listInfoAnimations.count();
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QHash<int, QByteArray> ModelListTestAnimations::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ModelListTestAnimations::ROLE_NAME] = "name";
    roles[ModelListTestAnimations::ROLE_LINK] = "link";

    return roles;
}

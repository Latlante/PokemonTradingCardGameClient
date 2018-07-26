#include "ctrlselectingcards.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqml.h>

#include "src_Cards/abstractcard.h"
#include "common/utils.h"

CtrlSelectingCards::CtrlSelectingCards(QObject *parent) :
    QObject(parent),
    m_modelSelectingCards(new ModelSelectingCards()),
    m_proxy(new ProxyModelSelectingCards())
{
    m_proxy->setSourceModel(m_modelSelectingCards);
    /*m_proxy->setSortRole(ModelSelectingCards::SelCards_Name);
    m_proxy->setDynamicSortFilter(true);
    m_proxy->sort(0, Qt::AscendingOrder);*/
}

CtrlSelectingCards::~CtrlSelectingCards()
{
    delete m_proxy;
    if(m_modelSelectingCards != nullptr)
    {
        delete m_modelSelectingCards;
        m_modelSelectingCards = nullptr;
    }
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void CtrlSelectingCards::declareQML()
{
    qmlRegisterUncreatableType<CtrlSelectingCards>("model", 1, 0, "CtrlSelectingCards", "CtrlSelectingCards error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QString CtrlSelectingCards::name()
{
    return m_modelSelectingCards->name();
}

void CtrlSelectingCards::setName(const QString &name)
{
    m_modelSelectingCards->setName(name);
}

void CtrlSelectingCards::newSelection(const QString &name)
{
    m_modelSelectingCards->clear();
    m_modelSelectingCards->setName(name);
}

bool CtrlSelectingCards::install(QQmlApplicationEngine *pEngine)
{
    qDebug() << "installing controleur...";

    bool bInstalled = false;

    if (NULL != pEngine)
    {
        QQmlContext* pContext = pEngine->rootContext();
        if (NULL != pContext)
        {
            bInstalled = true;
            qDebug() << "CtrlSelectingCards is installed in QML engine";
            pContext->setContextProperty("ctrlSelectingCards", this);
        }
        else
        {
            qCritical() << "Qml context is null.";
        }
    }

    return bInstalled;
}

QList<InfoCard> CtrlSelectingCards::listCards()
{
    /*QList<AbstractCard *> listOfCards;
    QList<InfoCard> listOfInfoCard = m_modelSelectingCards->listCardsSelected();

    foreach(InfoCard info, listOfInfoCard)
    {
        for(int indexQuantity=0;indexQuantity<info.quantity;++indexQuantity)
        {
            listOfCards.append(info.card->clone());
        }
    }

    return listOfCards;*/
    return m_modelSelectingCards->listCardsSelected();
}

ModelSelectingCards* CtrlSelectingCards::model()
{
    QQmlEngine::setObjectOwnership(m_modelSelectingCards, QQmlEngine::CppOwnership);
    return m_modelSelectingCards;
}

ProxyModelSelectingCards* CtrlSelectingCards::proxy()
{
    QQmlEngine::setObjectOwnership(m_proxy, QQmlEngine::CppOwnership);
    return m_proxy;
}

/*void CtrlSelectingCards::addANewCard(int id)
{
    changeQuantityCard(id, m_modelSelectingCards->rowCountById(id)+1);
}

void CtrlSelectingCards::removeACard(int id)
{
    changeQuantityCard(id, m_modelSelectingCards->rowCountById(id)-1);
}

void CtrlSelectingCards::changeQuantityCard(int id, int quantity)
{
    m_modelSelectingCards->setQuantity(id, quantity);
}*/

void CtrlSelectingCards::fillARandomList()
{
    int rowCount = m_modelSelectingCards->rowCount();

    for(int i=0;i<m_modelSelectingCards->maxCards();++i)
    {
        int randomId = Utils::randomValue(0, rowCount-1);
        m_modelSelectingCards->setQuantity(randomId, m_modelSelectingCards->quantity(randomId)+1);
    }
}

void CtrlSelectingCards::savePacket(const QString& name)
{
    QList<InfoCard> listCardsSelected = m_modelSelectingCards->listCardsSelected();
    QString container = "";

    for(int i=0;i<listCardsSelected.count();++i)
    {
        if(listCardsSelected[i].quantity > 0)
            container += QString::number(listCardsSelected[i].card->id()) + "##" + QString::number(listCardsSelected[i].quantity) + "\n";
    }

    QFile fichier("save/" + name + ".ptc");
    fichier.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    fichier.write(container.toLatin1());
    fichier.close();
}

void CtrlSelectingCards::loadPacket(const QString& name)
{
    QFile fichier("save/" + name);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QString container = fichier.readAll();
    fichier.close();
    qDebug() << name << container;

    QStringList containerSplit = container.split("\n");
    m_modelSelectingCards->clear();
    foreach(QString line, containerSplit)
    {
        int id = line.section("##", 0, 0).toInt();
        int quantity = line.section("##", 1, 1).toInt();


        m_modelSelectingCards->setQuantity(id, quantity);
    }
}


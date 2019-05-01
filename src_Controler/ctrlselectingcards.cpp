#include "ctrlselectingcards.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqml.h>

#include "src_Cards/abstractcard.h"
#include "src_Models/modellistfiltersselectingcards.h"
#include "common/utils.h"

CtrlSelectingCards::CtrlSelectingCards(QObject *parent) :
    QObject(parent),
    m_modelSelectingCards(new ModelSelectingCards()),
    m_proxySelectingCards(new ProxyModelSelectingCards()),
    m_modelListFilters(new ModelListFiltersSelectingCards())
{
    m_proxySelectingCards->setSourceModel(m_modelSelectingCards);
    /*m_proxySelectingCards->setSortRole(ModelSelectingCards::SelCards_Name);
    m_proxySelectingCards->setDynamicSortFilter(true);
    m_proxySelectingCards->sort(0, Qt::AscendingOrder);*/

    m_modelListFilters->addFilter("Tout", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_AllCards));
    m_modelListFilters->addFilter("Dresseur seulement", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsTrainersOnly));
    m_modelListFilters->addFilter("Energie seulement", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsEnergiesOnly));
    m_modelListFilters->addFilter("Pokemon seulement", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsPokemonOnly));
    m_modelListFilters->addFilter("El. combat", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementFightingOnly));
    m_modelListFilters->addFilter("El. eau", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementWaterOnly));
    m_modelListFilters->addFilter("El. electrique", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementElectricOnly));
    m_modelListFilters->addFilter("El. feu", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementFireOnly));
    m_modelListFilters->addFilter("El. normal", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementNormalOnly));
    m_modelListFilters->addFilter("El. plante", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementGrassOnly));
    m_modelListFilters->addFilter("El. psy", static_cast<int>(ProxyModelSelectingCards::SelCardsFilter_CardsElementPsyOnly));
}

CtrlSelectingCards::~CtrlSelectingCards()
{
    delete m_proxySelectingCards;
    if(m_modelSelectingCards != nullptr)
    {
        delete m_modelSelectingCards;
        m_modelSelectingCards = nullptr;
    }

    if(m_modelListFilters != nullptr)
    {
        delete m_modelListFilters;
        m_modelListFilters = nullptr;
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

void CtrlSelectingCards::setFilterProxy(int filter)
{
    m_proxySelectingCards->setFilter(static_cast<ProxyModelSelectingCards::SelectingCardsFilter>(filter));
}

bool CtrlSelectingCards::install(QQmlApplicationEngine *pEngine)
{
    qDebug() << "installing controleur...";

    bool bInstalled = false;

    if (nullptr != pEngine)
    {
        QQmlContext* pContext = pEngine->rootContext();
        if (nullptr != pContext)
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
    QQmlEngine::setObjectOwnership(m_proxySelectingCards, QQmlEngine::CppOwnership);
    return m_proxySelectingCards;
}

ModelListFiltersSelectingCards* CtrlSelectingCards::modelFilters()
{
    QQmlEngine::setObjectOwnership(m_modelListFilters, QQmlEngine::CppOwnership);
    return m_modelListFilters;
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
        int idCard = m_modelSelectingCards->idFromIndex(randomId);
        m_modelSelectingCards->setQuantity(idCard, m_modelSelectingCards->quantity(idCard)+1);
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


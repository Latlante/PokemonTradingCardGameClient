#include "ctrlselectingcards.h"

#include <QDebug>
#include <QFile>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqml.h>

#include "src_Cards/abstractcard.h"
#include "utils.h"

CtrlSelectingCards::CtrlSelectingCards(QObject *parent) :
    QObject(parent),
    m_modelSelectingCards(new ModelSelectingCards())
{

}

CtrlSelectingCards::~CtrlSelectingCards()
{
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
void CtrlSelectingCards::newSelection(const QString &name, bool lastPlayer)
{
    m_modelSelectingCards->clear();
    m_modelSelectingCards->setName(name);
    m_modelSelectingCards->setLastPlayer(lastPlayer);
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

QMap<QString, QList<AbstractCard *> > CtrlSelectingCards::listCardsByPlayer()
{
    QMap<QString, QList<AbstractCard *> > newListOfCardsByPlayer;

    for(int indexPlayer=0;indexPlayer<m_listCardsByPlayer.keys().count();++indexPlayer)
    {
        QList<AbstractCard *> listOfCards;
        QString namePlayer = m_listCardsByPlayer.keys()[indexPlayer];

        QList<InfoCard> listOfInfoCard = m_listCardsByPlayer[namePlayer];

        foreach(InfoCard info, listOfInfoCard)
        {
            for(int indexQuantity=0;indexQuantity<info.quantity;++indexQuantity)
            {
                listOfCards.append(info.card->clone());
            }
        }

        newListOfCardsByPlayer.insert(namePlayer, listOfCards);
    }

    return newListOfCardsByPlayer;
}

ModelSelectingCards* CtrlSelectingCards::model()
{
    QQmlEngine::setObjectOwnership(m_modelSelectingCards, QQmlEngine::CppOwnership);
    return m_modelSelectingCards;
}

void CtrlSelectingCards::addANewCard(int id)
{
    changeQuantityCard(id, m_modelSelectingCards->rowCountById(id)+1);
}

void CtrlSelectingCards::removeACard(int id)
{
    changeQuantityCard(id, m_modelSelectingCards->rowCountById(id)-1);
}

void CtrlSelectingCards::changeQuantityCard(int id, int quantity)
{
    m_modelSelectingCards->changeQuantityCard(id, quantity);
}

void CtrlSelectingCards::fillARandomList()
{
    int rowCount = m_modelSelectingCards->rowCount();

    for(int i=0;i<m_modelSelectingCards->maxCards();++i)
    {
        int randomId = Utils::randomValue(0, rowCount-1);
        addANewCard(randomId);
    }
}

void CtrlSelectingCards::onClickedListFinished()
{
    QString namePlayerToSelect = "";
    int index = 0;

    //On enregistre la liste en se débarrassant des pointeurs
    QList<InfoCard*> listCardsSelected = m_modelSelectingCards->listCardsSelected();
    QList<InfoCard> newListCardsSelected;
    foreach(InfoCard* info, listCardsSelected)
        newListCardsSelected.append(*info);

    m_listCardsByPlayer[m_modelSelectingCards->name()] = newListCardsSelected;

    //On vérifie s'il reste des personnes qui n'ont pas fait leur sélection
    while((namePlayerToSelect == "") && (index < m_listCardsByPlayer.keys().count()))
    {
        QString currentName = m_listCardsByPlayer.keys()[index];
        if(m_listCardsByPlayer[currentName].isEmpty())
        {
            namePlayerToSelect = currentName;
        }

        index++;
    }

    //On y retourne pour le prochain joueur
    if(namePlayerToSelect != "")
    {
        newSelection(namePlayerToSelect, false);
    }
    //Tout est bon, on passe à la suite
    else
    {
        emit listsComplete();
    }
}

void CtrlSelectingCards::savePacket()
{
    QList<InfoCard*> listCardsSelected = m_modelSelectingCards->listCardsSelected();
    QString container = "";

    for(int i=0;i<listCardsSelected.count();++i)
        container += QString::number(i) + "##" + QString::number(listCardsSelected[i]->quantity) + "\n";

    QFile fichier("save/" + m_modelSelectingCards->name() + ".ptc");
    fichier.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    fichier.write(container.toLatin1());
    fichier.close();
}

void CtrlSelectingCards::loadPacket()
{
    QFile fichier("save/" + m_modelSelectingCards->name() + ".ptc");
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QString container = fichier.readAll();
    fichier.close();

    QStringList containerSplit = container.split("\n");
    m_modelSelectingCards->clear();
    foreach(QString line, containerSplit)
    {
        int id = line.section("##", 0, 0).toInt();
        int quantity = line.section("##", 1, 1).toInt();


        m_modelSelectingCards->changeQuantityCard(id, quantity);
    }
}

void CtrlSelectingCards::selectCards(const QStringList &listOfPlayers)
{
    if(listOfPlayers.count() > 0)
    {
        //Initialisation de la liste
        foreach(QString name, listOfPlayers)
        {
            m_listCardsByPlayer.insert(name, QList<InfoCard>());
        }

        newSelection(m_listCardsByPlayer.keys().first(), false);
    }
}

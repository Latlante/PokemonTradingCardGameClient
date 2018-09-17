#include "factorymainpageloader.h"
#include <QtQml/qqml.h>

FactoryMainPageLoader::FactoryMainPageLoader(QObject *parent) :
    QObject(parent),
    m_link("")
{
    displayLogin();
    //displayMenuTest();
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/
void FactoryMainPageLoader::declareQML()
{
    qmlRegisterUncreatableType<FactoryMainPageLoader>("model", 1, 0, "FactoryMainPageLoader", "FactoryMainPageLoader error");
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QString FactoryMainPageLoader::link()
{
    return m_link;
}

void FactoryMainPageLoader::displayLogin()
{
    setLink("qrc:/Login.qml");
}

void FactoryMainPageLoader::displayAllPlayers()
{
    setLink("qrc:/PageListPlayers.qml");
}

void FactoryMainPageLoader::displayAllGamesAvailable()
{
    setLink("qrc:/PageSelectionExistingGame.qml");
}

void FactoryMainPageLoader::displayCreateChooseGame()
{
    setLink("qrc:/CreateChooseGame.qml");
}

void FactoryMainPageLoader::displaySelectPlayers()
{
    setLink("qrc:/SelectPlayers.qml");
}

void FactoryMainPageLoader::displaySelectCards()
{
    setLink("qrc:/SelectingCardsQML.qml");
}

void FactoryMainPageLoader::displayBoard()
{
    setLink("qrc:/BoardQML.qml");
}

void FactoryMainPageLoader::displayPageSelection()
{
    setLink("qrc:/PageSelection.qml");
}

void FactoryMainPageLoader::displayMenuTest()
{
    setLink("qrc:/qml/Tests/MenuTest.qml");
}

void FactoryMainPageLoader::displayCustomLink(const QString& link)
{
    setLink(link);
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void FactoryMainPageLoader::setLink(const QString &link)
{
    if(m_link != link)
    {
        m_link = link;
        emit linkChanged();
    }
}

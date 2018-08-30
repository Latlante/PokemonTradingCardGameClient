#include "socketclienttest.h"
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

SocketClientTest::SocketClientTest(QObject *parent) : AbstractSocketClient(parent)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool SocketClientTest::tryToConnect()
{
    waitForTimeOut();

    return true;
}

bool SocketClientTest::authentificate(const QString &name, const QString &password, QJsonDocument &jsonResponse)
{
    Q_UNUSED(name)
    Q_UNUSED(password)

    QJsonObject jsonObj;
    jsonObj["token"] = "token";
    QJsonArray arrayGames;
    QJsonObject objGame;
    objGame["uid"] = 1;
    objGame["name"] = "game";
    objGame["opponent"] = "the bad guy";
    arrayGames.append(objGame);
    jsonObj["games"] = arrayGames;

    waitForTimeOut();
    jsonResponse = QJsonDocument(jsonObj);

    return true;
}

bool SocketClientTest::listAllPlayers(QJsonDocument &jsonResponse)
{
    Q_UNUSED(jsonResponse)
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;

    return success;
}

bool SocketClientTest::listAllGamesAlreadyExisting(QJsonDocument &jsonResponse)
{
    Q_UNUSED(jsonResponse)
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;

    return success;
}

bool SocketClientTest::createANewGame(const QString &nameGame, int idOpponent, QJsonDocument &jsonResponse)
{
    Q_UNUSED(nameGame)
    Q_UNUSED(idOpponent)
    Q_UNUSED(jsonResponse)
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;


    return success;
}

bool SocketClientTest::removeAGame()
{
    return true;
}

bool SocketClientTest::getAllInfoOnTheGame(int idGame, QJsonDocument &jsonResponse)
{
    Q_UNUSED(idGame)
    Q_UNUSED(jsonResponse)
    return true;
}

bool SocketClientTest::sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse)
{
    Q_UNUSED(idGame)
    Q_UNUSED(listInfoCards)
    Q_UNUSED(jsonResponse)
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;

    return success;
}

bool SocketClientTest::initIsReady(int idGame, QJsonDocument &jsonResponse)
{
    Q_UNUSED(idGame)
    Q_UNUSED(jsonResponse)
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;

    return success;
}

bool SocketClientTest::moveACard(int idGame, ConstantesShared::EnumPacket idPacketOrigin, int idCardOrigin, ConstantesShared::EnumPacket idPacketDestination, int idCardDestination, QJsonDocument &jsonResponse)
{
    Q_UNUSED(idGame)
    Q_UNUSED(idPacketOrigin)
    Q_UNUSED(idCardOrigin)
    Q_UNUSED(idPacketDestination)
    Q_UNUSED(idCardDestination)
    Q_UNUSED(jsonResponse)
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;

    return success;
}

void SocketClientTest::waitForTimeOut()
{
    QTimer timerTimeOut;
    timerTimeOut.setSingleShot(true);
    timerTimeOut.start(timeOut());

    QEventLoop loop;
    connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
}

bool SocketClientTest::attack(int idGame, int indexAttack, QJsonDocument &jsonResponse)
{
    return true;
}

bool SocketClientTest::skipTheTurn(int idGame, QJsonDocument &jsonResponse)
{
    return true;
}

bool SocketClientTest::responseDisplayPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse)
{
    return true;
}

bool SocketClientTest::responseDisplayAllElements(int idGame, QList<int> listIdElements, QJsonDocument &jsonResponse)
{
    return true;
}

bool SocketClientTest::responseDisplayHiddenPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse)
{
    return true;
}

bool SocketClientTest::responseDisplayEnergiesForAPokemon(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse)
{
    return true;
}

bool SocketClientTest::responseDisplayAttacksPokemon(int idGame, int indexAttack, QJsonDocument &jsonResponse)
{
    return true;
}

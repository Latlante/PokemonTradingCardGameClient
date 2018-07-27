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
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;

    return success;
}

bool SocketClientTest::createANewGame(const QString &nameGame, int idOpponent, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;


    return success;
}

bool SocketClientTest::removeAGame()
{
    return true;
}

bool SocketClientTest::sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;


    return success;
}

bool SocketClientTest::initIsReady(int idGame, QJsonDocument &jsonResponse)
{

}

bool SocketClientTest::moveACard(int idGame, ConstantesShared::EnumPacket idPacketOrigin, int idCardOrigin, ConstantesShared::EnumPacket idPacketDestination, int idCardDestination, QJsonDocument &jsonResponse)
{

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

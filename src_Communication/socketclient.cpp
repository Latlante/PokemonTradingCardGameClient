#include "socketclient.h"
#include <QEventLoop>
#include <QHostAddress>
#include <QJsonArray>

#include <QJsonObject>
#include <QTcpSocket>

#include <QTimer>
#include <QThread>

#include "common/utils.h"
#include "Share/constantesshared.h"

SocketClient::SocketClient(QObject *parent) :
    QObject(parent),
    m_socket(new QTcpSocket(this)),
    m_timeOut(10000),
    m_token("")
{
    connect(m_socket, &QTcpSocket::connected, this, &SocketClient::onConnected_Socket);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_Socket);
}

SocketClient::~SocketClient()
{
    delete m_socket;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int SocketClient::timeOut()
{
    return m_timeOut;
}

void SocketClient::setTimeOut(int timer)
{
    m_timeOut = timer;
}

void SocketClient::setToken(const QString &token)
{
    m_token = token;
}

bool SocketClient::tryToConnect()
{
    bool success = false;
    const QString ipAddress = Utils::ipAddress();
    const int port = Utils::port();

    qDebug() << __PRETTY_FUNCTION__ << ipAddress << ":" << port;

    if((ipAddress != "") && (port > 0))
    {
        QTimer timerTimeOut;
        timerTimeOut.setSingleShot(true);
        timerTimeOut.start(timeOut());

        QEventLoop loop;
        connect(m_socket, &QTcpSocket::connected, &loop, &QEventLoop::quit);
        connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

        m_socket->abort();
        m_socket->connectToHost(QHostAddress(ipAddress), port);

        loop.exec();

        if(timerTimeOut.isActive())
        {
            timerTimeOut.stop();
            success = true;
        }
    }

    return success;
}

bool SocketClient::authentificate(const QString &name, const QString &password, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["name"] = name;
    jsonRequest["password"] = password;

    if(sendMessage(QJsonDocument(jsonRequest), response))
    {
        if(!response.isNull())
        {
            success = true;
            jsonResponse = response;
        }
    }

    return success;
}

bool SocketClient::listAllPlayers(QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_ListOfAllPlayers);
    jsonRequest["token"] = m_token;

    if(sendMessage(QJsonDocument(jsonRequest), response))
    {
        if(!response.isNull())
        {
            success = true;
            jsonResponse = response;
        }
    }

    return success;
}

int SocketClient::createANewGame()
{
    return 1;
}

bool SocketClient::removeAGame()
{
    return true;
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void SocketClient::onConnected_Socket()
{
    emit connected();
}

void SocketClient::onReadyRead_Socket()
{

}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
bool SocketClient::sendMessage(QJsonDocument jsonSender, QJsonDocument &jsonResponse)
{
    bool success = false;

    QTimer timerTimeOut;
    timerTimeOut.setSingleShot(true);
    timerTimeOut.start(timeOut());

    QEventLoop loop;
    connect(m_socket, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
    connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

    m_socket->write(jsonSender.toJson());

    loop.exec();
    QByteArray response = m_socket->readAll();

    while(response[response.length()-1] != '}')
    {
        loop.exec();
        response += m_socket->readAll();
    }

    if(timerTimeOut.isActive())
    {
        timerTimeOut.stop();
        jsonResponse = QJsonDocument::fromJson(response);
        success = true;
    }

    qDebug() << __PRETTY_FUNCTION__ << "response:" << response;

    return success;
}

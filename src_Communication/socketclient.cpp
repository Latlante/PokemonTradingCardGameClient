#include "socketclient.h"
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

#include "common/utils.h"

SocketClient::SocketClient(QObject *parent) :
    QObject(parent),
    m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_Socket);
}

SocketClient::~SocketClient()
{
    delete m_socket;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool SocketClient::tryToConnect()
{
    bool success = false;
    const QString ipAddress = Utils::ipAddress();
    const int port = Utils::port();

    if((ipAddress != "") && (port > 0))
    {
        m_socket->abort();
        m_socket->connectToHost(QHostAddress(ipAddress), port);
        success = true;
    }

    return success;
}

void SocketClient::authentificate(const QString &name, const QString &password)
{
    QJsonObject jsonRequest;
    jsonRequest["name"] = name;
    jsonRequest["password"] = password;

    m_socket->write(QJsonDocument(jsonRequest).toJson(QJsonDocument::Compact));
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void SocketClient::onReadyRead_Socket()
{

}

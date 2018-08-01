#include "socketclient.h"
#include <QEventLoop>
#include <QHostAddress>
#include <QJsonArray>

#include <QJsonObject>
#include <QTcpSocket>

#include <QTimer>
#include <QThread>

#include "common/utils.h"
#include "src_Cards/cardpokemon.h"

SocketClient::SocketClient(QObject *parent) :
    AbstractSocketClient(parent),
    m_socket(new QTcpSocket(this)),
    m_token(""),
    m_bufferNotification(QByteArray())
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

bool SocketClient::listAllGamesAlreadyExisting(QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_ListOfGameForThePlayer);
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

bool SocketClient::createANewGame(const QString &nameGame, int idOpponent, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_CreateANewGame);
    jsonRequest["token"] = m_token;
    jsonRequest["name"] = nameGame;
    jsonRequest["uidOtherPlayer"] = idOpponent;

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

bool SocketClient::removeAGame()
{
    return true;
}

bool SocketClient::getAllInfoOnTheGame(int idGame, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_GetAllInfoOnGame);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

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

bool SocketClient::sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_SelectCards);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

    QJsonArray arrayCards;
    foreach(InfoCard info, listInfoCards)
    {
        QJsonObject objCard;
        objCard["id"] = info.card->id();
        objCard["quantity"] = info.quantity;

        arrayCards.append(objCard);
    }
    jsonRequest["cards"] = arrayCards;

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

bool SocketClient::initIsReady(int idGame, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_InitReady);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

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

bool SocketClient::moveACard(int idGame, ConstantesShared::EnumPacket idPacketOrigin, int idCardOrigin, ConstantesShared::EnumPacket idPacketDestination, int idCardDestination, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_MoveACard);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;
    jsonRequest["idPacketOrigin"] = static_cast<int>(idPacketOrigin);
    jsonRequest["idCardOrigin"] = idCardOrigin;
    jsonRequest["idPacketDestination"] = static_cast<int>(idPacketDestination);
    jsonRequest["idCardDestination"] = idCardDestination;

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

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void SocketClient::onConnected_Socket()
{
    emit connected();
}

void SocketClient::onReadyRead_Socket()
{
    //message received and no message send before => notification
    m_bufferNotification += m_socket->readAll();

    if(m_bufferNotification.length() >= 2)
    {
        int size = (m_bufferNotification[0] << 8) | m_bufferNotification[1];
        if(m_bufferNotification.length() >= (size + 2))
        {
            QByteArray jsonNotif = m_bufferNotification.mid(2, size);
            QJsonParseError jsonError;
            QJsonDocument docNotif = QJsonDocument::fromJson(m_bufferNotification, &jsonError);

            if(jsonError.error == QJsonParseError::NoError)
            {
                emit newNotification(docNotif);
                m_bufferNotification.clear();
            }
            else
            {
                qWarning() << __PRETTY_FUNCTION__ << "error parsing:" << jsonError.errorString();
            }
        }
    }
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
    //disconnect private slot to get message here
    disconnect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_Socket);
    connect(m_socket, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
    connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

    m_socket->write(jsonSender.toJson(QJsonDocument::Compact));

    loop.exec();
    QByteArray response = m_socket->read(2);
    bool messageOk = false;

    while(messageOk == false)
    {
        if(response.length() >= 2)
        {
            int size = (response[0] << 8) | response[1];
            response += m_socket->read(size);
            if(response.length() >= (size + 2))
            {
                messageOk = true;
                response = response.remove(0, 2);
            }
        }
        loop.exec();
        //response += m_socket->readAll();
    }

    //not listening anymore, we can reconnect the slot
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_Socket);

    if(timerTimeOut.isActive())
    {
        timerTimeOut.stop();
        jsonResponse = QJsonDocument::fromJson(response);
        success = true;
    }

    qDebug() << __PRETTY_FUNCTION__ << "response:" << response;

    return success;
}

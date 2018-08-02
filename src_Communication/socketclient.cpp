#include "socketclient.h"
#include <QDataStream>
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
    m_sizeAnswerAsynchrone(0)
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
    //init the answer
    QByteArray responseSerialize;
    QDataStream requestToRead(m_socket);

    if(m_sizeAnswerAsynchrone == 0)
    {
        //Check we have the minimum to start reading
        if(m_socket->bytesAvailable() < sizeof(quint16))
            return;

        //Check we have all the answer
        requestToRead >> m_sizeAnswerAsynchrone;
    }

    if(m_socket->bytesAvailable() < m_sizeAnswerAsynchrone)
        return;

    //From here, we have everything, so we can get all the message
    requestToRead >> responseSerialize;
    QJsonParseError jsonError;
    QJsonDocument docNotif = QJsonDocument::fromJson(responseSerialize, &jsonError);

    if(jsonError.error == QJsonParseError::NoError)
    {
        emit newNotification(docNotif);
        m_sizeAnswerAsynchrone = 0;
    }
    else
    {
        qWarning() << __PRETTY_FUNCTION__ << "error parsing:" << jsonError.errorString();
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
bool SocketClient::sendMessage(QJsonDocument jsonSender, QJsonDocument &jsonResponse)
{
    bool success = false;

    qDebug() << __PRETTY_FUNCTION__ << "jsonSender: " << jsonSender;

    //init timer time out
    QTimer timerTimeOut;
    timerTimeOut.setSingleShot(true);
    timerTimeOut.start(timeOut());

    //init request
    QByteArray jsonSerialize = jsonSender.toJson(QJsonDocument::Compact);
    QByteArray requestToSend;
    QDataStream dataToSend(&requestToSend, QIODevice::WriteOnly);
    dataToSend << static_cast<quint16>(jsonSerialize.length());
    dataToSend << jsonSerialize;

    //init loop to synchronize with the answer
    QEventLoop loop;
        //disconnect private slot to get message here
    disconnect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_Socket);
    connect(m_socket, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
    connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

    //send the request
    m_socket->write(requestToSend);

    //wait the answer
    loop.exec();

    //Check we not pass because the timer time out
    if(timerTimeOut.isActive())
    {
        //Check we have the minimum to start reading
        while(m_socket->bytesAvailable() < sizeof(quint16))
            loop.exec();
    }

    if(timerTimeOut.isActive())
    {
        //init the answer
        QByteArray responseSerialize;
        QDataStream requestToRead(m_socket);
        quint16 sizeAnswer = 0;

        requestToRead >> sizeAnswer;

        //Check we have all the answer
        while(m_socket->bytesAvailable() < sizeAnswer)
            loop.exec();

        //From here, we have everything, so we are not listening anymore, we can reconnect the slot and stop the timer
        connect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_Socket);
        if(timerTimeOut.isActive())
        {
            timerTimeOut.stop();
            requestToRead >> responseSerialize;
            jsonResponse = QJsonDocument::fromJson(responseSerialize);
            success = true;
        }
        else
            qDebug() << __PRETTY_FUNCTION__ << "Time out";
    }
    else
        qDebug() << __PRETTY_FUNCTION__ << "Time out";

    qDebug() << __PRETTY_FUNCTION__ << "response:" << jsonResponse;

    return success;
}

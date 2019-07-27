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
    m_sizeAnswerAsynchrone(0),
    m_documentBufferNewMessage()
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
        if(info.quantity > 0)
        {
            QJsonObject objCard;
            objCard["id"] = info.card->id();
            objCard["quantity"] = info.quantity;

            arrayCards.append(objCard);
        }
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

bool SocketClient::attack(int idGame, int indexAttack, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_Attack_Retreat);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;
    jsonRequest["indexAttack"] = indexAttack;

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

bool SocketClient::skipTheTurn(int idGame, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_SkipTheTurn);
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

bool SocketClient::responseDisplayPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayPacketResponse);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

    QJsonArray arrayIndex;
    foreach(int index, listIndex)
        arrayIndex.append(index);
    jsonRequest["indexPacket"] = arrayIndex;

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

bool SocketClient::responseDisplayAllElements(int idGame, QList<int> listIdElements, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayAllElementsResponse);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

    QJsonArray arrayIndex;
    foreach(int index, listIdElements)
        arrayIndex.append(index);
    jsonRequest["elements"] = arrayIndex;

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

bool SocketClient::responseDisplayHiddenPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayHiddenPacketResponse);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

    QJsonArray arrayIndex;
    foreach(int index, listIndex)
        arrayIndex.append(index);
    jsonRequest["indexPacket"] = arrayIndex;

    if(sendMessage(QJsonDocument(jsonRequest), response))
    {
        if(!response.isNull())
        {
            success = true;
            jsonResponse = response;
        }
        else
            qWarning() << __PRETTY_FUNCTION__ << "response is empty";
    }

    return success;
}

bool SocketClient::responseDisplayEnergiesForAPokemon(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayPacketResponse);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;

    QJsonArray arrayIndex;
    foreach(int index, listIndex)
        arrayIndex.append(index);
    jsonRequest["indexPacket"] = arrayIndex;

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

bool SocketClient::responseDisplayAttacksPokemon(int idGame, int indexAttack, QJsonDocument &jsonResponse)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool success = false;
    QJsonDocument response;
    QJsonObject jsonRequest;
    jsonRequest["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayAttacksPokemonResponse);
    jsonRequest["token"] = m_token;
    jsonRequest["uidGame"] = idGame;
    jsonRequest["indexPacket"] = indexAttack;

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
    qDebug() << __PRETTY_FUNCTION__;

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
    qDebug() << __PRETTY_FUNCTION__ << "response serialize:" << responseSerialize;
    QJsonParseError jsonError;
    QJsonDocument docNotif = QJsonDocument::fromJson(responseSerialize, &jsonError);

    if(jsonError.error == QJsonParseError::NoError)
    {
        //get Phase to know the kind of message
        if(docNotif.isObject())
        {
            QJsonObject obj = docNotif.object();

            //All message must contain "phase" except for the authentification
            if(obj.contains("phase"))
            {
                int phase = obj["phase"].toInt();

                if((phase < static_cast<int>(ConstantesShared::PHASE_NotifNewGameCreated)) ||
                        (phase == static_cast<int>(ConstantesShared::PHASE_DisplayPacketResponse)) ||
                        (phase == static_cast<int>(ConstantesShared::PHASE_DisplayHiddenPacketResponse)))
                {
                    m_documentBufferNewMessage = docNotif;
                    emit newMessage();
                }
                else
                    emit newNotification(docNotif);
            }
            else
                emit newNotification(docNotif);
        }

        m_sizeAnswerAsynchrone = 0;
        qDebug() << __PRETTY_FUNCTION__ << "end of the function";
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
    connect(this, &SocketClient::newMessage, &loop, &QEventLoop::quit);
    connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

    //send the request
    m_socket->write(requestToSend);

    //wait the answer
    loop.exec();

    //Check we not pass because the timer time out
    if(timerTimeOut.isActive())
    {
        timerTimeOut.stop();
        jsonResponse = m_documentBufferNewMessage;
        success = true;
    }
    else
        qDebug() << __PRETTY_FUNCTION__ << "Time out";

    qDebug() << __PRETTY_FUNCTION__ << "response:" << jsonResponse;

    return success;
}

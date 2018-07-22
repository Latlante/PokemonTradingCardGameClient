#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QJsonDocument>

#include "src_Models/modelselectingcards.h"

class QTcpSocket;

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();

    int timeOut();
    void setTimeOut(int timer);
    void setToken(const QString& token);

    bool tryToConnect();
    bool authentificate(const QString& name, const QString& password, QJsonDocument& jsonResponse);
    bool listAllPlayers(QJsonDocument &jsonResponse);
    bool createANewGame(const QString& nameGame, int idOpponent, QJsonDocument &jsonResponse);
    bool removeAGame();

    bool sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse);

signals:
    void connected();

private slots:
    void onConnected_Socket();
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;
    int m_timeOut;
    QString m_token;

    bool sendMessage(QJsonDocument jsonSender, QJsonDocument& jsonResponse);
};

#endif // SOCKETCLIENT_H

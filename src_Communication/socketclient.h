#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include "abstractsocketclient.h"

class QTcpSocket;

class SocketClient : public AbstractSocketClient
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();

    void setToken(const QString& token) override;

    bool tryToConnect() override;
    bool authentificate(const QString& name, const QString& password, QJsonDocument& jsonResponse) override;
    bool listAllPlayers(QJsonDocument &jsonResponse) override;
    bool createANewGame(const QString& nameGame, int idOpponent, QJsonDocument &jsonResponse) override;
    bool removeAGame() override;

    bool sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse) override;

signals:
    void connected();

private slots:
    void onConnected_Socket();
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;

    QString m_token;

    bool sendMessage(QJsonDocument jsonSender, QJsonDocument& jsonResponse);
};

#endif // SOCKETCLIENT_H

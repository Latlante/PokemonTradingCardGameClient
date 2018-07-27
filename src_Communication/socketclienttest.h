#ifndef SOCKETCLIENTTEST_H
#define SOCKETCLIENTTEST_H

#include "abstractsocketclient.h"

class QTimer;

class SocketClientTest : public AbstractSocketClient
{
    Q_OBJECT
public:
    explicit SocketClientTest(QObject *parent = nullptr);

    bool tryToConnect() override;
    bool authentificate(const QString& name, const QString& password, QJsonDocument& jsonResponse) override;
    bool listAllPlayers(QJsonDocument &jsonResponse) override;
    bool createANewGame(const QString& nameGame, int idOpponent, QJsonDocument &jsonResponse) override;
    bool removeAGame() override;

    bool sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse) override;
    bool initIsReady(int idGame, QJsonDocument &jsonResponse) override;

    bool moveACard(int idGame, ConstantesShared::EnumPacket idPacketOrigin, int idCardOrigin, ConstantesShared::EnumPacket idPacketDestination, int idCardDestination, QJsonDocument &jsonResponse) override;

private:
    void waitForTimeOut();
};

#endif // SOCKETCLIENTTEST_H

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include "abstractsocketclient.h"

class QTcpSocket;

class SocketClient : public AbstractSocketClient
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient() override;

    void setToken(const QString& token) override;

    bool tryToConnect() override;
    bool authentificate(const QString& name, const QString& password, QJsonDocument& jsonResponse) override;
    bool listAllPlayers(QJsonDocument &jsonResponse) override;
    bool listAllGamesAlreadyExisting(QJsonDocument &jsonResponse) override;
    bool createANewGame(const QString& nameGame, int idOpponent, QJsonDocument &jsonResponse) override;
    bool removeAGame() override;
    bool getAllInfoOnTheGame(int idGame, QJsonDocument &jsonResponse) override;

    bool sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse) override;
    bool initIsReady(int idGame, QJsonDocument &jsonResponse) override;

    bool moveACard(int idGame, ConstantesShared::EnumPacket idPacketOrigin, int idCardOrigin, ConstantesShared::EnumPacket idPacketDestination, int idCardDestination, QJsonDocument &jsonResponse) override;

    bool attack(int idGame, int indexAttack, QJsonDocument &jsonResponse) override;
    bool skipTheTurn(int idGame, QJsonDocument &jsonResponse) override;

    bool responseDisplayPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse) override;
    bool responseDisplayAllElements(int idGame, QList<int> listIdElements, QJsonDocument &jsonResponse) override;
    bool responseDisplayHiddenPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse) override;
    bool responseDisplayEnergiesForAPokemon(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse) override;
    bool responseDisplayAttacksPokemon(int idGame, int indexAttack, QJsonDocument &jsonResponse) override;

signals:
    void connected();
    void newMessage();
    void newNotification(const QJsonDocument&);

private slots:
    void onConnected_Socket();
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;

    QString m_token;
    quint16 m_sizeAnswerAsynchrone;
    QJsonDocument m_documentBufferNewMessage;

    bool sendMessage(QJsonDocument jsonSender, QJsonDocument& jsonResponse);
};

#endif // SOCKETCLIENT_H

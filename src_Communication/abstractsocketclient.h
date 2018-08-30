#ifndef ABSTRACTSOCKETCLIENT_H
#define ABSTRACTSOCKETCLIENT_H

#include <QObject>
#include <QJsonDocument>
#include "src_Models/modelselectingcards.h"
#include "Share/constantesshared.h"

class AbstractSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSocketClient(QObject *parent = nullptr);

    int timeOut();
    void setTimeOut(int timer);

    virtual void setToken(const QString& token) {}

    virtual bool tryToConnect() = 0;
    virtual bool authentificate(const QString& name, const QString& password, QJsonDocument& jsonResponse) = 0;
    virtual bool listAllPlayers(QJsonDocument &jsonResponse) = 0;
    virtual bool listAllGamesAlreadyExisting(QJsonDocument &jsonResponse) = 0;
    virtual bool createANewGame(const QString& nameGame, int idOpponent, QJsonDocument &jsonResponse) = 0;
    virtual bool removeAGame() = 0;
    virtual bool getAllInfoOnTheGame(int idGame, QJsonDocument &jsonResponse) = 0;

    virtual bool sendCardsSelected(int idGame, QList<InfoCard> listInfoCards, QJsonDocument &jsonResponse) = 0;
    virtual bool initIsReady(int idGame, QJsonDocument &jsonResponse) = 0;

    virtual bool moveACard(int idGame, ConstantesShared::EnumPacket idPacketOrigin, int idCardOrigin, ConstantesShared::EnumPacket idPacketDestination, int idCardDestination, QJsonDocument &jsonResponse) = 0;

    virtual bool attack(int idGame, int indexAttack, QJsonDocument &jsonResponse) = 0;
    virtual bool skipTheTurn(int idGame, QJsonDocument &jsonResponse) = 0;

    virtual bool responseDisplayPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse) = 0;
    virtual bool responseDisplayAllElements(int idGame, QList<int> listIdElements, QJsonDocument &jsonResponse) = 0;
    virtual bool responseDisplayHiddenPacket(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse) = 0;
    virtual bool responseDisplayEnergiesForAPokemon(int idGame, QList<int> listIndex, QJsonDocument &jsonResponse) = 0;
    virtual bool responseDisplayAttacksPokemon(int idGame, int indexAttack, QJsonDocument &jsonResponse) = 0;

signals:

private:
    int m_timeOut;
};

#endif // ABSTRACTSOCKETCLIENT_H

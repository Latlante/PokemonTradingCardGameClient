#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>

class QTcpSocket;

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();

    bool tryToConnect(int timeoutMs = 10000);
    bool authentificate(const QString& name, const QString& password);
    QStringList listAllPlayers();
    int createANewGame();
    void removeAGame();

signals:
    void connected();

private slots:
    void onConnected_Socket();
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;
};

#endif // SOCKETCLIENT_H

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

    bool tryToConnect();
    void authentificate(const QString& name, const QString& password);

signals:

private slots:
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;
};

#endif // SOCKETCLIENT_H

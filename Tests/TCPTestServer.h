#ifndef TCPTESTSERVER_H
#define TCPTESTSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QPointer>
#include <QByteArray>

class TCPTestServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPTestServer(const QByteArray& toSend, QObject *parent = 0);
    explicit TCPTestServer(int size, QObject * parent =0);

    int listenPort() const;
    QByteArray toSend() const;

private:
    QPointer<QTcpServer> _server;
    QByteArray _toSend;

    void generateRandomToSend(int size);
    
signals:
    
public slots:

private slots:
    void handleIncomingConnection();
    
};

#endif // TCPTESTSERVER_H

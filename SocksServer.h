#ifndef SOCKSSERVER_H
#define SOCKSSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QPointer>
#include <QList>

#include "SocksConnection.h"

class SocksServer : public QObject
{
    Q_OBJECT
public:
    explicit SocksServer(QObject *parent = 0);
    ~SocksServer();

    void start();

    bool isStarted() const;
    
signals:
    
public slots:

private slots:
    void handleNewIncomingConnection();

private:
    QPointer<QTcpServer> _serverSock;
    QList<QPointer<SocksConnection> > _connections;
    
};

#endif // SOCKSSERVER_H
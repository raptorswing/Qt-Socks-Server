#include "SocksServer.h"

#include <QtDebug>
#include <QTcpServer>

#include "SocksConnection.h"

SocksServer::SocksServer(QObject *parent) :
    QObject(parent)
{
}

SocksServer::~SocksServer()
{
    if (!_serverSock.isNull())
    {
        _serverSock->close();
        _serverSock->deleteLater();
    }
}

void SocksServer::start()
{
    if (!_serverSock.isNull())
        _serverSock->deleteLater();

    //TODO: Extract these parameters somewhere where they can be configured at runtime.
    const QHostAddress listenAddress = QHostAddress::Any;
    const quint16 listenPort = 1080;

    _serverSock = new QTcpServer(this);

    if (!_serverSock->listen(listenAddress,listenPort))
    {
        _serverSock->deleteLater();
        qWarning() << this << "failed to listen on" << listenAddress << listenPort;
        return;
    }


    connect(_serverSock.data(),
            SIGNAL(newConnection()),
            this,
            SLOT(handleNewIncomingConnection()));

    qDebug() << "Listening on" << listenAddress << listenPort;
}

bool SocksServer::isStarted() const
{
    if (_serverSock.isNull())
        return false;
    return _serverSock->isListening();
}

//private slot
void SocksServer::handleNewIncomingConnection()
{
    int count = 0;
    const int max = 50;
    while (_serverSock->hasPendingConnections() && ++count < max)
    {
        QTcpSocket * clientSock = _serverSock->nextPendingConnection();
        QPointer<SocksConnection> connection = new SocksConnection(clientSock,this);
        _connections.append(connection);
        //qDebug() << "Client" << clientSock->peerAddress().toString() << ":" << clientSock->peerPort() << "connected";
    }

    if (count == max)
        qDebug() << this << "looped too much";
}

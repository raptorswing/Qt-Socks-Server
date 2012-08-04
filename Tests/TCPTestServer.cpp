#include "TCPTestServer.h"

#include <QtGlobal>
#include <QDateTime>
#include <QHostInfo>
#include <QTcpSocket>

TCPTestServer::TCPTestServer(const QByteArray &toSend, QObject *parent) :
    QObject(parent), _toSend(toSend)
{
    _server = new QTcpServer(this);
    _server->listen(QHostAddress::Any,
                    0);

    connect(_server.data(),
            SIGNAL(newConnection()),
            this,
            SLOT(handleIncomingConnection()));
}

TCPTestServer::TCPTestServer(int size, QObject *parent) :
    QObject(parent)
{
    _server = new QTcpServer(this);
    _server->listen(QHostAddress::Any,
                    0);

    connect(_server.data(),
            SIGNAL(newConnection()),
            this,
            SLOT(handleIncomingConnection()));

    this->generateRandomToSend(size);
}

int TCPTestServer::listenPort() const
{
    if (_server.isNull())
        return -1;
    else if (!_server->isListening())
        return -1;
    else return _server->serverPort();
}

//private
QByteArray TCPTestServer::toSend() const
{
    return _toSend;
}

//private slot
void TCPTestServer::generateRandomToSend(int size)
{
    qsrand(QDateTime::currentDateTime().toTime_t() ^ qHash(QHostInfo::localHostName()));

    _toSend.reserve(size);
    for (int i = 0; i < size; i++)
        _toSend.append((char)(qrand() % 256));
}

//private slot
void TCPTestServer::handleIncomingConnection()
{
    while (_server->hasPendingConnections())
    {
        QTcpSocket * sock = _server->nextPendingConnection();

        qint64 written = sock->write(_toSend);
        if (written != _toSend.size())
            qWarning() << this << "Failed to write all" << _toSend.size() << "bytes. Wrote only" << written << "bytes.";
    }
}

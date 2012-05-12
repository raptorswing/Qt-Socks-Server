#include "Socks4ConnectedState.h"

#include <QtDebug>

Socks4ConnectedState::Socks4ConnectedState(QTcpSocket *socket, SocksConnection *parent) :
    SocksState(parent), _socket(socket)
{
    if (_socket.isNull())
        return;

    _socket->setParent(this);

    connect(_socket.data(),
             SIGNAL(readyRead()),
             this,
             SLOT(handleRemoteReadyRead()));
    connect(_socket.data(),
             SIGNAL(disconnected()),
             this,
             SLOT(handleRemoteDisconnect()));
}

//pure-virtual from SocksState
void Socks4ConnectedState::handleIncomingBytes(QByteArray &bytes)
{
    qint64 written = _socket->write(bytes);

    if (written < bytes.size())
        qWarning() << "Failed to write all" << bytes.size() << "desired bytes to remote connect. Wrote only" << written << "bytes";

    //Clear the bytes!
    bytes.clear();
}

//virtual from SocksState
void Socks4ConnectedState::handleSetAsNewState()
{
    if (_socket.isNull())
    {
        qWarning() << this << "received null remote socket";
        _parent->close();
        return;
    }

    //Get any bytes from the remote socket that are waiting!
    this->handleRemoteReadyRead();
}

//private slot
void Socks4ConnectedState::handleRemoteReadyRead()
{
    while (_socket->bytesAvailable())
    {
        QByteArray bytes = _socket->readAll();
        _parent->sendData(bytes);
    }
}

//private slot
void Socks4ConnectedState::handleRemoteDisconnect()
{
    //Close the client connection too
    _parent->close();
}

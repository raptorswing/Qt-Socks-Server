#include "Socks5ConnectedState.h"

#include <QPointer>

Socks5ConnectedState::Socks5ConnectedState(QTcpSocket *remoteSocket, SocksConnection *parent) :
    SocksState(parent), _socket(remoteSocket)
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

void Socks5ConnectedState::handleIncomingBytes(QByteArray &bytes)
{
    qint64 written = _socket->write(bytes);

    if (written < bytes.size())
        qWarning() << "Failed to write all" << bytes.size() << "desired bytes to remote connect. Wrote only" << written << "bytes";

    //Clear the bytes!
    bytes.clear();
}

void Socks5ConnectedState::handleSetAsNewState()
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
void Socks5ConnectedState::handleRemoteReadyRead()
{
    while (_socket->bytesAvailable())
    {
        QByteArray bytes = _socket->readAll();
        _parent->sendData(bytes);
    }
}

//private slot
void Socks5ConnectedState::handleRemoteDisconnect()
{
    //Close the client connection too
    _parent->close();
}

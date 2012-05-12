#ifndef SOCKS5CONNECTEDSTATE_H
#define SOCKS5CONNECTEDSTATE_H

#include "SocksState.h"

#include <QTcpSocket>
#include <QPointer>

class Socks5ConnectedState : public SocksState
{
    Q_OBJECT
public:
    explicit Socks5ConnectedState(QTcpSocket * remoteSocket, SocksConnection *parent = 0);

    //pure-virtual from SocksState
    virtual void handleIncomingBytes(QByteArray& bytes);

    //virtual from SocksState
    virtual void handleSetAsNewState();
    
signals:
    
public slots:

private slots:
    void handleRemoteReadyRead();
    void handleRemoteDisconnect();

private:
    QPointer<QTcpSocket> _socket;
    
};

#endif // SOCKS5CONNECTEDSTATE_H

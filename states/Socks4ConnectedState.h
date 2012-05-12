#ifndef SOCKS4CONNECTEDSTATE_H
#define SOCKS4CONNECTEDSTATE_H

#include "SocksState.h"

#include <QTcpSocket>
#include <QPointer>

class Socks4ConnectedState : public SocksState
{
    Q_OBJECT
public:
    explicit Socks4ConnectedState(QTcpSocket * socket, SocksConnection *parent = 0);

    //pure-virtual from SocksState
    virtual void handleIncomingBytes(QByteArray &bytes);

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

#endif // SOCKS4CONNECTEDSTATE_H

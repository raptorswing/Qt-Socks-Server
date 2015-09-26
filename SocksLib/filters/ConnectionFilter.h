#ifndef CONNECTIONFILTER_H
#define CONNECTIONFILTER_H

#include "SocksLib_global.h"

#include <QObject>
#include <QHostAddress>

class SOCKSLIBSHARED_EXPORT ConnectionFilter : public QObject
{
    Q_OBJECT

public:
    explicit ConnectionFilter(const QHostAddress &clientAddress, const quint16 clientPort, QObject *parent = 0) :
        QObject(parent),
        m_clientAddress(clientAddress),
        m_clientPort(clientPort)
    {}

    virtual ~ConnectionFilter() {}

    virtual bool isConnectionToAddressAllowed(const QHostAddress &address, const quint16 port) = 0;
    virtual bool isConnectionToDomainAllowed(const QString &domain, const quint16 port) = 0;

protected:
    const QHostAddress m_clientAddress;
    const quint16 m_clientPort;
};

#endif // CONNECTIONFILTER_H

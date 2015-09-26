#ifndef CONNECTIONFILTERFACTORY_H
#define CONNECTIONFILTERFACTORY_H

#include "SocksLib_global.h"
#include "ConnectionFilter.h"

#include <QObject>
#include <QHostAddress>

class SOCKSLIBSHARED_EXPORT ConnectionFilterFactory: public QObject
{
    Q_OBJECT

public:
    explicit ConnectionFilterFactory(QObject *parent = 0) : QObject(parent) {}
    virtual ~ConnectionFilterFactory() {}

    virtual ConnectionFilter *newConnectionFilter(const QHostAddress &address, const quint16 port) const = 0;
};

#endif // CONNECTIONFILTERFACTORY_H

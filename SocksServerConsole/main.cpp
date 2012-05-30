#include <QtCore/QCoreApplication>
#include <QtDebug>

#include "SocksServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocksServer server;
    server.start();

    if (!server.isStarted())
    {
        qWarning() << "Failed to start server. Shutting down";
        a.quit();
    }
    
    return a.exec();
}

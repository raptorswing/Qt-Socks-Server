#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QPointer>
#include <QTcpSocket>
#include <QScopedPointer>

#include "TCPTestServer.h"

#include "decorators/ThrottlingDecorator.h"

class BasicTCPTests : public QObject
{
    Q_OBJECT
    
public:
    BasicTCPTests();

private:
    void runThrottledTest(int size);
    void runNormalTest(int size);
    
private Q_SLOTS:
    void ThrottledSockTest();
    void NormalSockTest();
};

BasicTCPTests::BasicTCPTests()
{
}

//private
void BasicTCPTests::runThrottledTest(int size)
{
    QScopedPointer<TCPTestServer> server(new TCPTestServer(size));

    int port = server->listenPort();
    QVERIFY2(port >= 0, "Failed to setup test server");
    qDebug() << "Connect to port" << port;

    QTcpSocket * rawSock = new QTcpSocket();
    rawSock->connectToHost(QHostAddress::LocalHost,
                           port);
    int timeout = 0;
    while (rawSock->state() != QTcpSocket::ConnectedState
           && timeout++ < 50)
        QTest::qWait(10);

    QVERIFY2(timeout < 50, "Connection to test server timed out");

    QByteArray bytes;
    QScopedPointer<ThrottlingDecorator> sock(new ThrottlingDecorator(rawSock));

    int lastSize;
    do
    {
        lastSize = bytes.size();
        QTest::qWait(50);
        bytes.append(sock->readAll());
    }
    while (lastSize < bytes.size());
    qDebug() << "Read" << bytes.size() << "bytes";
    QVERIFY2(bytes.size() == server->toSend().size(),
             "Read incorrect number of bytes");
}

//private
void BasicTCPTests::runNormalTest(int size)
{
    QScopedPointer<TCPTestServer> server(new TCPTestServer(size));

    int port = server->listenPort();
    QVERIFY2(port >= 0, "Failed to setup test server");
    qDebug() << "Connect to port" << port;

    QTcpSocket * rawSock = new QTcpSocket();
    rawSock->connectToHost(QHostAddress::LocalHost,
                           port);
    int timeout = 0;
    while (rawSock->state() != QTcpSocket::ConnectedState
           && timeout++ < 50)
        QTest::qWait(10);

    QVERIFY2(timeout < 50, "Connection to test server timed out");

    QByteArray bytes;

    int lastSize;
    do
    {
        lastSize = bytes.size();
        QTest::qWait(50);
        bytes.append(rawSock->readAll());
    }
    while (lastSize < bytes.size());
    qDebug() << "Read" << bytes.size() << "bytes";
    QVERIFY2(bytes.size() == server->toSend().size(),
             "Read incorrect number of bytes");
}

//private slot
void BasicTCPTests::ThrottledSockTest()
{
    for (int i = 1; i < 100; i++)
    {
        qDebug() << "#############################";
        qDebug() << i << "KiB Test Starting";
        qDebug() << "#############################";
        this->runThrottledTest(i * 1024);
    }
}

//private slot
void BasicTCPTests::NormalSockTest()
{
    for (int i = 1; i < 100; i++)
    {
        qDebug() << "#############################";
        qDebug() << i << "KiB Test Starting";
        qDebug() << "#############################";
        this->runNormalTest(i * 1024);
    }
}

QTEST_MAIN(BasicTCPTests)

#include "tst_BasicTCPTests.moc"

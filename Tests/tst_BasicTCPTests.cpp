#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QPointer>
#include <QTcpSocket>
#include <QScopedPointer>

#include "TCPTestServer.h"

#include "decorators/ThrottlingDecorator.h"
#include "decorators/QTcpSocketDecorator.h"

class BasicTCPTests : public QObject
{
    Q_OBJECT
    
public:
    BasicTCPTests();

private:
    void runThrottledTest(int size, qreal speed = 25.0);
    void runNormalTest(int size);
    
private Q_SLOTS:
    void ThrottledSockTest();
    void NormalSockTest();
};

BasicTCPTests::BasicTCPTests()
{
}

//private
void BasicTCPTests::runThrottledTest(int size, qreal speed)
{
    speed *= 1024;
    QScopedPointer<TCPTestServer> server(new TCPTestServer(size));

    int port = server->listenPort();
    QVERIFY2(port >= 0, "Failed to setup test server");

    QTcpSocket * rawSock = new QTcpSocket();
    rawSock->connectToHost(QHostAddress::LocalHost,
                           port);
    int timeout = 0;
    while (rawSock->state() != QTcpSocket::ConnectedState
           && timeout++ < 50)
        QTest::qWait(10);

    QVERIFY2(timeout < 50, "Connection to test server timed out");

    QByteArray bytes;
    QScopedPointer<ThrottlingDecorator> sock(new ThrottlingDecorator(speed, speed, rawSock));

    int lastSize;
    do
    {
        lastSize = bytes.size();
        QTest::qWait(50);
        bytes.append(sock->readAll());
    }
    while (bytes.size() < size && sock->isReadable());

    if (bytes.size() != size)
    {
        qWarning() << "Expected to read" << size << "bytes at" << speed << "bytes/second. Read" << bytes.size() << "bytes";
        QFAIL("");
    }

}

//private
void BasicTCPTests::runNormalTest(int size)
{
    QScopedPointer<TCPTestServer> server(new TCPTestServer(size));

    int port = server->listenPort();
    QVERIFY2(port >= 0, "Failed to setup test server");

    QAbstractSocket * rawSock = new QTcpSocketDecorator();
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
    while (rawSock->isValid() && bytes.size() < size);
    QVERIFY2(bytes.size() == size,
             "Read incorrect number of bytes.");

    rawSock->deleteLater();
}

//private slot
void BasicTCPTests::ThrottledSockTest()
{
    //Small Tests
    qDebug() << "Starting small throttle tests";
    for (int speed = 5; speed < 10; speed++)
    {
        for (int i = 1; i < 20; i++)
            this->runThrottledTest(i * 1024,speed);
    }

    //Medium Tests
    qDebug() << "Starting medium throttle tests";
    for (int speed = 10; speed < 11; speed++)
    {
        for (int i = 50; i < 52; i++)
            this->runThrottledTest(i * 1024,speed);
    }

    //Large Tests
    qDebug() << "Starting large throttle tests";
    for (int speed = 20; speed < 22; speed++)
    {
        for (int i = 1024; i < 1025; i++)
            this->runThrottledTest(i * 1024,speed);
    }

}

//private slot
void BasicTCPTests::NormalSockTest()
{
    for (int i = 1; i < 100; i++)
        this->runNormalTest(i * 1024);
}

QTEST_MAIN(BasicTCPTests)

#include "tst_BasicTCPTests.moc"

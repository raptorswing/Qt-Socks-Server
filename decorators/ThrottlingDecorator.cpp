#include "ThrottlingDecorator.h"

#include <QtDebug>

ThrottlingDecorator::ThrottlingDecorator(QIODevice * toDecorate, QObject *parent) :
    QIODeviceDecorator(toDecorate, parent)
{
    _readBucket = 0;
    _writeBucket = 0;
    _readBytesPerSecond = 1024 * 250;
    _writeBytesPerSecond = 1024 * 250;

    _bucketTimer = new QTimer(this);
    connect(_bucketTimer,
            SIGNAL(timeout()),
            this,
            SLOT(handleBuckets()));
    _bucketTimer->start(50);

    _childIsFinished = false;
}

ThrottlingDecorator::~ThrottlingDecorator()
{
}

//virtual from QIODeviceDecorator
bool ThrottlingDecorator::atEnd() const
{
    return (this->bytesAvailable() <= 0);
}

//virtual from QIODeviceDecorator
qint64 ThrottlingDecorator::bytesAvailable() const
{
    qint64 toRet = qMin<qint64>(_readBucket, _readQueue.size()) + QIODevice::bytesAvailable();
    return toRet;
}

//virtual from QIODeviceDecorator
qint64 ThrottlingDecorator::bytesToWrite() const
{
    return _writeQueue.size();
}

//virtual from QIODeviceDecorator
bool ThrottlingDecorator::canReadLine() const
{
    QByteArray allAvailableData = _readQueue.mid(0,_readBucket);
    return allAvailableData.contains('\n') || QIODevice::canReadLine();
}

//virtual from QIODeviceDecorator
bool ThrottlingDecorator::waitForBytesWritten(int msecs)
{
    Q_UNUSED(msecs)
    return false;
}

//virtual from QIODeviceDecorator
bool ThrottlingDecorator::waitForReadyRead(int msecs)
{
    Q_UNUSED(msecs)
    return false;
}

//protected
//pure-virtual from QIODevice
qint64 ThrottlingDecorator::readData(char *data, qint64 maxlen)
{
    if (this->bytesAvailable() == 0)
    {
        //qDebug() << "Client called readData, but no bytes available";
        return 0;
    }

    qint64 actuallyRead = qMin<qint64>(qMin<qint64>(maxlen, _readBucket),
                                       _readQueue.size());
    _readBucket -= actuallyRead;

    QByteArray temp = _readQueue.mid(0,actuallyRead);
    _readQueue.remove(0,actuallyRead);

    for (int i = 0; i < actuallyRead; i++)
        data[i] = temp[i];

    //qDebug() << "Client read:" << temp.toHex();

    return actuallyRead;
}

//protected
//pure-virtual from QIODevice
qint64 ThrottlingDecorator::readLineData(char *data, qint64 maxlen)
{
    if (!this->canReadLine())
    {
        qWarning() << this << "readLineData() called with" << maxlen<< ", but can't read any line data";
        return 0;
    }

    int index = _readQueue.indexOf('\n');
    QByteArray temp = _readQueue.mid(0,index + 1);
    _readQueue.remove(0,index + 1);

    for (int i = 0; i < index + 1; i++)
        data[i] = temp[i];

    _readBucket -= temp.size();

    return temp.size();
}

//protected
//pure-virtual from QIODevice
qint64 ThrottlingDecorator::writeData(const char *data, qint64 len)
{
    for (int i = 0; i < len; i++)
        _writeQueue.append(data[i]);

    if (_writeBucket > 0)
        this->handleWriteQueue();

    return len;
}

//protected slot
//virtual from QIODeviceDecorator
void ThrottlingDecorator::handleChildAboutToClose()
{
    _childIsFinished = true;
}

//protected slot
//virtual from QIODeviceDecorator
void ThrottlingDecorator::handleChildReadChannelFinished()
{
    _childIsFinished = true;
}

//protected slot
//virtual from QIODeviceDecorator
void ThrottlingDecorator::handleChildReadyRead()
{
    //Read it all into our read queue
    _readQueue.append(_toDecorate->readAll());
}

//private slot
void ThrottlingDecorator::handleBuckets()
{
    const qint32 ticksPerSecond = 1000 / _bucketTimer->interval();
    const qint64 readPerTick = _readBytesPerSecond / ticksPerSecond;
    const qint64 writePerTick = _writeBytesPerSecond / ticksPerSecond;

    _readBucket = qMin<qint64>(readPerTick,
                               _readBucket + readPerTick);
    _writeBucket = qMin<qint64>(readPerTick,
                                _writeBucket + writePerTick);

    //If there are waiting bytes, notify the client that they are available now.
    if (this->bytesAvailable() > 0)
        this->readyRead();
    else if (_childIsFinished)
    {
        this->aboutToClose();
        this->close();
    }

    //Send as many bytes as we can from the write queue
    this->handleWriteQueue();
}

//private slot
void ThrottlingDecorator::handleWriteQueue()
{
    qint64 numToSend = qMin<qint64>(_writeBucket,_writeQueue.size());

    if (numToSend == 0)
        return;

    QByteArray toSend = _writeQueue.mid(0,numToSend);
    _writeQueue.remove(0,numToSend);

    _toDecorate->write(toSend);

    _writeBucket -= numToSend;
}

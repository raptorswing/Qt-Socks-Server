#include "ThrottlingDecorator.h"

#include <QtDebug>

ThrottlingDecorator::ThrottlingDecorator(QIODevice * toDecorate, QObject *parent) :
    QIODeviceDecorator(toDecorate, parent)
{
    _readBucket = 0;
    _writeBucket = 0;
    _readBytesPerSecond = 1024 * 2;
    _writeBytesPerSecond = 1024 * 2;

    _bucketTimer = new QTimer(this);
    connect(_bucketTimer,
            SIGNAL(timeout()),
            this,
            SLOT(handleBuckets()));
    _bucketTimer->start(50);
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
    return qMin<qint64>(_readBucket, _toDecorate->bytesAvailable());
}

//virtual from QIODeviceDecorator
qint64 ThrottlingDecorator::bytesToWrite() const
{
    return _writeQueue.size();
}

//virtual from QIODeviceDecorator
bool ThrottlingDecorator::canReadLine() const
{
    QByteArray peeked = _toDecorate->peek(_readBucket);
    return peeked.contains('\n');
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
    qint64 actuallyRead = _toDecorate->read(data,qMin<qint64>(_readBucket,maxlen));
    _readBucket -= actuallyRead;
    return actuallyRead;
}

//protected
//pure-virtual from QIODevice
qint64 ThrottlingDecorator::readLineData(char *data, qint64 maxlen)
{
    QByteArray temp = _toDecorate->readLine(qMin<qint64>(_readBucket,maxlen));
    if (temp.isEmpty())
        return 0;

    //Remove trailing null-byte from readLine
    temp.remove(temp.size()-1,1);

    //Copy the data
    for (int i = 0; i < temp.size(); i++)
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
void ThrottlingDecorator::handleChildReadChannelFinished()
{
    //Start a smorgasborg of reading! If our child still has data to read
    if (_toDecorate->bytesAvailable() > 0)
    {
        _readBucket += _toDecorate->bytesAvailable();
        this->readyRead();
    }

    QIODeviceDecorator::handleChildReadChannelFinished();
}

//protected slot
//virtual from QIODeviceDecorator
void ThrottlingDecorator::handleChildReadyRead()
{
    if (_readBucket > 0)
        this->readyRead();
}

//private slot
void ThrottlingDecorator::handleBuckets()
{
    qint64 readPerTick = _readBytesPerSecond / (1000 / _bucketTimer->interval());
    qint64 writePerTick = _writeBytesPerSecond / (1000 / _bucketTimer->interval());

    _readBucket = qMin<qint64>(_readBytesPerSecond,_readBucket + readPerTick);
    _writeBucket = qMin<qint64>(_writeBytesPerSecond,_writeBucket + writePerTick);

    //If there are waiting bytes, notify the client that they are available now.
    if (_toDecorate->bytesAvailable() > 0)
        this->readyRead();

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

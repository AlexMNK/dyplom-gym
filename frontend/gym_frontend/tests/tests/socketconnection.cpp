#include "socketconnection.h"

class QByteConverter
{
public:
    static json QByteToJson(QByteArray byteArray)
    {
        return json::parse(QString(byteArray).toStdString());
    }

    static QByteArray JsonToQByte(json jsonObject)
    {
        return QString::fromStdString(jsonObject.dump()).toUtf8();
    }

    static std::string QByteToString(QByteArray byteArray)
    {
        return QString(byteArray).toStdString();
    }

    static QByteArray StringToQByte(std::string stringValue)
    {
        return QString::fromStdString(stringValue).toUtf8();
    }
};

SocketConnection::SocketConnection(QTcpSocket &socket, int maxBlockingTimeout) :
    mTcpSocket{socket},
    mBlockingTimeout{maxBlockingTimeout}
{
}

SocketConnection::~SocketConnection()
{
    mTcpSocket.disconnectFromHost();

    if (mTcpSocket.state() != QAbstractSocket::UnconnectedState)
    {
         mTcpSocket.waitForDisconnected(mBlockingTimeout);
    }

    delete &mTcpSocket;
}

std::optional<json> SocketConnection::Read()
{
    const bool waitResult = mTcpSocket.waitForReadyRead(mBlockingTimeout);

    if (waitResult)
    {
        QByteArray readData = mTcpSocket.readAll();
        return QByteConverter::QByteToJson(readData);
    }

    return std::nullopt;
}

bool SocketConnection::Write(const json& jsonObject)
{
    mTcpSocket.write(QByteConverter::JsonToQByte(jsonObject));
    return mTcpSocket.waitForBytesWritten(mBlockingTimeout);
}

QByteArray SocketConnection::ReadRaw()
{
   mTcpSocket.waitForReadyRead(mBlockingTimeout);
   return mTcpSocket.readAll();
}

bool SocketConnection::WriteRaw(const QByteArray& byteArray)
{
    mTcpSocket.write(byteArray);
    return mTcpSocket.waitForBytesWritten(mBlockingTimeout);
}

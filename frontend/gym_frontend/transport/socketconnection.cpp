#include "socketconnection.h"
#include "utils/QByteArrayConverter.hpp"

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
        qDebug() << "read size: " << readData.size();
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





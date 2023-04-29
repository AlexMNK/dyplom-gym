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
    qDebug() << waitResult;

    if (waitResult)
    {
        QByteArray readData = mTcpSocket.readAll();
        return QByteConverter::QByteToJson(readData);
    }

    return std::nullopt;
}

bool SocketConnection::Write(const json jsonObject)
{
    mTcpSocket.write(QByteConverter::JsonToQByte(jsonObject));
    return mTcpSocket.waitForBytesWritten(mBlockingTimeout);
}





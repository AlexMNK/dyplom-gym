#include "serverworkerthread.h"
#include "serverhandlers.h"
#include "db/dbhelper.h"
#include "utils/QByteArrayConverter.hpp"

ServerWorkerThread::ServerWorkerThread(DBTransport* mDBTransport, SocketConnection* socketConnection) :
    mSocketConnection{socketConnection},
    mDBTransport{mDBTransport},
    mCurrentReceivedMessage(nullptr)
{
}

bool ServerWorkerThread::WaitServermessage()
{
    qDebug() << "Going to block on read";
    auto readResult = mSocketConnection->Read();
    qDebug() << "Unblocked from read";

    if (readResult)
    {
        qDebug() << "Fine result";
        mCurrentReceivedMessage = new json(readResult.value());
        qDebug() << "CurrentOperation" << QString::fromStdString((*mCurrentReceivedMessage)["Operation"]);
        return true;
    }

    return false;
}

bool ServerWorkerThread::HandleServerMessage()
{
    QString operation = QString::fromStdString((*mCurrentReceivedMessage)["Operation"]);

    json ResultJson;

    if (!ServerMessageHandler::HandleMessage(mSocketConnection, mDBTransport, operation, *mCurrentReceivedMessage, ResultJson))
    {
        qDebug() << "Error while handling client message";
        return false;
    }

    qDebug() << "Got out of handler";
    return !ResultJson.empty() ? mSocketConnection->Write(ResultJson) : false;
}

void ServerWorkerThread::StartWorkerThread()
{
    qDebug() << "Worker thread started, going for infinite loop";
    while(1)
    {
        if (!WaitServermessage())
        {
            qDebug() << "WaitServerMessage returned an error, stopping thread...";
            break;
        }

        if (!HandleServerMessage())
        {
            qDebug() << "Error while handling a message, stopping thread...";
            break;
        }
    }
}

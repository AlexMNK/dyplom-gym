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
        mCurrentReceivedMessage = new json(readResult.value());
        return true;
    }

    return false;
}

bool ServerWorkerThread::HandleServerMessage()
{
    QString operation = QString::fromStdString((*mCurrentReceivedMessage)["Operation"]);

    json ResultJson;

    if (!HandleMessage(mDBTransport, operation, *mCurrentReceivedMessage, ResultJson))
    {
        qDebug() << "Error while handling client message";
        return false;
    }

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

#include "mainserverthread.h"

static constexpr int MAX_SERVER_BLOCKING_PERIOD = -1; // -1 means that calls are blocked forever with no timeout

MainServerThread::MainServerThread(DBTransport* dbTransport, int port) :
    mDBTransport{dbTransport},
    mPort{port}
{
}

MainServerThread::~MainServerThread()
{
    if (mDBTransport != nullptr)
    {
        delete mDBTransport;
    }

    if (mTcpServer != nullptr)
    {
        delete mTcpServer;
    }

    for(const auto &thread : mWorkerThreads)
    {
        if (thread != nullptr)
        {
            delete thread;
        }
    }
}

bool MainServerThread::StartDBTransport()
{
    qDebug() << "Starting DB Transport";

    if (!mDBTransport->OpenConnection())
    {
         qDebug() << "Server failed to open a connection to a database";
         return false;
    }

    return true;
}

bool MainServerThread::StartSocketConnection()
{
    qDebug() << "Starting socket connection listening";

    this->mTcpServer = new QTcpServer();
    if (!mTcpServer->listen(QHostAddress::Any, mPort))
    {
        qDebug() << "Server failed listen on a port";
        return false;
    }

    return true;
}

void MainServerThread::RunLoop()
{
    while(1)
    {
        mTcpServer->waitForNewConnection(MAX_SERVER_BLOCKING_PERIOD);

        if (mTcpServer->hasPendingConnections())
        {
           qDebug() << "Got new connection";
           QTcpSocket *socket = mTcpServer->nextPendingConnection();

           SocketConnection* newConnection = new SocketConnection{*socket, MAX_SERVER_BLOCKING_PERIOD};
           ServerWorkerThread* worker = new ServerWorkerThread(mDBTransport, newConnection);

           QFuture<void> future = QtConcurrent::run([=](){worker->StartWorkerThread();});

           mWorkerThreads.emplace_back(worker);
           qDebug() << "After thread creation";
        }
    }
}

bool MainServerThread::StartServer()
{
    if (!StartDBTransport() || !StartSocketConnection())
    {
         qDebug() << "Failed to start a server";
         return false;
    }

    RunLoop();
    return true;
}

#ifndef MAINSERVERTHREAD_H
#define MAINSERVERTHREAD_H

#include "serverworkerthread.h"
#include "db/dbtransport.h"
#include "transport/socketconnection.h"

#include <QTcpServer>
#include <QString>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

#include <list>

class MainServerThread
{
public:
    MainServerThread(DBTransport* dbTransport, int port);
    ~MainServerThread();

    bool StartServer();

private:

    void RunLoop();

    bool StartDBTransport();
    bool StartSocketConnection();

private:
    QTcpServer* mTcpServer;
    DBTransport* mDBTransport;
    std::list<ServerWorkerThread*> mWorkerThreads;

    const int mPort;
};

#endif // MAINSERVERTHREAD_H

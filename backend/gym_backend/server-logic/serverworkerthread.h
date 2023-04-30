#ifndef SERVERWORKERTHREAD_H
#define SERVERWORKERTHREAD_H

#include <QString>
#include "db/dbtransport.h"
#include "transport/socketconnection.h"

class ServerWorkerThread: public QObject
{
Q_OBJECT
public:
    ServerWorkerThread(DBTransport* mDBTransport, SocketConnection* socketConnection);

    void StartWorkerThread();

private:
    bool WaitServermessage();
    bool HandleServerMessage();

private:
    DBTransport* mDBTransport;
    SocketConnection* mSocketConnection;
    json *mCurrentReceivedMessage;
};

#endif // SERVERWORKERTHREAD_H

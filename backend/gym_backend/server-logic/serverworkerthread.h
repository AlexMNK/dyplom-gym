#ifndef SERVERWORKERTHREAD_H
#define SERVERWORKERTHREAD_H

#include "db/dbtransport.h"
#include "transport/socketconnection.h"

#include <QString>

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

    SocketConnection* mSocketConnection;
    DBTransport* mDBTransport;
    json *mCurrentReceivedMessage;
};

#endif // SERVERWORKERTHREAD_H

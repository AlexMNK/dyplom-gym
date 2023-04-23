#include "main.h"

int main()
{
    DBTransport* dbTransport = new DBTransport("QODBC", "Driver={ODBC Driver 17 for SQL Server};server=localhost;database=GymDatabase;trusted_connection=Yes;");
    if (!dbTransport->OpenConnection())
    {
         qDebug() << "failed to open connection";
         return 1;
    }
    auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM Users");
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        while(DBHelper::GetNextQueryResultRow(query))
        {
            qDebug() << DBHelper::GetQueryData(query, 0).toInt();
            qDebug() << DBHelper::GetQueryData(query, 1).toString();
        }
    }


    QTcpServer server;
    server.listen(QHostAddress::Any, 1234); // Listen on port 1234

    qDebug() << "Server started";

    qDebug() << "Going to block";
    qDebug() << server.waitForNewConnection(-1); // Wait for a new client to connect
    qDebug() << "Unblocked";
    if (server.hasPendingConnections())
    {
        QTcpSocket *socket = server.nextPendingConnection(); // Get the socket for the new client
        qDebug() << "Client connected";

        socket->waitForReadyRead();
        QByteArray data = socket->readAll();
        qDebug() << "Received data from client:" << data;

        socket->write("Hello, client!");
        socket->waitForBytesWritten();
        qDebug() << "Written data";

     }

    json ex3 = {
      {"happy", true},
      {"pi", 3.141},
    };

    float value = ex3["pi"];

    qDebug() << value;
}

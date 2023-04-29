#include "main.h"

#include <QThread>

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

    qDebug() << server.waitForNewConnection(-1); // Wait for a new client to connect
    if (server.hasPendingConnections())
    {
        QTcpSocket *socket = server.nextPendingConnection();
        SocketConnection newConnection{*socket, 30000};

        auto optionalQuery = dbTransport->ExecuteQuery("SELECT image from Images WHERE id = 5");
        if (optionalQuery)
        {
            QSqlQuery query(std::move(optionalQuery.value()));

            while(DBHelper::GetNextQueryResultRow(query))
            {
                std::string img_string = QByteConverter::QByteToString(DBHelper::GetQueryData(query, 0).toByteArray());
                qDebug() << QString::fromStdString(img_string);
                json sendObject =
                {
                    {"status", "OK"},
                    {"image", img_string},
                };

                json sendObject2 =
                {
                    {"status", "OK"},
                    {"image", "228"},
                };

                if (newConnection.Write(sendObject))
                {
                    qDebug() << "Write succeded";
                }

                while(1)
                {

                }

                newConnection.Read();
            }


        }
    }
}

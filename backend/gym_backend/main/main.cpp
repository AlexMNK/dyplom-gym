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

    QTcpServer server;
    server.listen(QHostAddress::Any, 1234); // Listen on port 1234

    qDebug() << "Server started";

    qDebug() << server.waitForNewConnection(-1); // Wait for a new client to connect
    if (server.hasPendingConnections())
    {
        QTcpSocket *socket = server.nextPendingConnection();
        SocketConnection newConnection{*socket, 30000};

        // Wait for user id

        const auto Result = newConnection.Read();

        int id = -1;
        if (Result)
        {
            const json userData(std::move(Result.value()));
            id = userData["UserId"];
            qDebug() << id;
        }

        // Get user data

        auto optionalQuery = dbTransport->ExecuteQuery("SELECT user_name, user_password, user_profile_picture FROM Users WHERE id = " + QString::number(id));
        if (optionalQuery)
        {
            qDebug() << "1st query executed";
            QSqlQuery query(std::move(optionalQuery.value()));

            while(DBHelper::GetNextQueryResultRow(query))
            {
                std::string user_name = DBHelper::GetQueryData(query, 0).toString().toStdString();
                std::string user_password = DBHelper::GetQueryData(query, 1).toString().toStdString();
                int user_picture = DBHelper::GetQueryData(query, 2).toInt();

                qDebug() << QString::fromStdString(user_name);
                qDebug() << QString::fromStdString(user_password);
                qDebug() << user_picture;

                // Get image as QByteArray

                auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT image from Images WHERE id = " + QString::number(user_picture));
                if (optionalQuery2)
                {
                    qDebug() << "2st query executed";
                    QSqlQuery query2(std::move(optionalQuery2.value()));
                    std::string user_image;

                    while(DBHelper::GetNextQueryResultRow(query2))
                    {
                        user_image = QByteConverter::QByteToString(DBHelper::GetQueryData(query2, 0).toByteArray());
                    }

                    json sendObject =
                    {
                        {"name", user_name},
                        {"password", user_password},
                        {"image", user_image}
                    };

                    newConnection.Write(sendObject);
                }


                while(1)
                {

                }
            }
        }
    }
}

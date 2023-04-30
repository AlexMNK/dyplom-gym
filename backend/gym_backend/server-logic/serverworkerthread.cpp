#include "serverworkerthread.h"
#include "db/dbhelper.h"
#include "utils/QByteArrayConverter.hpp"

ServerWorkerThread::ServerWorkerThread(DBTransport* mDBTransport, SocketConnection* socketConnection) :
    mDBTransport{mDBTransport},
    mSocketConnection{socketConnection},
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
    std::string operation = (*mCurrentReceivedMessage)["Operation"];
    json sendObject;

    // a lot of handlers in the future

    if (operation == "GAY")
    {
        sendObject =
        {
            {"GAY", "you are gae"},
        };
    }
    else if (operation == "GetUserData")
    {
        int id = (*mCurrentReceivedMessage)["UserId"];
        qDebug() << id;

        // Get user data

        auto optionalQuery = mDBTransport->ExecuteQuery("SELECT user_name, user_password, user_profile_picture FROM Users WHERE id = " + QString::number(id));
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

                auto optionalQuery2 = mDBTransport->ExecuteQuery("SELECT image from Images WHERE id = " + QString::number(user_picture));
                if (optionalQuery2)
                {
                    qDebug() << "2st query executed";
                    QSqlQuery query2(std::move(optionalQuery2.value()));
                    std::string user_image;

                    while(DBHelper::GetNextQueryResultRow(query2))
                    {
                        user_image = QByteConverter::QByteToString(DBHelper::GetQueryData(query2, 0).toByteArray());
                    }

                    sendObject  =
                    {
                        {"name", user_name},
                        {"password", user_password},
                        {"image", user_image}
                    };
                }
            }
        }
    }

    return !sendObject.empty() ? mSocketConnection->Write(sendObject) : false;
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

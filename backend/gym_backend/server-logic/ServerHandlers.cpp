#include "serverhandlers.h"

#include "db/dbhelper.h"
#include "messaging-protocol/messagingprotocol.h"

#include <QDebug>

typedef bool (*Handler)(DBTransport* dbTransport, const json&, json&);

static bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);


static std::map<QString, Handler> operationPointers =
{
    {"GAY", &HandleGayOperation},
    {"GetUserData", &HandleGetUserDataOperation},
};


bool ServerMessageHandler::HandleMessage(DBTransport* dbTransport, const QString& operation, const json& userMessage, json& outResultMessage)
{
    return operationPointers[operation](dbTransport, userMessage, outResultMessage);
}


// -------------------------------------------- ALL HANDLERS ---------------------------------------------------//

bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    outResultMessage =
    {
        {"GAY", "you are gae"},
    };

    return true;
}

bool HandleGetUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int id;
    MessagingProtocol::AcquireGetUserData(userMessage, id);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT user_name, user_password, user_profile_picture FROM Users WHERE id = " + QString::number(id));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            QString user_name = DBHelper::GetQueryData(query, 0).toString();
            QString user_password = DBHelper::GetQueryData(query, 1).toString();
            int user_picture = DBHelper::GetQueryData(query, 2).toInt();

            auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT image from Images WHERE id = " + QString::number(user_picture));
            if (optionalQuery2)
            {
                QSqlQuery query2(std::move(optionalQuery2.value()));
                QByteArray user_image;

                if (DBHelper::GetNextQueryResultRow(query2))
                {
                    user_image = DBHelper::GetQueryData(query2, 0).toByteArray();
                }

                MessagingProtocol::BuildReplyGetUserData(outResultMessage, user_name, user_password, user_image);

                return true;
            }
        }
    }

    return false;
}




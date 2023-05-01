#include "serverhandlers.h"

#include "db/dbhelper.h"
#include "messaging-protocol/messagingprotocol.h"

#include <QDebug>

typedef bool (*Handler)(DBTransport* dbTransport, const json&, json&);

static bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleUpdateUserImageOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);


static std::map<QString, Handler> operationPointers =
{
    {"GAY", &HandleGayOperation},
    {"GetUserData", &HandleGetUserDataOperation},
    {"UpdateUserImage", &HandleUpdateUserImageOperation},
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
    int userId;
    MessagingProtocol::AcquireGetUserData(userMessage, userId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT user_name, user_password, user_profile_picture FROM Users WHERE id = " + QString::number(userId));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            QString userName = DBHelper::GetQueryData(query, 0).toString();
            QString userPassword = DBHelper::GetQueryData(query, 1).toString();
            int userPictureId = DBHelper::GetQueryData(query, 2).toInt();

            auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT image from Images WHERE id = " + QString::number(userPictureId));
            if (optionalQuery2)
            {
                QSqlQuery query2(std::move(optionalQuery2.value()));
                QByteArray userImage;

                if (DBHelper::GetNextQueryResultRow(query2))
                {
                    userImage = DBHelper::GetQueryData(query2, 0).toByteArray();
                }

                MessagingProtocol::BuildReplyGetUserData(outResultMessage, userName, userPassword, userImage);

                return true;
            }
        }
    }

    return false;
}

bool HandleUpdateUserImageOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    QByteArray imageData;
    MessagingProtocol::AcquireUpdateImage(userMessage, userId, imageData);

    qDebug() << imageData;

    auto optionalQuery = dbTransport->ExecuteQuery("Declare @IMAGE_ID int "
                                                   "INSERT INTO Images "
                                                   "VALUES (CONVERT(varbinary(MAX), '" + QString(imageData) +"')) "
                                                   "SET @IMAGE_ID = SCOPE_IDENTITY() "
                                                   "UPDATE Users "
                                                   "SET user_profile_picture = @IMAGE_ID "
                                                   "WHERE id = " + QString::number(userId));
    if (optionalQuery)
    {
        MessagingProtocol::BuildReplyUpdateImage(outResultMessage, true);
        return true;
    }

    qDebug() << "False query result";
    MessagingProtocol::BuildReplyUpdateImage(outResultMessage, false);
    return false;
}




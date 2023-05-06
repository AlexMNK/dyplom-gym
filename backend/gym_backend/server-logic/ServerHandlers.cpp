#include "serverhandlers.h"

#include "db/dbhelper.h"
#include "messaging-protocol/messagingprotocol.h"
#include "utils/DataPartImageHelper.hpp"

#include <QDebug>

typedef bool (*Handler)(DBTransport* dbTransport, const json&, json&);
typedef bool (*DataPartHandler)(SocketConnection* socketConnection, DBTransport* dbTransport, const json&, json&);

// -- Message Handlers -- //

static bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleAuthorizeOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);

static std::map<QString, Handler> messageHandlers =
{
    {"GAY", &HandleGayOperation},
    {"Authorize", &HandleAuthorizeOperation},
};

// -- Data part handlers -- //

static bool HandleGetUserDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleUpdateUserImageOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);

static std::map<QString, DataPartHandler> dataPartHandlers =
{
    {"GetUserData", &HandleGetUserDataOperation},
    {"UpdateUserImage", &HandleUpdateUserImageOperation},
};

bool ServerMessageHandler::HandleMessage(SocketConnection* socketConnection, DBTransport* dbTransport, const QString& operation, const json& userMessage, json& outResultMessage)
{
    if (!messageHandlers[operation])
    {
        return dataPartHandlers[operation](socketConnection, dbTransport, userMessage, outResultMessage);
    }

    return messageHandlers[operation](dbTransport, userMessage, outResultMessage);
}


// -------------------------------------------- MESSAGE HANDLERS ---------------------------------------------------//

bool HandleAuthorizeOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    QString userLogin, userPassword;
    MessagingProtocol::AcquireAuthorize(userMessage, userLogin, userPassword);

    if (userLogin[0] == '@') // logic with hashtags
    {
        auto optionalQuery = dbTransport->ExecuteQuery("SELECT id, user_password FROM Users "
                                                       "WHERE user_hashtag = ( "
                                                       "SELECT id FROM User_Hashtags "
                                                       "WHERE hashtag ='" + userLogin + "')");
        if (optionalQuery)
        {
            QSqlQuery query(std::move(optionalQuery.value()));
            if (DBHelper::GetNextQueryResultRow(query))
            {
                int userId = DBHelper::GetQueryData(query, 0).toInt();
                QString DBuserPassword = DBHelper::GetQueryData(query, 1).toString();
                if (userPassword == DBuserPassword)
                {
                    MessagingProtocol::BuildAuthorizeReply(outResultMessage, userId, "OK");
                }
                else
                {
                    MessagingProtocol::BuildAuthorizeReply(outResultMessage, userId, "INVALIDPASSWORD");
                }
            }
            else
            {
                MessagingProtocol::BuildAuthorizeReply(outResultMessage, 0, "NOUSER");
            }
        }
        else
        {
            return false;
        }
    }
    else // logic with email
    {
        auto optionalQuery = dbTransport->ExecuteQuery("SELECT id, user_password FROM Users "
                                                       "WHERE user_email = ( "
                                                       "SELECT user_email FROM Users "
                                                       "WHERE user_email ='" + userLogin + "')");
        if (optionalQuery)
        {
            QSqlQuery query(std::move(optionalQuery.value()));
            if (DBHelper::GetNextQueryResultRow(query))
            {
                int userId = DBHelper::GetQueryData(query, 0).toInt();
                QString DBuserPassword = DBHelper::GetQueryData(query, 1).toString();
                if (userPassword == DBuserPassword)
                {
                    MessagingProtocol::BuildAuthorizeReply(outResultMessage, userId, "OK");
                }
                else
                {
                    MessagingProtocol::BuildAuthorizeReply(outResultMessage, userId, "INVALIDPASSWORD");
                }
            }
            else
            {
                MessagingProtocol::BuildAuthorizeReply(outResultMessage, 0, "NOUSER");
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    outResultMessage =
    {
        {"GAY", "you are gae"},
    };

    return true;
}

// -------------------------------------------- DATA PART HANDLERS ---------------------------------------------------//

bool HandleGetUserDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
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

                json imageSize;
                MessagingProtocol::BuildImageSize(imageSize, userImage.size());
                socketConnection->Write(imageSize);

                DataPartImageHelper::SendImageByParts(socketConnection, userImage);

                MessagingProtocol::BuildGetUserDataReply(outResultMessage, userName, userPassword);

                return true;
            }
        }
    }

    return false;
}

bool HandleUpdateUserImageOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    json status =
    {
        {"Status", "OK"},
    };
    socketConnection->Write(status);

    int userId, imageSize;
    MessagingProtocol::AcquireUpdateImage(userMessage, userId, imageSize);

    QByteArray imageData = DataPartImageHelper::ReceiveImageByParts(socketConnection, imageSize);

    auto optionalQuery = dbTransport->ExecuteQuery("Declare @IMAGE_ID int "
                                                   "INSERT INTO Images "
                                                   "VALUES (CONVERT(varbinary(MAX), '" + QString(imageData.toHex()) +"')) "
                                                   "SET @IMAGE_ID = SCOPE_IDENTITY() "
                                                   "UPDATE Users "
                                                   "SET user_profile_picture = @IMAGE_ID "
                                                   "WHERE id = " + QString::number(userId));
    if (optionalQuery)
    {
        MessagingProtocol::BuildUpdateImageReply(outResultMessage, true);
        return true;
    }

    MessagingProtocol::BuildUpdateImageReply(outResultMessage, false);
    return false;
}




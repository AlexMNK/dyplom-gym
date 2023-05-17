#include "serverhandlers.h"

#include "db/dbhelper.h"
#include "messaging-protocol/messagingprotocol.h"
#include "utils/DataPartImageHelper.hpp"

#include <QDate>
#include <QDebug>

typedef bool (*Handler)(DBTransport* dbTransport, const json&, json&);
typedef bool (*DataPartHandler)(SocketConnection* socketConnection, DBTransport* dbTransport, const json&, json&);

// -- Message Handlers -- //

static bool HandleAuthorizeOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetUserFriendsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetPostsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetUserExercisesOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetExerciseDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetAllExercisesOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleRefreshUserTrainingWeekOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);

static std::map<QString, Handler> messageHandlers =
{
    {"Authorize", &HandleAuthorizeOperation},
    {"GetUserFriends", &HandleGetUserFriendsOperation},
    {"GetPosts", &HandleGetPostsOperation},
    {"GetUserExercises", &HandleGetUserExercisesOperation},
    {"GetExerciseData", &HandleGetExerciseDataOperation},
    {"GetAllExercises", &HandleGetAllExercisesOperation},
    {"RefreshTrainingWeek", &HandleRefreshUserTrainingWeekOperation},
};

// -- Data part handlers -- //

static bool HandleGetUserDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleUpdateUserImageOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetPostDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);

static std::map<QString, DataPartHandler> dataPartHandlers =
{
    {"GetUserData", &HandleGetUserDataOperation},
    {"UpdateUserImage", &HandleUpdateUserImageOperation},
    {"GetPostData", &HandleGetPostDataOperation},
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

bool HandleGetUserFriendsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    std::vector<std::pair<int, QString>> friendIds;
    MessagingProtocol::AcquireGetUserFriends(userMessage, userId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT responder_user_id, FORMAT(friends_since,  'dd/MM/yyyy, hh:mm:ss ') FROM Friendships WHERE requester_user_id = " + QString::number(userId) + " AND STATUS = 2 "
                                                   "UNION SELECT requester_user_id, FORMAT(friends_since,  'dd/MM/yyyy, hh:mm:ss ') FROM Friendships WHERE responder_user_id = " + QString::number(userId) + " AND STATUS = 2");
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        while (DBHelper::GetNextQueryResultRow(query))
        {
            friendIds.push_back(std::make_pair(DBHelper::GetQueryData(query, 0).toInt(), DBHelper::GetQueryData(query, 1).toString()));
        }

        MessagingProtocol::BuildGetUserFriendsReply(outResultMessage, friendIds);

        return true;
    }

    return false;
}

bool HandleGetPostsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    std::vector<int> postIds;
    MessagingProtocol::AcquireGetPosts(userMessage, userId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT id FROM Posts WHERE post_user_id IN "
                                                   "(SELECT responder_user_id FROM Friendships WHERE requester_user_id = " + QString::number(userId) + " "
                                                   "UNION SELECT requester_user_id FROM Friendships WHERE responder_user_id = " + QString::number(userId) + " AND STATUS = 2) "
                                                   "OR post_user_id = " + QString::number(userId));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        while (DBHelper::GetNextQueryResultRow(query))
        {
            postIds.push_back((DBHelper::GetQueryData(query, 0).toInt()));
        }

        MessagingProtocol::BuildGetPostsReply(outResultMessage, postIds);

        return true;
    }

    return false;
}

bool HandleGetUserExercisesOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    std::vector<int> exerciseIds;
    MessagingProtocol::AcquireGetUserExercises(userMessage, userId);

    // update statuses to not done for days before this

    dbTransport->ExecuteQuery("UPDATE User_Exercises "
                              "SET exercise_status = 3 "
                              "WHERE exercise_status = 2 AND day_of_the_week_id < " + QString::number(QDate::currentDate().dayOfWeek()) + " "
                              "AND user_id = " + QString::number(userId));

    // get all exercise ids

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT id from User_Exercises WHERE user_id = " + QString::number(userId));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        while (DBHelper::GetNextQueryResultRow(query))
        {
            exerciseIds.push_back((DBHelper::GetQueryData(query, 0).toInt()));
        }

        MessagingProtocol::BuildGetUserExercisesReply(outResultMessage, exerciseIds);

        return true;
    }

    return false;
}

bool HandleGetExerciseDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int exerciseId;
    MessagingProtocol::AcquireGetExerciseData(userMessage, exerciseId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT day_of_the_week_name, exercise_name, status_name, points_per_hour, duration from User_Exercises "
                                                   "JOIN Days_Of_The_Week on day_of_the_week_id = Days_Of_The_Week.id "
                                                   "JOIN Exercises on exercise_id = Exercises.id "
                                                   "JOIN Exercise_statuses on exercise_status = Exercise_statuses.id "
                                                   "WHERE User_Exercises.id = " + QString::number(exerciseId));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            QString dayOfTheWeek = DBHelper::GetQueryData(query, 0).toString();
            QString exerciseName = DBHelper::GetQueryData(query, 1).toString();
            QString statusName = DBHelper::GetQueryData(query, 2).toString();
            float pointsPerHour = DBHelper::GetQueryData(query, 3).toFloat();
            int duration = DBHelper::GetQueryData(query, 4).toInt();

            MessagingProtocol::BuildGetExerciseDataReply(outResultMessage, dayOfTheWeek, exerciseName, statusName, pointsPerHour, duration);

            return true;
        }
    }

    return false;
}

bool HandleGetAllExercisesOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{  
    std::vector<QString> allExercises;

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT exercise_name from Exercises");
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        while (DBHelper::GetNextQueryResultRow(query))
        {
            allExercises.push_back((DBHelper::GetQueryData(query, 0).toString()));
        }

        MessagingProtocol::BuildGetAllExercisesReply(outResultMessage, allExercises);

        return true;
    }

    return false;
}

bool HandleRefreshUserTrainingWeekOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    MessagingProtocol::AcquireRefreshUserTrainingWeek(userMessage, userId);

    dbTransport->ExecuteQuery("UPDATE User_Exercises SET exercise_status = 2 WHERE user_id = " + QString::number(userId));

    outResultMessage = {
        {"Status", "OK"},
    };

    return true;
}

// -------------------------------------------- DATA PART HANDLERS ---------------------------------------------------//

bool HandleGetUserDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    MessagingProtocol::AcquireGetUserData(userMessage, userId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM Users WHERE id = " + QString::number(userId));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            QString userName = DBHelper::GetQueryData(query, 1).toString();
            QString userEmail = DBHelper::GetQueryData(query, 2).toString();
            QString userPassword = DBHelper::GetQueryData(query, 3).toString();
            int userHashId = DBHelper::GetQueryData(query, 4).toInt();
            int userPictureId = DBHelper::GetQueryData(query, 5).toInt();
            float userBenchMax = DBHelper::GetQueryData(query, 6).toFloat();
            float userSquatMax = DBHelper::GetQueryData(query, 7).toFloat();
            float userDeadliftMax = DBHelper::GetQueryData(query, 8).toFloat();
            int userHeight = DBHelper::GetQueryData(query, 9).toInt();
            float userWeight = DBHelper::GetQueryData(query, 10).toFloat();
            int userAge = DBHelper::GetQueryData(query, 11).toInt();
            int userPoints = DBHelper::GetQueryData(query, 12).toInt();

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

                auto optionalQuery3 = dbTransport->ExecuteQuery("SELECT hashtag FROM User_Hashtags WHERE id = " + QString::number(userHashId));
                if (optionalQuery3)
                {
                    QSqlQuery query3(std::move(optionalQuery3.value()));
                    QString userHashtag;

                    if (DBHelper::GetNextQueryResultRow(query3))
                    {
                        userHashtag = DBHelper::GetQueryData(query3, 0).toString();
                    }

                    MessagingProtocol::BuildGetUserDataReply(outResultMessage, userName, userPassword, userHashtag, userEmail, userBenchMax, userSquatMax, userDeadliftMax,
                                                                 userHeight, userWeight, userAge, userPoints);

                    return true;
                }
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

bool HandleGetPostDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int postId;
    MessagingProtocol::AcquireGetPostData(userMessage, postId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT post_user_id, post_text, FORMAT(post_time,  'dd/MM/yyyy, hh:mm:ss '), post_picture from Posts WHERE id = " + QString::number(postId));
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            int postUserId = DBHelper::GetQueryData(query, 0).toInt();
            QString postText = DBHelper::GetQueryData(query, 1).toString();
            QString postTime = DBHelper::GetQueryData(query, 2).toString();
            int postPictureId = DBHelper::GetQueryData(query, 3).toInt();

            auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT image from Images WHERE id = " + QString::number(postPictureId));
            if (optionalQuery2)
            {
                QSqlQuery query2(std::move(optionalQuery2.value()));
                QByteArray postImage;

                if (DBHelper::GetNextQueryResultRow(query2))
                {
                    postImage = DBHelper::GetQueryData(query2, 0).toByteArray();
                }

                json imageSize;
                MessagingProtocol::BuildImageSize(imageSize, postImage.size());
                socketConnection->Write(imageSize);

                DataPartImageHelper::SendImageByParts(socketConnection, postImage);

                MessagingProtocol::BuildGetPostDataReply(outResultMessage, postUserId, postText, postTime);

                return true;
            }
        }
    }

    return false;
}





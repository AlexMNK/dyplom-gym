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
static bool HandleGetUserFriendRequestsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetPostsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetUserExercisesOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetExerciseDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetAllExercisesOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleRefreshUserTrainingWeekOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleAddExerciseOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleDeleteExerciseOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleEditExerciseOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleEditPostTextOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleDeletePostOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleSignUpOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleUpdateUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleReplyFriendRequestOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleDeleteFriendOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleAddFriendOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);

static std::map<QString, Handler> messageHandlers =
{
    {"Authorize", &HandleAuthorizeOperation},
    {"GetUserFriends", &HandleGetUserFriendsOperation},
    {"GetUserFriendRequests", &HandleGetUserFriendRequestsOperation},
    {"GetPosts", &HandleGetPostsOperation},
    {"GetUserExercises", &HandleGetUserExercisesOperation},
    {"GetExerciseData", &HandleGetExerciseDataOperation},
    {"GetAllExercises", &HandleGetAllExercisesOperation},
    {"RefreshTrainingWeek", &HandleRefreshUserTrainingWeekOperation},
    {"AddExercise", &HandleAddExerciseOperation},
    {"DeleteExercise", &HandleDeleteExerciseOperation},
    {"EditExercise", &HandleEditExerciseOperation},
    {"EditPostText", &HandleEditPostTextOperation},
    {"DeletePost", &HandleDeletePostOperation},
    {"SignUp", &HandleSignUpOperation},
    {"UpdateUserInfo", &HandleUpdateUserDataOperation},
    {"ReplyFriendRequest", &HandleReplyFriendRequestOperation},
    {"DeleteFriend", &HandleDeleteFriendOperation},
    {"AddFriend", &HandleAddFriendOperation},
};

// -- Data part handlers -- //

static bool HandleGetUserDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleUpdateUserImageOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetPostDataOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleAddPostOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleEditPostImageOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage);

static std::map<QString, DataPartHandler> dataPartHandlers =
{
    {"GetUserData", &HandleGetUserDataOperation},
    {"UpdateUserImage", &HandleUpdateUserImageOperation},
    {"GetPostData", &HandleGetPostDataOperation},
    {"AddPost", &HandleAddPostOperation},
    {"EditPostImage", &HandleEditPostImageOperation},
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
    else // logic with name
    {
        auto optionalQuery = dbTransport->ExecuteQuery("SELECT id, user_password FROM Users "
                                                       "WHERE user_name = '" + userLogin + "'");
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

bool HandleGetUserFriendRequestsOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    std::vector<std::pair<int, int>> friendRequestIds;
    MessagingProtocol::AcquireGetUserFriendRequests(userMessage, userId);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT id, requester_user_id FROM Friendships WHERE responder_user_id = " + QString::number(userId) + " AND STATUS = 1");
    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));

        while (DBHelper::GetNextQueryResultRow(query))
        {
            friendRequestIds.push_back(std::make_pair(DBHelper::GetQueryData(query, 1).toInt(), DBHelper::GetQueryData(query, 0).toInt()));
        }

        MessagingProtocol::BuildGetUserFriendRequestsReply(outResultMessage, friendRequestIds);

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

bool HandleAddExerciseOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId, duration;
    QString dayOfTheWeek, exerciseName;

    MessagingProtocol::AcquireAddExercise(userMessage, userId, dayOfTheWeek, exerciseName, duration);

    auto optionalQuery = dbTransport->ExecuteQuery("INSERT INTO User_Exercises "
                              " VALUES (" + QString::number(userId) +", "
                              "(SELECT id FROM Days_Of_The_Week WHERE day_of_the_week_name = '" + dayOfTheWeek + "'), "
                              "(SELECT id FROM Exercises WHERE exercise_name = '" + exerciseName + "'), "
                              "2, " +  QString::number(duration) + ")");

    if (optionalQuery)
    {
        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleDeleteExerciseOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int exerciseId;

    MessagingProtocol::AcquireDeleteExercise(userMessage, exerciseId);

    auto optionalQuery = dbTransport->ExecuteQuery("DELETE FROM User_Exercises WHERE id = " + QString::number(exerciseId));

    if (optionalQuery)
    {
        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleEditExerciseOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int exerciseId, duration, status;
    int userId, prevStatus, points_per_hour;

    MessagingProtocol::AcquireEditExercise(userMessage, exerciseId, duration, status);

    auto preparationQuery = dbTransport->ExecuteQuery("SELECT user_id, exercise_status, points_per_hour FROM User_Exercises "
                                                      "JOIN Exercises on exercise_id = Exercises.id "
                                                      "WHERE User_Exercises.id = " + QString::number(exerciseId));

    QSqlQuery query(std::move(preparationQuery.value()));

    if (DBHelper::GetNextQueryResultRow(query))
    {
        userId = DBHelper::GetQueryData(query, 0).toInt();
        prevStatus = DBHelper::GetQueryData(query, 1).toInt();
        points_per_hour = DBHelper::GetQueryData(query, 2).toInt();
    }

    auto optionalQuery = dbTransport->ExecuteQuery("UPDATE User_Exercises "
                                                   "SET exercise_status = " + QString::number(status + 1) + ", "
                                                   "duration = " + QString::number(duration) + " "
                                                   "WHERE id = " + QString::number(exerciseId));
    if (optionalQuery)
    {
        if (prevStatus != 1 && status == 0)
        {
            float temp = duration / 60.0;
            int pointsToAdd = temp * points_per_hour;

            dbTransport->ExecuteQuery("UPDATE Users SET user_points = user_points + " + QString::number(pointsToAdd) + " WHERE id = " + QString::number(userId));
        }

        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleEditPostTextOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int postId;
    QString text;

    MessagingProtocol::AcquireEditPost(userMessage, postId, text);

    auto optionalQuery = dbTransport->ExecuteQuery("UPDATE Posts SET post_text = '" + text + "' WHERE id = " + QString::number(postId));
    if (optionalQuery)
    {
        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleDeletePostOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int postId;

    MessagingProtocol::AcquireDeletePost(userMessage, postId);

    auto optionalQuery = dbTransport->ExecuteQuery("DELETE FROM Posts WHERE id = " + QString::number(postId));

    if (optionalQuery)
    {
        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleSignUpOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    QString name, email, password;

    MessagingProtocol::AcquireSignUp(userMessage, name, email, password);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM Users WHERE user_name = '" + name + "'");

    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));
        QString valueTest;

        if (DBHelper::GetNextQueryResultRow(query))
        {
            valueTest = DBHelper::GetQueryData(query, 0).toString();
        }

        if (!valueTest.isEmpty()) // case when user name is taken
        {
            int index = 1;
            while (true)
            {
                auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM Users WHERE user_name = '" + name + QString::number(index) + "'");

                if (optionalQuery)
                {
                    QSqlQuery query(std::move(optionalQuery.value()));
                    QString valueTest;

                    if (DBHelper::GetNextQueryResultRow(query))
                    {
                        valueTest = DBHelper::GetQueryData(query, 0).toString();
                    }

                    if (valueTest.isEmpty())
                    {
                        MessagingProtocol::BuildSignUpReply(outResultMessage, "NewName", name.append(QString::number(index)));
                        return true;
                    }
                    else
                    {
                        index++;
                    }
                }
            }
        }
        else // if name is unique
        {
            QString hashtag = '@' + name;

            auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM User_Hashtags WHERE hashtag = '" + hashtag + "'");

            if (optionalQuery)
            {
                QSqlQuery query(std::move(optionalQuery.value()));
                QString valueTest;

                if (DBHelper::GetNextQueryResultRow(query))
                {
                    valueTest = DBHelper::GetQueryData(query, 0).toString();
                }

                if (!valueTest.isEmpty()) // case when hashtag is taken
                {
                    int index = 1;
                    while (true)
                    {
                        auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM User_Hashtags WHERE hashtag = '" + hashtag + QString::number(index) + "'");

                        if (optionalQuery)
                        {
                            QSqlQuery query(std::move(optionalQuery.value()));
                            QString valueTest;

                            if (DBHelper::GetNextQueryResultRow(query))
                            {
                                valueTest = DBHelper::GetQueryData(query, 0).toString();
                            }

                            if (valueTest.isEmpty())
                            {
                                hashtag = hashtag.append(QString::number(index));
                                break;
                            }
                            else
                            {
                                index++;
                            }
                        }
                    }
                }

                auto optionalQuery = dbTransport->ExecuteQuery("Declare @Hash_ID int "
                                                               "INSERT INTO User_Hashtags "
                                                               "VALUES ('" + hashtag + "') "
                                                               "SET @Hash_ID = SCOPE_IDENTITY() "
                                                               "INSERT INTO Users "
                                                               "VALUES ('" + name + "', '" + email + "', '" + password + "', @Hash_ID, "
                                                               "NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0)");

                if (optionalQuery)
                {
                    outResultMessage = {
                        {"Status", "OK"},
                    };

                    return true;
                }

                outResultMessage = {
                    {"Status", "ERROR"},
                };

                return false;
            }

            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleUpdateUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    QString name, hash, email, password;
    int userId, age, height;
    float weight, bench, squat, deadlift;

    bool ifNameExists = false;
    bool ifHashExists = false;
    bool ifHashDidNotChange = true;

    MessagingProtocol::AcquireUpdateUserInfo(userMessage, userId, name, hash, email, password, age, height, weight, bench, squat, deadlift);

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT * FROM Users WHERE user_name = '" + name + "' and id != " + QString::number(userId));

    if (optionalQuery)
    {
        QSqlQuery query(std::move(optionalQuery.value()));
        QString valueTest;

        if (DBHelper::GetNextQueryResultRow(query))
        {
            valueTest = DBHelper::GetQueryData(query, 0).toString();
        }

        if (!valueTest.isEmpty())
        {
            ifNameExists = true;
        }
    }
    else
    {
        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }

    int currentHashtagId = 0;

    auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT user_hashtag FROM Users WHERE id = " + QString::number(userId));

    if (optionalQuery2)
    {
        QSqlQuery query(std::move(optionalQuery2.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            currentHashtagId = DBHelper::GetQueryData(query, 0).toInt();
        }
    }

    auto optionalQuery3 = dbTransport->ExecuteQuery("SELECT * FROM User_Hashtags WHERE hashtag = '" + hash + "'");

    if (optionalQuery3)
    {
        QSqlQuery query(std::move(optionalQuery3.value()));
        QString valueTest;

        if (DBHelper::GetNextQueryResultRow(query))
        {
            valueTest = DBHelper::GetQueryData(query, 0).toString();
        }

        if (!valueTest.isEmpty())
        {
            if (valueTest.toInt() != currentHashtagId)
            {
                ifHashExists = true;
            }
        }
        else
        {
            ifHashDidNotChange = false;
        }
    }
    else
    {
        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }

    if (!ifNameExists && !ifHashExists)
    {
        if (!ifHashDidNotChange) // if hash should be updated
        {
            auto optionalQuery4 = dbTransport->ExecuteQuery("Declare @Hash_ID int "
                                                            "INSERT INTO User_Hashtags "
                                                            "VALUES ('" + hash + "') "
                                                            "SET @Hash_ID = SCOPE_IDENTITY() "
                                                            "UPDATE Users SET "
                                                            "user_name = '" + name + "', "
                                                            "user_email = '" + email + "', "
                                                            "user_password = '" + password + "', user_hashtag = @Hash_ID, "
                                                            "user_max_bench = " + QString::number(bench) + ", "
                                                            "user_max_squat = " + QString::number(squat) + ", "
                                                            "user_max_deadlift = " + QString::number(deadlift) + ", "
                                                            "user_height = " + QString::number(height) + ", "
                                                            "user_weight = " + QString::number(weight) + ", "
                                                            "user_age = " + QString::number(age) + " WHERE id = " + QString::number(userId));

            if (optionalQuery4)
            {
                outResultMessage = {
                    {"Status", "OK"},
                };

                return true;
            }

            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
        else
        {
            auto optionalQuery4 = dbTransport->ExecuteQuery("UPDATE Users SET "
                                                            "user_name = '" + name + "', "
                                                            "user_email = '" + email + "', "
                                                            "user_password = '" + password + "', "
                                                            "user_max_bench = " + QString::number(bench) + ", "
                                                            "user_max_squat = " + QString::number(squat) + ", "
                                                            "user_max_deadlift = " + QString::number(deadlift) + ", "
                                                            "user_height = " + QString::number(height) + ", "
                                                            "user_weight = " + QString::number(weight) + ", "
                                                            "user_age = " + QString::number(age) + " WHERE id = " + QString::number(userId));

            if (optionalQuery4)
            {
                outResultMessage = {
                    {"Status", "OK"},
                };

                return true;
            }

            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
    }
    else if (ifNameExists && !ifHashExists)
    {
        if (!ifHashDidNotChange) // if hash should be updated
        {
            auto optionalQuery4 = dbTransport->ExecuteQuery("Declare @Hash_ID int "
                                                            "INSERT INTO User_Hashtags "
                                                            "VALUES ('" + hash + "') "
                                                            "SET @Hash_ID = SCOPE_IDENTITY() "
                                                            "UPDATE Users SET "
                                                            "user_email = '" + email + "', "
                                                            "user_password = '" + password + "', user_hashtag = @Hash_ID, "
                                                            "user_max_bench = " + QString::number(bench) + ", "
                                                            "user_max_squat = " + QString::number(squat) + ", "
                                                            "user_max_deadlift = " + QString::number(deadlift) + ", "
                                                            "user_height = " + QString::number(height) + ", "
                                                            "user_weight = " + QString::number(weight) + ", "
                                                            "user_age = " + QString::number(age) + " WHERE id = " + QString::number(userId));

            if (optionalQuery4)
            {
                outResultMessage = {
                    {"Status", "NameExists"},
                };

                return true;
            }

            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
        else
        {
            auto optionalQuery4 = dbTransport->ExecuteQuery("UPDATE Users SET "
                                                            "user_email = '" + email + "', "
                                                            "user_password = '" + password + "', "
                                                            "user_max_bench = " + QString::number(bench) + ", "
                                                            "user_max_squat = " + QString::number(squat) + ", "
                                                            "user_max_deadlift = " + QString::number(deadlift) + ", "
                                                            "user_height = " + QString::number(height) + ", "
                                                            "user_weight = " + QString::number(weight) + ", "
                                                            "user_age = " + QString::number(age) + " WHERE id = " + QString::number(userId));

            if (optionalQuery4)
            {
                outResultMessage = {
                    {"Status", "NameExists"},
                };

                return true;
            }

            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
    }
    else if (!ifNameExists && ifHashExists)
    {
        auto optionalQuery4 = dbTransport->ExecuteQuery("UPDATE Users SET "
                                                        "user_name = '" + name + "', "
                                                        "user_email = '" + email + "', "
                                                        "user_password = '" + password + "', "
                                                        "user_max_bench = " + QString::number(bench) + ", "
                                                        "user_max_squat = " + QString::number(squat) + ", "
                                                        "user_max_deadlift = " + QString::number(deadlift) + ", "
                                                        "user_height = " + QString::number(height) + ", "
                                                        "user_weight = " + QString::number(weight) + ", "
                                                        "user_age = " + QString::number(age) + " WHERE id = " + QString::number(userId));

        if (optionalQuery4)
        {
            outResultMessage = {
                {"Status", "HashExists"},
            };

            return true;
        }

        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
    else
    {
        auto optionalQuery4 = dbTransport->ExecuteQuery("UPDATE Users SET "
                                                        "user_email = '" + email + "', "
                                                        "user_password = '" + password + "', "
                                                        "user_max_bench = " + QString::number(bench) + ", "
                                                        "user_max_squat = " + QString::number(squat) + ", "
                                                        "user_max_deadlift = " + QString::number(deadlift) + ", "
                                                        "user_height = " + QString::number(height) + ", "
                                                        "user_weight = " + QString::number(weight) + ", "
                                                        "user_age = " + QString::number(age) + " WHERE id = " + QString::number(userId));

        if (optionalQuery4)
        {
            outResultMessage = {
                {"Status", "NameHashExists"},
            };

            return true;
        }

        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
}

bool HandleReplyFriendRequestOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int requestId;
    bool status;

    MessagingProtocol::AcquireReplyFriendRequest(userMessage, requestId, status);

    if (status)
    {
        auto optionalQuery = dbTransport->ExecuteQuery("UPDATE Friendships "
                                                       "SET status = 2, friends_since = '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "' "
                                                       "WHERE id = " + QString::number(requestId));

        if (optionalQuery)
        {
            outResultMessage = {
                {"Status", "OK"},
            };

            return true;
        }

        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
    else
    {
        auto optionalQuery = dbTransport->ExecuteQuery("DELETE FROM Friendships "
                                                       "WHERE id = " + QString::number(requestId));

        if (optionalQuery)
        {
            outResultMessage = {
                {"Status", "OK"},
            };

            return true;
        }

        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
}

bool HandleDeleteFriendOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId, friendId;

    MessagingProtocol::AcquireDeleteFriend(userMessage, userId, friendId);

    auto optionalQuery = dbTransport->ExecuteQuery("DELETE FROM Friendships "
                                                   "WHERE (requester_user_id = " + QString::number(userId) + " AND responder_user_id = " + QString::number(friendId) + ") "
                                                   "OR (requester_user_id = " + QString::number(friendId) + " AND responder_user_id = " + QString::number(userId) + ")");

    if (optionalQuery)
    {
        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }

    outResultMessage = {
        {"Status", "ERROR"},
    };

    return false;
}

bool HandleAddFriendOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId;
    QString friendName;

    MessagingProtocol::AcquireAddFriend(userMessage, userId, friendName);

    if (friendName[0] == '@') // logic with hashtags
    {
        auto optionalQuery = dbTransport->ExecuteQuery("SELECT id FROM User_Hashtags WHERE hashtag = '" + friendName + "'");
        if (optionalQuery)
        {
            QSqlQuery query(std::move(optionalQuery.value()));
            QString valueTest;
            int hashId = 0;

            if (DBHelper::GetNextQueryResultRow(query))
            {
                valueTest = DBHelper::GetQueryData(query, 0).toString();
            }

            if (!valueTest.isEmpty())
            {
                hashId = valueTest.toInt();
            }
            else
            {
                outResultMessage = {
                    {"Status", "NOUSER"},
                };

                return true;
            }

            auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT id FROM Users WHERE user_hashtag = " + QString::number(hashId));
            if (optionalQuery2)
            {
                QSqlQuery query2(std::move(optionalQuery2.value()));
                int friendId = 0;

                if (DBHelper::GetNextQueryResultRow(query2))
                {
                    friendId = DBHelper::GetQueryData(query2, 0).toInt();
                }

                auto optionalQuery3 = dbTransport->ExecuteQuery("INSERT INTO Friendships "
                                                                "VALUES (" + QString::number(userId) + ", "
                                                                + QString::number(friendId) + ", 1, NULL)");
                if (optionalQuery3)
                {
                    outResultMessage = {
                        {"Status", "OK"},
                    };

                    return true;
                }

                outResultMessage = {
                    {"Status", "ERROR"},
                };

                return false;
            }
        }

        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
    else // logic with User Name
    {
        auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT id FROM Users WHERE user_name = '" + friendName + "'");
        if (optionalQuery2)
        {
            QSqlQuery query2(std::move(optionalQuery2.value()));
            QString valueTest;
            int friendId = 0;

            if (DBHelper::GetNextQueryResultRow(query2))
            {
                valueTest = DBHelper::GetQueryData(query2, 0).toString();
            }

            if (!valueTest.isEmpty())
            {
                friendId = valueTest.toInt();
            }
            else
            {
                outResultMessage = {
                    {"Status", "NOUSER"},
                };

                return true;
            }

            auto optionalQuery3 = dbTransport->ExecuteQuery("INSERT INTO Friendships "
                                                            "VALUES (" + QString::number(userId) + ", "
                                                            + QString::number(friendId) + ", 1, NULL)");
            if (optionalQuery3)
            {
                outResultMessage = {
                    {"Status", "OK"},
                };

                return true;
            }

            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }

        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
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

                if (userImage.size() != 0)
                {
                    DataPartImageHelper::SendImageByParts(socketConnection, userImage);
                }

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

                if (postImage.size() != 0)
                {
                    DataPartImageHelper::SendImageByParts(socketConnection, postImage);
                }

                MessagingProtocol::BuildGetPostDataReply(outResultMessage, postUserId, postText, postTime);

                return true;
            }
        }
    }

    return false;
}

bool HandleAddPostOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int userId, imageSize;
    QString text;

    MessagingProtocol::AcquireAddPost(userMessage, userId, text, imageSize);

    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");

    if (imageSize == 0) // case with no image
    {


        auto optionalQuery = dbTransport->ExecuteQuery("INSERT INTO Posts "
                                                       "VALUES (" + QString::number(userId) + ", "
                                                       "'" + text + "', '"
                                                       + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "', NULL)");
        if (optionalQuery)
        {
            outResultMessage = {
                {"Status", "OK"},
            };

            return true;
        }
        else
        {
            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
    }
    else // case with image
    {
        QByteArray imageData = DataPartImageHelper::ReceiveImageByParts(socketConnection, imageSize);

        auto optionalQuery = dbTransport->ExecuteQuery("Declare @IMAGE_ID int "
                                                       "INSERT INTO Images "
                                                       "VALUES (CONVERT(varbinary(MAX), '" + QString(imageData.toHex()) +"')) "
                                                       "SET @IMAGE_ID = SCOPE_IDENTITY() "
                                                       "INSERT INTO Posts "
                                                       "VALUES (" + QString::number(userId) + ", "
                                                       "'" + text + "', '"
                                                       + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "', @IMAGE_ID)");

        if (optionalQuery)
        {
            outResultMessage = {
                {"Status", "OK"},
            };

            return true;
        }
        else
        {
            outResultMessage = {
                {"Status", "ERROR"},
            };

            return false;
        }
    }
}

bool HandleEditPostImageOperation(SocketConnection* socketConnection, DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int postId, imageSize;
    MessagingProtocol::AcquireUpdatePostImage(userMessage, postId, imageSize);

    QByteArray imageData = DataPartImageHelper::ReceiveImageByParts(socketConnection, imageSize);

    auto optionalQuery = dbTransport->ExecuteQuery("Declare @IMAGE_ID int "
                                                   "INSERT INTO Images "
                                                   "VALUES (CONVERT(varbinary(MAX), '" + QString(imageData.toHex()) +"')) "
                                                   "SET @IMAGE_ID = SCOPE_IDENTITY() "
                                                   "UPDATE Posts "
                                                   "SET post_picture = @IMAGE_ID "
                                                   "WHERE id = " + QString::number(postId));
    if (optionalQuery)
    {
        outResultMessage = {
            {"Status", "OK"},
        };

        return true;
    }
    else
    {
        outResultMessage = {
            {"Status", "ERROR"},
        };

        return false;
    }
}





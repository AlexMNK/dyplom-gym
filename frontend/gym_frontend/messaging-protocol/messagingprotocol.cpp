#include "messagingprotocol.h"
#include "utils/QByteArrayConverter.hpp"

#include <QDebug>

//
void MessagingProtocol::BuildAuthorize(json& outMessage, const QString& userLogin, const QString& userPassword)
{
    outMessage =
    {
        {"Operation", "Authorize"},
        {"UserLogin", userLogin.toStdString()},
        {"UserPassword", userPassword.toStdString()},
    };
}

void MessagingProtocol::AcquireAuthorizeReply(const json& message, int& outUserId, QString& outStatus)
{
    outUserId = message["UserId"];
    outStatus = QString::fromStdString(message["Status"]);
}

//
void MessagingProtocol::BuildGetUserData(json& outMessage, int userId)
{
    outMessage =
    {
        {"Operation", "GetUserData"},
        {"UserId", userId},
    };
}

void MessagingProtocol::AcquireGetUserDataReply(const json& message, QString& outUserName, QString& outUserPassword, QString& outUserHashtag, QString& outUserEmail, float& outUserMaxBench
                                                    , float& outUserMaxSquat, float& outUserMaxDeadlift, int& outUserHeight, float& outUserWeight, int &outUserAge, int &outUserPoints)
{
    outUserName = QString::fromStdString(message["Name"]);
    outUserPassword = QString::fromStdString(message["Password"]);
    outUserHashtag = QString::fromStdString(message["Hashtag"]);
    outUserEmail = QString::fromStdString(message["Email"]);
    outUserMaxBench = message["MaxBench"];
    outUserMaxSquat = message["MaxSquat"];
    outUserMaxDeadlift = message["MaxDeadlift"];
    outUserHeight = message["Height"];
    outUserWeight = message["Weight"];
    outUserAge = message["Age"];
    outUserPoints = message["Points"];
}

//
void MessagingProtocol::BuildGetUserFriends(json& outMessage, int userId)
{
    outMessage =
    {
        {"Operation", "GetUserFriends"},
        {"UserId", userId},
    };
}

void MessagingProtocol::AcquireGetUserFriendsReply(const json& message, std::vector<std::pair<int, QString>>& outIds)
{
    int size = message["Friends"].size();

    for(int i = 0; i < size; ++i)
    {
        outIds.push_back(std::make_pair(message["Friends"][i], QString::fromStdString(message["FriendsSince"][i])));
    }
}

//
void MessagingProtocol::BuildGetPosts(json& outMessage, int userId)
{
    outMessage =
    {
        {"Operation", "GetPosts"},
        {"UserId", userId},
    };
}

void MessagingProtocol::AcquireGetPostsReply(const json& message, std::vector<int>& outIds)
{
    int size = message["PostIds"].size();

    for(int i = 0; i < size; ++i)
    {
        outIds.push_back(message["PostIds"][i]);
    }
}

//
void MessagingProtocol::BuildGetPostData(json& outMessage, int postId)
{
    outMessage =
    {
        {"Operation", "GetPostData"},
        {"PostId", postId},
    };
}

void MessagingProtocol::AcquireGetPostDataReply(const json& message, int& outPostUserId, QString& outPostText, QString& outPostTime)
{
    outPostUserId = message["PostUserId"];
    outPostText = QString::fromStdString(message["PostText"]);
    outPostTime = QString::fromStdString(message["PostTime"]);
}

//
void MessagingProtocol::BuildUpdateImage(json& outMessage, int userId, int imageSize)
{
    outMessage =
    {
        {"Operation", "UpdateUserImage"},
        {"UserId", userId},
        {"ImageSize", imageSize},
    };
}

void MessagingProtocol::AcquireUpdateImageReply(const json& message, bool& outResult)
{
    outResult = message["Result"];
}

//
void MessagingProtocol::BuildImageSize(json& outMessage, const int size)
{
    outMessage =
    {
        {"ImageSize", size},
    };
}

void MessagingProtocol::AcquireImageSize(const json& message, int& outImageSize)
{
    outImageSize = message["ImageSize"];
}

//
void MessagingProtocol::BuildGetUserExercises(json& outMessage, int userId)
{
    outMessage =
    {
        {"Operation", "GetUserExercises"},
        {"UserId", userId},
    };
}

void MessagingProtocol::AcquireGetUserExercisesReply(const json& message, std::vector<int>& outIds)
{
    int size = message["ExerciseIds"].size();

    for(int i = 0; i < size; ++i)
    {
        outIds.push_back(message["ExerciseIds"][i]);
    }
}

//
void MessagingProtocol::BuildGetExerciseData(json& outMessage, int exerciseId)
{
    outMessage =
    {
        {"Operation", "GetExerciseData"},
        {"ExerciseId", exerciseId},
    };
}

void MessagingProtocol::AcquireGetExerciseDataReply(const json& message, QString& outDayOfTheWeek, QString& outName, QString& outStatus, float& pointsPerHour, int& outDuration)
{
    outDayOfTheWeek = QString::fromStdString(message["DayOfTheWeek"]);
    outName = QString::fromStdString(message["ExerciseName"]);
    outStatus = QString::fromStdString(message["Status"]);
    pointsPerHour = message["PointsPerHour"];
    outDuration = message["Duration"];

    qDebug() << QString::fromStdString(message["Status"]);
}

//
void MessagingProtocol::BuildGetAllExercises(json& outMessage)
{
    outMessage =
    {
        {"Operation", "GetAllExercises"},
    };
}

void MessagingProtocol::AcquireGetAllExercisesReply(const json& message, std::vector<QString>& outExercises)
{
    int size = message["Exercises"].size();

    for(int i = 0; i < size; ++i)
    {
        outExercises.push_back(QString::fromStdString(message["Exercises"][i]));
    }
}

//
void MessagingProtocol::BuildRefreshUserTrainingWeek(json& outMessage, int userId)
{
    outMessage =
    {
        {"Operation", "RefreshTrainingWeek"},
        {"UserId", userId},
    };
}

//
void MessagingProtocol::BuildAddExercise(json& outMessage, int userId, const QString& dayOfTheWeek, const QString& exerciseName, int duration)
{
    outMessage =
    {
        {"Operation", "AddExercise"},
        {"UserId", userId},
        {"DayOfTheWeek", dayOfTheWeek.toStdString()},
        {"ExerciseName", exerciseName.toStdString()},
        {"Duration", duration},
    };
}

//
void MessagingProtocol::BuildDeleteExercise(json& outMessage, int exerciseId)
{
    outMessage =
    {
        {"Operation", "DeleteExercise"},
        {"ExerciseId", exerciseId},
    };
}

//
void MessagingProtocol::BuildEditExercise(json& outMessage, int exerciseId, int duration, int status)
{
    outMessage =
    {
        {"Operation", "EditExercise"},
        {"ExerciseId", exerciseId},
        {"Duration", duration},
        {"Status", status},
    };
}

//
void MessagingProtocol::BuildAddPost(json& outMessage, int userId, const QString& text, int imageSize)
{
    outMessage =
    {
        {"Operation", "AddPost"},
        {"UserId", userId},
        {"Text", text.toStdString()},
        {"ImageSize", imageSize},
    };
}

//
void MessagingProtocol::BuildEditPost(json& outMessage, int postId, const QString& text)
{
    outMessage =
    {
        {"Operation", "EditPostText"},
        {"PostId", postId},
        {"Text", text.toStdString()},
    };
}

//
void MessagingProtocol::BuildUpdatePostImage(json& outMessage, int postId, int imageSize)
{
    outMessage =
    {
        {"Operation", "EditPostImage"},
        {"PostId", postId},
        {"ImageSize", imageSize},
    };
}

//
void MessagingProtocol::BuildDeletePost(json& outMessage, int postId)
{
    outMessage =
    {
        {"Operation", "DeletePost"},
        {"PostId", postId},
    };
}

//
void MessagingProtocol::BuildSignUp(json& outMessage, const QString& userName, const QString& userEmail, const QString& userPassword)
{
    outMessage =
    {
        {"Operation", "SignUp"},
        {"UserName", userName.toStdString()},
        {"UserEmail", userEmail.toStdString()},
        {"UserPassword", userPassword.toStdString()},
    };
}

void MessagingProtocol::AcquireSignUpReply(const json& message, QString& outStatus, QString& outNewName)
{
    outStatus = QString::fromStdString(message["Status"]);
    if (outStatus == "NewName")
    {
        outNewName = QString::fromStdString(message["NewName"]);
    }
}






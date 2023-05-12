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

void MessagingProtocol::AcquireGetExerciseDataReply(const json& message, QString& outDayOfTheWeek, QString& outName, int& outDuration)
{
    outDayOfTheWeek = QString::fromStdString(message["DayOfTheWeek"]);
    outName = QString::fromStdString(message["ExerciseName"]);
    outDuration = message["Duration"];
}





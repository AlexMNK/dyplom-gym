#include "messagingprotocol.h"
#include "utils/QByteArrayConverter.hpp"

#include <QDebug>

void MessagingProtocol::AcquireAuthorize(const json& message, QString& outUserLogin, QString& outUserPassword)
{
    outUserLogin = QString::fromStdString(message["UserLogin"]);
    outUserPassword = QString::fromStdString(message["UserPassword"]);
}

void MessagingProtocol::BuildAuthorizeReply(json& outMessage, int userId, const QString& status)
{
    outMessage =
    {
        {"UserId", userId},
        {"Status", status.toStdString()},
    };
}

//
void MessagingProtocol::AcquireGetUserData(const json& message, int& outId)
{
    outId = message["UserId"];
}

void MessagingProtocol::BuildGetUserDataReply(json& outMessage, const QString& userName, const QString& userPassword, const QString& userHashtag, const QString& userEmail,
                                                  float userMaxBench, float userMaxSquat, float userMaxDeadlift, int userHeight, float userWeight,
                                                  int userAge, int userPoints)
{
    outMessage =
    {
        {"Name", userName.toStdString()},
        {"Password", userPassword.toStdString()},
        {"Hashtag", userHashtag.toStdString()},
        {"Email", userEmail.toStdString()},
        {"MaxBench", userMaxBench},
        {"MaxSquat", userMaxSquat},
        {"MaxDeadlift", userMaxDeadlift},
        {"Height", userHeight},
        {"Weight", userWeight},
        {"Age", userAge},
        {"Points", userPoints},
    };
}

//
void MessagingProtocol::AcquireGetPosts(const json& message, int& outId)
{
    outId = message["UserId"];
}

void MessagingProtocol::BuildGetPostsReply(json& outMessage, const std::vector<int> postIds)
{
    json idsArray;

    for (const auto& id : postIds)
    {
        idsArray.push_back(id);
    }

    outMessage =
    {
        {"PostIds", idsArray},
    };
}

//
void MessagingProtocol::AcquireGetPostData(const json& message, int& outPostId)
{
    outPostId = message["PostId"];
}

void MessagingProtocol::BuildGetPostDataReply(json& outMessage, const int postUserId, const QString& postText, const QString& postTime)
{
    outMessage =
    {
        {"PostUserId", postUserId},
        {"PostText", postText.toStdString()},
        {"PostTime", postTime.toStdString()},
    };
}

//
void MessagingProtocol::AcquireGetUserFriends(const json& message, int& outId)
{
    outId = message["UserId"];
}

void MessagingProtocol::BuildGetUserFriendsReply(json& outMessage, const std::vector<std::pair<int, QString>> friendIds)
{
    json idsArray;

    for (const auto& friends : friendIds)
    {
        idsArray.push_back(friends.first);
    }

    json frSinceArray;

    for (const auto& friends : friendIds)
    {
        frSinceArray.push_back(friends.second.toStdString());
    }

    outMessage =
    {
        {"Friends", idsArray},
        {"FriendsSince", frSinceArray},
    };
}

//
void MessagingProtocol::AcquireUpdateImage(const json& message, int& outId, int& outImageSize)
{
    outId = message["UserId"];
    outImageSize = message["ImageSize"];
}

void MessagingProtocol::BuildUpdateImageReply(json& outMessage, const bool result)
{
    outMessage =
    {
        {"Result", result},
    };
}

//
void MessagingProtocol::AcquireImageSize(const json& message, int& outImageSize)
{
    outImageSize = message["ImageSize"];
}

void MessagingProtocol::BuildImageSize(json& outMessage, const int size)
{
    outMessage =
    {
        {"ImageSize", size},
    };
}

//
void MessagingProtocol::AcquireGetUserExercises(const json& message, int& outId)
{
    outId = message["UserId"];
}

void MessagingProtocol::BuildGetUserExercisesReply(json& outMessage, const std::vector<int> exerciseIds)
{
    json idsArray;

    for (const auto& id : exerciseIds)
    {
        idsArray.push_back(id);
    }

    outMessage =
    {
        {"ExerciseIds", idsArray},
    };
}

//
void MessagingProtocol::AcquireGetExerciseData(const json& message, int& outExerciseId)
{
    outExerciseId = message["ExerciseId"];
}

void MessagingProtocol::BuildGetExerciseDataReply(json& outMessage, const QString& dayOfTheWeek, const QString& exerciseName, int duration)
{
    outMessage =
    {
        {"DayOfTheWeek", dayOfTheWeek.toStdString()},
        {"ExerciseName", exerciseName.toStdString()},
        {"Duration", duration},
    };
}


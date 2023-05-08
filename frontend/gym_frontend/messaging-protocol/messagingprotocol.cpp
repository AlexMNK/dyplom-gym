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

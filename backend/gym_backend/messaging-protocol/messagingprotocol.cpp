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
void MessagingProtocol::AcquireGetUserFriends(const json& message, int& outId)
{
    outId = message["UserId"];
}

void MessagingProtocol::BuildGetUserFriendsReply(json& outMessage, const std::vector<int> friendIds)
{
    json idsArray;

    for (const auto& id : friendIds)
    {
        idsArray.push_back(id);
    }

    outMessage =
    {
        {"Friends", idsArray},
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

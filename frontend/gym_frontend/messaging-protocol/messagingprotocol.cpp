#include "messagingprotocol.h"
#include "utils/QByteArrayConverter.hpp"

#include <QDebug>

// Image size

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


// All other messages

void MessagingProtocol::BuildGetUserData(json& outMessage, int userId)
{
    outMessage =
    {
        {"Operation", "GetUserData"},
        {"UserId", userId},
    };
}

void MessagingProtocol::AcquireGetUserDataReply(const json& message, QString& outUserName, QString& outUserPassword)
{
    outUserName = QString::fromStdString(message["Name"]);
    outUserPassword = QString::fromStdString(message["Password"]);
}


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

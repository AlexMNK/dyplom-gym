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

void MessagingProtocol::AcquireGetUserDataReply(const json& message, QString& outUserName, QString& outUserPassword)
{
    outUserName = QString::fromStdString(message["Name"]);
    outUserPassword = QString::fromStdString(message["Password"]);
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

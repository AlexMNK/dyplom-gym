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

void MessagingProtocol::AcquireGetUserData(const json& message, int& outId)
{
    outId = message["UserId"];
}

void MessagingProtocol::BuildReplyGetUserData(json& outMessage, const QString& name, const QString& password)
{
    outMessage =
    {
        {"Name", name.toStdString()},
        {"Password", password.toStdString()},
    };
}

void MessagingProtocol::AcquireUpdateImage(const json& message, int& outId, int& outImageSize)
{
    outId = message["UserId"];
    outImageSize = message["ImageSize"];
}

void MessagingProtocol::BuildReplyUpdateImage(json& outMessage, const bool result)
{
    outMessage =
    {
        {"Result", result},
    };
}

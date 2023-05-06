#ifndef MESSAGINGPROTOCOL_H
#define MESSAGINGPROTOCOL_H

#include <nlohmann/json.hpp>

#include <QString>
#include <QByteArray>

using json = nlohmann::json;

class MessagingProtocol
{
public:
    // Image size
    static void AcquireImageSize(const json& message, int& outImageSize);
    static void BuildImageSize(json& outMessage, const int size);

    // Get user data by Id
    static void BuildGetUserData(json& outMessage, int userId);
    static void AcquireGetUserDataReply(const json& message, QString& outUserName, QString& outUserPassword);

    // Update user image by Id
    static void BuildUpdateImage(json& outMessage, int userId, int imageSize);
    static void AcquireUpdateImageReply(const json& message, bool& outResult);


};

#endif // MESSAGINGPROTOCOL_H

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
    static void AcquireGetUserData(const json& message, int& outId);
    static void BuildReplyGetUserData(json& outMessage, const QString& name, const QString& password);

    // Update user image by Id
    static void AcquireUpdateImage(const json& message, int& outId, int& outImageSize);
    static void BuildReplyUpdateImage(json& outMessage, const bool result);

};

#endif // MESSAGINGPROTOCOL_H

#ifndef MESSAGINGPROTOCOL_H
#define MESSAGINGPROTOCOL_H

#include <nlohmann/json.hpp>

#include <QString>
#include <QByteArray>
#include <vector>

using json = nlohmann::json;

class MessagingProtocol
{
public:
    // Get user id by user email or hash and password
    static void AcquireAuthorize(const json& message, QString& outUserLogin, QString& outUserPassword);
    static void BuildAuthorizeReply(json& outMessage, int userId, const QString& status);

    // Get main user data by Id
    static void AcquireGetUserData(const json& message, int& outId);
    static void BuildGetUserDataReply(json& outMessage, const QString& userName, const QString& userPassword, const QString& userHashtag, const QString& userEmail,
                                          float userMaxBench, float userMaxSquat, float userMaxDeadlift, int userHeight, float userWeight,
                                          int userAge, int userPoints);

    static void AcquireGetUserFriends(const json& message, int& outId);
    static void BuildGetUserFriendsReply(json& outMessage, const std::vector<int> friendIds);

    // Update user image by Id
    static void AcquireUpdateImage(const json& message, int& outId, int& outImageSize);
    static void BuildUpdateImageReply(json& outMessage, const bool result);

    // Image size
    static void AcquireImageSize(const json& message, int& outImageSize);
    static void BuildImageSize(json& outMessage, const int size);
};

#endif // MESSAGINGPROTOCOL_H


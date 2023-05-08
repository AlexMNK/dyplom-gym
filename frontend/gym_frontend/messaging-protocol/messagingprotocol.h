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
    static void BuildAuthorize(json& outMessage, const QString& userLogin, const QString& userPassword);
    static void AcquireAuthorizeReply(const json& message, int& outUserId, QString& outStatus);

    // Get main user data by Id
    static void BuildGetUserData(json& outMessage, int userId);
    static void AcquireGetUserDataReply(const json& message, QString& outUserName, QString& outUserPassword, QString& outUserHashtag, QString& outUserEmail, float& outUserMaxBench, float& outUserMaxSquat, float& outUserMaxDeadlift, int& outUserHeight, float& outUserWeight, int &outUserAge, int &outUserPoints);

    // Get user friends by Id
    static void BuildGetUserFriends(json& outMessage, int userId);
    static void AcquireGetUserFriendsReply(const json& message, std::vector<std::pair<int, QString>>& outIds);

    // Update user image by Id
    static void BuildUpdateImage(json& outMessage, int userId, int imageSize);
    static void AcquireUpdateImageReply(const json& message, bool& outResult);

    // Image size
    static void BuildImageSize(json& outMessage, const int size);
    static void AcquireImageSize(const json& message, int& outImageSize);

};

#endif // MESSAGINGPROTOCOL_H

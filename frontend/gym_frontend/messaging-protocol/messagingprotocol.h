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

    // Get all friend post ids by user id
    static void BuildGetPosts(json& outMessage, int userId);
    static void AcquireGetPostsReply(const json& message, std::vector<int>& outIds);

    // Get all post info by post id
    static void BuildGetPostData(json& outMessage, int postId);
    static void AcquireGetPostDataReply(const json& message, int& outPostUserId, QString& outPostText, QString& outPostTime);

    // Update user image by Id
    static void BuildUpdateImage(json& outMessage, int userId, int imageSize);
    static void AcquireUpdateImageReply(const json& message, bool& outResult);

    // Image size
    static void BuildImageSize(json& outMessage, const int size);
    static void AcquireImageSize(const json& message, int& outImageSize);

    // Get all exercises ids by user id
    static void BuildGetUserExercises(json& outMessage, int userId);
    static void AcquireGetUserExercisesReply(const json& message, std::vector<int>& outIds);

    // Get all exercise info by exercise id
    static void BuildGetExerciseData(json& outMessage, int exerciseId);
    static void AcquireGetExerciseDataReply(const json& message, QString& outDayOfTheWeek, QString& outName, int& outDuration);



};

#endif // MESSAGINGPROTOCOL_H

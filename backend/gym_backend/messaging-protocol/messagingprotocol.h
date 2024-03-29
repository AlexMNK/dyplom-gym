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
    static void BuildGetUserFriendsReply(json& outMessage, const std::vector<std::pair<int, QString>> friendIds);

    // Get all friend post ids by user id
    static void AcquireGetPosts(const json& message, int& outId);
    static void BuildGetPostsReply(json& outMessage, const std::vector<int> postIds);

    // Get all post info by post id
    static void AcquireGetPostData(const json& message, int& outPostId);
    static void BuildGetPostDataReply(json& outMessage, const int postUserId, const QString& postText, const QString& postTime);

    // Update user image by Id
    static void AcquireUpdateImage(const json& message, int& outId, int& outImageSize);
    static void BuildUpdateImageReply(json& outMessage, const bool result);

    // Image size
    static void AcquireImageSize(const json& message, int& outImageSize);
    static void BuildImageSize(json& outMessage, const int size);

    // Get all exercises ids by user id
    static void AcquireGetUserExercises(const json& message, int& outId);
    static void BuildGetUserExercisesReply(json& outMessage, const std::vector<int> exerciseIds);

    // Get all exercise info by exercise id
    static void AcquireGetExerciseData(const json& message, int& outExerciseId);
    static void BuildGetExerciseDataReply(json& outMessage, const QString& dayOfTheWeek, const QString& exerciseName, const QString& statusName, float pointsPerHour, int duration);

    // Get all available exercises
    // no acquire
    static void BuildGetAllExercisesReply(json& outMessage, const std::vector<QString> exercises);

    // Set all user exersises to in progress
    static void AcquireRefreshUserTrainingWeek(const json& message, int& outId);
    // no reply

    // Add a new exercise on a certain day
    static void AcquireAddExercise(const json& message, int& outId, QString& outDayOfTheWeek, QString& outExerciseName, int& outDuration);
    // no reply

    // Delete a user exercise by id
    static void AcquireDeleteExercise(const json& message, int& outExerciseId);
    // no reply

    // Edit a user exercise by id
    static void AcquireEditExercise(const json& message, int& outExerciseId, int& outDuration, int& outStatus);
    // no reply

    // Add a new post
    static void AcquireAddPost(const json& message, int& outUserId, QString& outText, int& outImageSize);
    // no reply

    // Edit post text
    static void AcquireEditPost(const json& message, int& outPostId, QString& outText);
    // no reply

    // Edit post image
    static void AcquireUpdatePostImage(const json& message, int& outPostId, int& outImageSize);
    // no reply

    // Delete post by id
    static void AcquireDeletePost(const json& message, int& outPostId);
    // no reply

    // Sign Up
    static void AcquireSignUp(const json& message, QString& outUserName, QString& outUserEmail, QString& outUserPassword);
    static void BuildSignUpReply(json& outMessage, const QString& status, const QString& newName);

    // UpdateUserInfo
    static void AcquireUpdateUserInfo(const json& message, int& outUserId, QString& outUserName, QString& outUserHashtag, QString& outUserEmail, QString& outUserPassword,
                                        int& outUserAge, int& outUserHeight, float& outUserWeight, float& outUserBench, float& outUserSquat, float& outUserDeadlift);
    // no reply

    // Get Friend requests by user id
    static void AcquireGetUserFriendRequests(const json& message, int& outId);
    static void BuildGetUserFriendRequestsReply(json& outMessage, const std::vector<std::pair<int, int>> friendRequestIds);

    // Reply to a friend request
    static void AcquireReplyFriendRequest(const json& message, int& outRequestId, bool& outStatus);
    // no reply

    // Delete a friend
    static void AcquireDeleteFriend(const json& message, int& outUserId, int& outFriendId);
    // no reply

    // Send a friend request
    static void AcquireAddFriend(const json& message, int& outUserId, QString& outFriendName);
    // no reply
};

#endif // MESSAGINGPROTOCOL_H

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
    static void AcquireGetUserDataReply(const json& message, QString& outUserName, QString& outUserPassword, QString& outUserHashtag, QString& outUserEmail, int& outUserMaxBench, int& outUserMaxSquat, int& outUserMaxDeadlift, int& outUserHeight, int& outUserWeight, int &outUserAge, int &outUserPoints);

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
    static void AcquireGetExerciseDataReply(const json& message, QString& outDayOfTheWeek, QString& outName, QString& outStatus, float& pointsPerHour, int& outDuration);

    // Get all available exercises
    static void BuildGetAllExercises(json& outMessage);
    static void AcquireGetAllExercisesReply(const json& message, std::vector<QString>& outExercises);

    // Set all user exersises to in progress
    static void BuildRefreshUserTrainingWeek(json& outMessage, int userId);
    // no reply

    // Add a new exercise on a certain day
    static void BuildAddExercise(json& outMessage, int userId, const QString& dayOfTheWeek, const QString& exerciseName, int duration);
    // no reply

    // Delete a user exercise by id
    static void BuildDeleteExercise(json& outMessage, int exerciseId);
    // no reply

    // Edit a user exercise by id
    static void BuildEditExercise(json& outMessage, int exerciseId, int duration, int status);
    // no reply

    // Add a new post
    static void BuildAddPost(json& outMessage, int userId, const QString& text, int imageSize);
    // no reply

    // Edit post text
    static void BuildEditPost(json& outMessage, int postId, const QString& text);
    // no reply

    // Edit post image
    static void BuildUpdatePostImage(json& outMessage, int postId, int imageSize);
    // no reply

    // Delete post by id
    static void BuildDeletePost(json& outMessage, int postId);
    // no reply

    // Sign Up
    static void BuildSignUp(json& outMessage, const QString& userName, const QString& userEmail, const QString& userPassword);
    static void AcquireSignUpReply(const json& message, QString& outStatus, QString& outNewName);

    // UpdateUserInfo
    static void BuildUpdateUserInfo(json& outMessage, int userId, const QString& userName, const QString& userHashtag, const QString& userEmail, const QString& userPassword,
                                    int userAge, int userHeight, float userWeight, float userBench, float userSquat, float userDeadlift);
    // no reply
};

#endif // MESSAGINGPROTOCOL_H

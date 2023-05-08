#include "mainuser.h"
#include <QDebug>

MainUser::MainUser(int id)
{
    this->mUserId = id;
}

MainUser::~MainUser()
{
    for (const auto& userFriend : mUserFriends)
    {
        delete userFriend;
    }
}

void MainUser::BuildGetUserDataRequest(json& outMessage)
{
    MessagingProtocol::BuildGetUserData(outMessage, mUserId);
}

void MainUser::AcquireGetUserDataReply(const json& message)
{
    MessagingProtocol::AcquireGetUserDataReply(message, mUserName, mUserPassword, mUserHashtag, mUserEmail, mUserMaxBench,
                                                   mUserMaxSquat, mUserMaxDeadlift, mUserHeight, mUserWeight, mUserAge, mUserPoints);

    qDebug() << mUserName;
    qDebug() << mUserPassword;
    qDebug() << mUserHashtag;
    qDebug() << mUserEmail;
    qDebug() << mUserMaxBench;
    qDebug() << mUserMaxSquat;
    qDebug() << mUserMaxDeadlift;
    qDebug() << mUserHeight;
    qDebug() << mUserWeight;
    qDebug() << mUserAge;
    qDebug() << mUserPoints;
}

const FriendUser* MainUser::GetFriendById(int id)
{
    for (const auto& friendInstance: mUserFriends)
    {
        if (friendInstance->GetUserId() == id)
        {
            return friendInstance;
            break;
        }
    }

    return nullptr;
}


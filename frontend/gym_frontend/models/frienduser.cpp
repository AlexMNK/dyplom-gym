#include "frienduser.h"
#include <QDebug>

FriendUser::FriendUser(int id, QString friendsSince)
{
    this->mUserId = id;
    this->mFriendsSince = friendsSince;
}

void FriendUser::AcquireGetUserDataReply(const json& message, FriendStatuses status)
{
    QString dummyPassword;
    MessagingProtocol::AcquireGetUserDataReply(message, mUserName, dummyPassword, mUserHashtag, mUserEmail, mUserMaxBench,
                                                   mUserMaxSquat, mUserMaxDeadlift, mUserHeight, mUserWeight, mUserAge, mUserPoints);

    qDebug() << "Friend user with id " << mUserId;
    qDebug() << "Friends since " << mFriendsSince;

    qDebug() << mUserName;
    qDebug() << dummyPassword;
    qDebug() << mUserHashtag;
    qDebug() << mUserEmail;
    qDebug() << mUserMaxBench;
    qDebug() << mUserMaxSquat;
    qDebug() << mUserMaxDeadlift;
    qDebug() << mUserHeight;
    qDebug() << mUserWeight;
    qDebug() << mUserAge;
    qDebug() << mUserPoints;

    this->mUserFriendStatus = status;
}

#ifndef FRIENDUSER_H
#define FRIENDUSER_H

#include "IUser.h"
#include "messaging-protocol/messagingprotocol.h"

#include <QString>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum FriendStatuses
{
    FriendStatusRequestedByMe = 0,
    FriendStatusRequestedByHim = 1,
    FriendStatusIsFriend = 2,
};


class FriendUser : public IUser
{
public:
    FriendUser(int id, QString friendsSince);
    FriendStatuses GetUserFriendStatus() { return mUserFriendStatus; }
    QString GetUserFriendFriendsSince() { return mFriendsSince; }

    void AcquireGetUserDataReply(const json& message, FriendStatuses status);

private:
     FriendStatuses mUserFriendStatus;
     QString mFriendsSince;
};

#endif // FRIENDUSER_H

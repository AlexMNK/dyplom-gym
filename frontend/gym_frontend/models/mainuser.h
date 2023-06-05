#ifndef MAINUSER_H
#define MAINUSER_H

#include "IUser.h"
#include "frienduser.h"
#include "messaging-protocol/messagingprotocol.h"

#include <QString>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;


class MainUser : public IUser
{
public:
    MainUser(int id);
    ~MainUser();

    QString GetUserPassword() { return mUserPassword; }
    std::vector<FriendUser*>& GetUserFriends() { return mUserFriends; }
    std::vector<std::pair<FriendUser*, int>>& GetUserFriendRequests() { return mUserFriendRequests; }

    void BuildGetUserDataRequest(json& outMessage);
    void AcquireGetUserDataReply(const json& message);

    const FriendUser* GetFriendById(int id);

private:

    QString mUserPassword;
    std::vector<FriendUser*> mUserFriends;
    std::vector<std::pair<FriendUser*, int>> mUserFriendRequests;
};

#endif // MAINUSER_H

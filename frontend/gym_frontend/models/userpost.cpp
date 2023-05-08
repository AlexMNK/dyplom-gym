#include "userpost.h"
#include <QDebug>
#include "messaging-protocol/messagingprotocol.h"

UserPost::UserPost(int postId)
{
    this->mPostId = postId;
}

void UserPost::AcquireGetPostDataReply(const json& message)
{
    MessagingProtocol::AcquireGetPostDataReply(message, mPostUserId, mPostText, mPostTime);

    qDebug() << "Got post with id " << mPostId;
    qDebug() << mPostUserId;
    qDebug() << mPostText;
    qDebug() << mPostTime;
}

#ifndef USERPOST_H
#define USERPOST_H

#include <QByteArray>
#include <QString>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class UserPost
{
public:
    UserPost(int postId);

    int GetPostId() const { return mPostId; }
    int GetPostUserId() const { return mPostUserId; }
    QString GetPostText() const { return mPostText; }
    QString GetPostTime() const { return mPostTime; }
    QByteArray* GetPostPicture() const { return mPostPicture; }

    void SetPostPicture(QByteArray* postPicture) { mPostPicture = postPicture; }
    void AcquireGetPostDataReply(const json& message);

private:
    int mPostId;
    int mPostUserId;
    QString mPostText;
    QString mPostTime;
    QByteArray* mPostPicture{nullptr};
};

#endif // USERPOST_H

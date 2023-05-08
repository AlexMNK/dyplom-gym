#ifndef IUSER_H
#define IUSER_H

#include <QString>

class IUser
{
public:
    int GetUserId() const { return mUserId; }
    QString GetUserName() const { return mUserName; }
    QString GetUserEmail() const { return mUserEmail; }
    QString GetUserHashtag() const { return mUserHashtag; }
    QByteArray* GetUserPicture() const { return mUserProfilePicture; }
    float GetUserMaxBench() const { return mUserMaxBench; }
    float GetUserMaxSquat() const { return mUserMaxSquat; }
    float GetUserMaxDeadlift() const { return mUserMaxDeadlift; }
    int GetUserHeight() const { return mUserHeight; }
    float GetUserWeight() const { return mUserWeight; }
    int GetUserAge() const { return mUserAge; }
    int GetUserPoints() const { return mUserPoints; }

    void SetUserPicture(QByteArray* pictureData) { mUserProfilePicture = pictureData; }

protected:
    int mUserId;
    QString mUserName;
    QString mUserEmail;
    QString mUserHashtag;
    QByteArray* mUserProfilePicture;
    float mUserMaxBench;
    float mUserMaxSquat;
    float mUserMaxDeadlift;
    int mUserHeight;
    float mUserWeight;
    int mUserAge;
    int mUserPoints;
};

#endif // IUSER_H

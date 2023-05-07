#ifndef IUSER_H
#define IUSER_H

#include <QString>

class IUser
{
public:
    int GetUserId() { return mUserId; }
    QString GetUserName() { return mUserName; }
    QString GetUserEmail() { return mUserEmail; }
    QString GetUserHashtag() { return mUserHashtag; }
    QByteArray* GetUserPicture() { return mUserProfilePicture; }
    float GetUserMaxBench() { return mUserMaxBench; }
    float GetUserMaxSquat() { return mUserMaxSquat; }
    float GetUserMaxDeadlift() { return mUserMaxDeadlift; }
    int GetUserHeight() { return mUserHeight; }
    float GetUserWeight() { return mUserWeight; }
    int GetUserAge() { return mUserAge; }
    int GetUserPoints() { return mUserPoints; }

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

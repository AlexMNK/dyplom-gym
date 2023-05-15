#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "models/frienduser.h"

void MainWindow::PerformGetUserDataOperation()
{
    int imageSize;
    json getMainUserDataMessage;

    mCurrentUser->BuildGetUserDataRequest(getMainUserDataMessage);

    mClientInstance->SendDataToServer(getMainUserDataMessage);                                                                       // SEND user id

    const json serverImageSize = mClientInstance->ReceiveDataFromServer();                                                          // RCV image size

    MessagingProtocol::AcquireImageSize(serverImageSize, imageSize);

    mCurrentUser->SetUserPicture(&DataPartImageHelper::ReceiveImageByParts(mClientInstance->GetSocketConnection(), imageSize));     // RCV image by parts

    const json serverReply = mClientInstance->ReceiveDataFromServer();                                                              // RCV all remaining user data

    mCurrentUser->AcquireGetUserDataReply(serverReply);

    FillCurentUserDataFields();
}

void MainWindow::PerformGetUserFriendsOperation()
{
    mCurrentUser->GetUserFriends().clear(); // clr user friends before assigning again

    std::vector<std::pair<int, QString>> userFriends;

    json getUserFriends;
    MessagingProtocol::BuildGetUserFriends(getUserFriends, mCurrentUser->GetUserId());

    mClientInstance->SendDataToServer(getUserFriends);                                                      // SEND user id

    const json serverFriendsIds = mClientInstance->ReceiveDataFromServer();                                 // RCV friend ids

    MessagingProtocol::AcquireGetUserFriendsReply(serverFriendsIds, userFriends);

    for (const auto& friendInstance : userFriends) // cycle to get all friends data
    {
        int imageSize;
        json getUserDataMessage;
        MessagingProtocol::BuildGetUserData(getUserDataMessage, friendInstance.first);

        mClientInstance->SendDataToServer(getUserDataMessage);                                                                         // SEND friend id

        const json serverImageSize = mClientInstance->ReceiveDataFromServer();                                                         // RCV image size

        MessagingProtocol::AcquireImageSize(serverImageSize, imageSize);
        FriendUser* friendUser = new FriendUser(friendInstance.first, friendInstance.second); // first - id, second - friends since

        friendUser->SetUserPicture(&DataPartImageHelper::ReceiveImageByParts(mClientInstance->GetSocketConnection(), imageSize));      // RCV image by parts

        const json serverReply = mClientInstance->ReceiveDataFromServer();                                                             // RCV all remaining friend data

        friendUser->AcquireGetUserDataReply(serverReply, FriendStatusIsFriend);
        mCurrentUser->GetUserFriends().push_back(friendUser);
    }

    qDebug() << "friendlist size";
    qDebug() << mCurrentUser->GetUserFriends().size();

    FillFriendList();
}

void MainWindow::PerformUpdateUserImageOperation()
{
    const QString file_name = QFileDialog::getOpenFileName(this, "Choose image");

    QImageReader imageReader{file_name};

    QImage image = imageReader.read();

    if(image.isNull())
    {
         QMessageBox::warning(this, "Warning", "Failed to load image");
         return;
    }

    QByteArray* byteImage = new QByteArray();
    QBuffer buffer(byteImage);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");                                    // todo: png

    int imageSize = byteImage->size();

    json updateImageMessage;
    MessagingProtocol::BuildUpdateImage(updateImageMessage, mCurrentUser->GetUserId(), imageSize);

    mClientInstance->SendDataToServer(updateImageMessage);                                      // SEND user id and image size

    const json serverReply2 = mClientInstance->ReceiveDataFromServer();                         // RCV for sync

    DataPartImageHelper::SendImageByParts(mClientInstance->GetSocketConnection(), *byteImage);  // SEND img by parts

    const json serverReply = mClientInstance->ReceiveDataFromServer();                          // RCV status

    bool Result = false;
    MessagingProtocol::AcquireUpdateImageReply(serverReply, Result);

    if (Result)
    {
        mCurrentUser->SetUserPicture(byteImage);
    }
}

void MainWindow::PerformGetPostsOperation()
{
    mPosts.clear(); // clear prev posts before loading once again

    std::vector<int> postIds;

    json getPosts;
    MessagingProtocol::BuildGetPosts(getPosts, mCurrentUser->GetUserId());

    mClientInstance->SendDataToServer(getPosts);                                 // SEND user id to get posts ids

    const json serverReply = mClientInstance->ReceiveDataFromServer();           // RCV post ids

    MessagingProtocol::AcquireGetPostsReply(serverReply, postIds);

    for (const auto& postId : postIds)
    {
        int imageSize;
        json getPostDataMessage;
        MessagingProtocol::BuildGetPostData(getPostDataMessage, postId);

        mClientInstance->SendDataToServer(getPostDataMessage);                                                                          // SEND post id

        const json serverImageSize = mClientInstance->ReceiveDataFromServer();                                                           // RCV post image size

        MessagingProtocol::AcquireImageSize(serverImageSize, imageSize);
        UserPost* userPost = new UserPost(postId);

        userPost->SetPostPicture(&DataPartImageHelper::ReceiveImageByParts(mClientInstance->GetSocketConnection(), imageSize));         // RCV image by parts

        const json serverReply = mClientInstance->ReceiveDataFromServer();                                                              // RCV all remaining post data

        userPost->AcquireGetPostDataReply(serverReply);
        mPosts.push_back(userPost);
    }

    FillPostsList();
}

void MainWindow::PerformGetUserExercises()
{
    mExercises.clear(); // clear prev exercises before loading once again

    std::vector<int> exerciseIds;

    json getExercises;
    MessagingProtocol::BuildGetUserExercises(getExercises, mCurrentUser->GetUserId());

    mClientInstance->SendDataToServer(getExercises);                                 // SEND user id to get exercise ids

    const json serverReply = mClientInstance->ReceiveDataFromServer();              // RCV exercise ids

    MessagingProtocol::AcquireGetUserExercisesReply(serverReply, exerciseIds);

    for (const auto& exerciseId : exerciseIds)
    {
        json getExerciseDataMessage;
        MessagingProtocol::BuildGetExerciseData(getExerciseDataMessage, exerciseId);

        mClientInstance->SendDataToServer(getExerciseDataMessage);                  // SEND exercise id

        const json exerciseData = mClientInstance->ReceiveDataFromServer();         // RCV all exercise data

        Exercise* userExercise = new Exercise(mCurrentUser->GetUserId(), exerciseId);

        userExercise->AcquireGetExerciseDataReply(exerciseData);
        mExercises.push_back(userExercise);
    }

    qDebug() << "number of exercises in a week " << mExercises.size();

    FillTrainingLists();
}



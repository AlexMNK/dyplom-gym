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

    QPixmap pixmap;
    bool loadImage = pixmap.loadFromData(QByteArray::fromHex(*mCurrentUser->GetUserPicture()));

    if (!loadImage)
    {
        QMessageBox::warning(this, "Warning", "Failed to get image data from server");
    }

    ui->image->setPixmap(pixmap);

    const json serverReply = mClientInstance->ReceiveDataFromServer();                                                              // RCV all remaining user data

    mCurrentUser->AcquireGetUserDataReply(serverReply);
}

void MainWindow::PerformGetUserFriendsOperation()
{
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

    mClientInstance->SendDataToServer(updateImageMessage);                                      // SEND user if and image size

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

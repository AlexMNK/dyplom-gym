#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::PerformGetUserDataOperation()
{
    int imageSize = 0;
    QString userName, UserPassword;
    QByteArray userPicture;

    json getUserDataMessage;
    MessagingProtocol::BuildGetUserData(getUserDataMessage, mUserId);

    mClientInstance->SendDataToServer(getUserDataMessage);                                                          // SEND user id

    const json serverImageSize = mClientInstance->ReceiveDataFromServer();                                          // RCV image size

    MessagingProtocol::AcquireImageSize(serverImageSize, imageSize);

    userPicture = DataPartImageHelper::ReceiveImageByParts(mClientInstance->GetSocketConnection(), imageSize);      // RCV image by parts

    QPixmap pixmap;
    bool loadImage = pixmap.loadFromData(QByteArray::fromHex(userPicture));

    if (!loadImage)
    {
        QMessageBox::warning(this, "Warning", "Failed to get image data from server");
    }

    ui->image->setPixmap(pixmap);

    const json serverReply = mClientInstance->ReceiveDataFromServer();                                              // RCV user name and password

    MessagingProtocol::AcquireGetUserDataReply(serverReply, userName, UserPassword);

    ui->name->setText(userName);
    ui->password->setText(UserPassword);
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

    QByteArray byteImage;
    QBuffer buffer(&byteImage);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");                                    // todo: png

    int imageSize = byteImage.size();

    json updateImageMessage;
    MessagingProtocol::BuildUpdateImage(updateImageMessage, mUserId, imageSize);

    mClientInstance->SendDataToServer(updateImageMessage);                                      // SEND user if and image size

    const json serverReply2 = mClientInstance->ReceiveDataFromServer();                         // RCV for sync

    DataPartImageHelper::SendImageByParts(mClientInstance->GetSocketConnection(), byteImage);   // SEND img by parts

    const json serverReply = mClientInstance->ReceiveDataFromServer();                          // RCV status

    bool Result = false;
    MessagingProtocol::AcquireUpdateImageReply(serverReply, Result);
}

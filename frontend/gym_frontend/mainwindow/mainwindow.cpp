#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AuthorizationSuccess(Client* clientInstance, int userId)
{
    int imageSize = 0;
    QString userName, UserPassword;
    QByteArray userPicture;

    this->mClientInstance = clientInstance;
    mClientInstance->SetCurrentWindow(this);
    this->mUserId = userId;

    json getUserDataMessage;
    MessagingProtocol::BuildGetUserData(getUserDataMessage, userId);

    mClientInstance->SendDataToServer(getUserDataMessage); // send user id

    const json serverImageSize = mClientInstance->ReceiveDataFromServer(); // rcv image size

    MessagingProtocol::AcquireImageSize(serverImageSize, imageSize);

    qDebug() << "Image Size";
    qDebug() << imageSize;

    userPicture = DataPartImageHelper::ReceiveImageByParts(mClientInstance->GetSocketConnection(), imageSize); // rcv image by parts

    QPixmap pixmap;
    bool res = pixmap.loadFromData(QByteArray::fromHex(userPicture));
    qDebug() << res;
    ui->image->setPixmap(pixmap);

    const json serverReply = mClientInstance->ReceiveDataFromServer(); // rcv user name and password

    MessagingProtocol::AcquireGetUserDataReply(serverReply, userName, UserPassword);

    ui->name->setText(userName);
    ui->password->setText(UserPassword);
}

void MainWindow::on_pushButton_clicked()
{
    emit BackToAuthorization();
}


void MainWindow::on_Gay_button_clicked()
{
    const json GayJson =
    {
        {"Operation", "GAY"},
    };

    mClientInstance->SendDataToServer(GayJson);

    const json serverReply = mClientInstance->ReceiveDataFromServer();

    const QString result = QString::fromStdString(serverReply["GAY"]);
    QMessageBox::information(this, "Data", result);

}


void MainWindow::on_update_img_clicked()
{
    const QString file_name = QFileDialog::getOpenFileName(this, "Choose image");
    qDebug() << file_name;

    QImageReader imageReader{file_name};
    qDebug() << imageReader.canRead();

    QImage image = imageReader.read();

    if(!image.isNull())
    {
        QByteArray byteImage;
        QBuffer buffer(&byteImage);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "JPEG");

        qDebug() << byteImage.size();

        int imageSize = byteImage.size();

        json updateImageMessage;
        MessagingProtocol::BuildUpdateImage(updateImageMessage, mUserId, imageSize);

        mClientInstance->SendDataToServer(updateImageMessage); // send image info

        const json serverReply2 = mClientInstance->ReceiveDataFromServer();

        const QString result = QString::fromStdString(serverReply2["GAY2"]);
        QMessageBox::information(this, "Data", result);

        DataPartImageHelper::SendImageByParts(mClientInstance->GetSocketConnection(), byteImage); // send img by parts

        const json serverReply = mClientInstance->ReceiveDataFromServer(); // receive status

        bool Result = false;
        MessagingProtocol::AcquireUpdateImageReply(serverReply, Result);

        qDebug() << "Server answered " << Result;
    }
}


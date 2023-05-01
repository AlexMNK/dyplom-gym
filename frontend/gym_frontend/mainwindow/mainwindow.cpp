#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    this->mClientInstance = clientInstance;
    mClientInstance->SetCurrentWindow(this);
    this->mUserId = userId;

    json getUserDataMessage;
    MessagingProtocol::BuildGetUserData(getUserDataMessage, userId);

    mClientInstance->SendDataToServer(getUserDataMessage);

    const json serverReply = mClientInstance->ReceiveDataFromServer();

    QString userName, UserPassword;
    QByteArray userPicture;
    MessagingProtocol::AcquireGetUserDataReply(serverReply, userName, UserPassword, userPicture);

    ui->name->setText(userName);
    ui->password->setText(UserPassword);

    QPixmap pixmap;
    bool res = pixmap.loadFromData(userPicture);
    qDebug() << res;
    ui->image->setPixmap(pixmap);
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

        qDebug() << byteImage;

        json updateImageMessage;
        MessagingProtocol::BuildUpdateImage(updateImageMessage, mUserId, byteImage);

        mClientInstance->SendDataToServer(updateImageMessage);

        const json serverReply = mClientInstance->ReceiveDataFromServer();

        bool Result = false;
        MessagingProtocol::AcquireUpdateImageReply(serverReply, Result);

        qDebug() << "Server answered " << Result;
    }
}


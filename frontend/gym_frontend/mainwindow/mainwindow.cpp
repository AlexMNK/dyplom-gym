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


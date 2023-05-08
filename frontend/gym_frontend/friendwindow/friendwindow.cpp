#include "friendwindow.h"
#include "ui_friendwindow.h"

#include <QDebug>
#include <QMessageBox>

FriendWindow::FriendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FriendWindow)
{
    ui->setupUi(this);
}

FriendWindow::~FriendWindow()
{
    delete ui;
}

void FriendWindow::FriendWindowOpened(Client* clientInstance, MainUser* mainUser, FriendUser* friendUser)
{
    this->mClientInstance = clientInstance;
    this->mCurrentUser = mainUser;
    this->mFriendInstance = friendUser;
    mClientInstance->SetCurrentWindow(this);

    ui->name->setText(mFriendInstance->GetUserName());
    ui->hashtag->setText(mFriendInstance->GetUserHashtag());
    ui->friendsSince->setText(mFriendInstance->GetUserFriendFriendsSince());
    ui->bench->setText(QString::number(mFriendInstance->GetUserMaxBench()));

    QPixmap pixmap;
    bool loadImage = pixmap.loadFromData(QByteArray::fromHex(*mFriendInstance->GetUserPicture()));

    if (!loadImage)
    {
        QMessageBox::warning(this, "Warning", "Failed load user image");
    }

    ui->image->setPixmap(pixmap);
}

void FriendWindow::on_backButton_clicked()
{
    emit BackToMainWindow();
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetupUiDesign();

    HideMyProfileSection();
    HideTrainingSection();
    ShowPostsSection();

    connect(ui->friendList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnFriendClicked(QListWidgetItem*)));
    connect(ui->postsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnPostClicked(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mCurrentUser;
    delete mFriendWindow;
    delete mPostWindow;

    for (const auto& post : mPosts)
    {
        delete post;
    }

    for (const auto& exercise : mExercises)
    {
        delete exercise;
    }
}

void MainWindow::AuthorizationSuccess(Client* clientInstance, int userId)
{
    this->mClientInstance = clientInstance;
    mClientInstance->SetCurrentWindow(this);
    this->mCurrentUser = new MainUser(userId);

    PerformGetUserDataOperation();
    PerformGetUserFriendsOperation();
    PerformGetPostsOperation();
    PerformGetUserExercises();
}

void MainWindow::on_signOutButton_clicked()
{
    emit BackToAuthorization();
}

void MainWindow::on_userEditProfile_clicked()
{
    if (mIsEditProfileButton) // if edit
    {
        ui->userEditProfile->setText("Cancel");
        ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
        mIsEditProfileButton = false;
        MakeProfileFieldsEdit();
    }
    else // if cancel
    {
        ui->userEditProfile->setText("Edit profile");
        ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
        mIsEditProfileButton = true;
        MakeProfileFieldsNonEdit();
        FillCurentUserDataFields();
    }
}


void MainWindow::on_userSaveProfile_clicked()
{
    // todo - UPDATE operation

    ui->userEditProfile->setText("Edit profile");
    ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    mIsEditProfileButton = true;
    MakeProfileFieldsNonEdit();
    FillCurentUserDataFields();
}


void MainWindow::on_userEditPicture_clicked()
{
    PerformUpdateUserImageOperation();
    PerformGetUserDataOperation();

    ui->userEditProfile->setText("Edit profile");
    ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    mIsEditProfileButton = true;
    MakeProfileFieldsNonEdit();
}


void MainWindow::on_postsButton_clicked()
{
    HideMyProfileSection();
    HideTrainingSection();

    ShowPostsSection();
}


void MainWindow::on_myProfileButton_clicked()
{
    HideTrainingSection();
    HidePostsSection();

    ShowMyProfileSection();
}


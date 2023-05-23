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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mCurrentUser;

    if (mIsFriendWindowCreated)
    {
       delete mFriendWindow;
    }

    if (mIsPostWindowCreated)
    {
        delete mPostWindow;
    }

    if (mIsExerciseWindowCreated)
    {
        delete mExerciseWindow;
    }

    if (mIsAddExerciseWindowCreated)
    {
        delete mAddExerciseWindow;
    }

    if (mIsAddPostWindowCreated)
    {
        delete mAddPostWindow;
    }

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

    HideMyProfileSection();
    HideTrainingSection();

    ShowPostsSection();
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

    FillPostsList(); // to update icons in posts
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


void MainWindow::on_myTrainingButton_clicked()
{
    HideMyProfileSection();
    HidePostsSection();

    ShowTrainingSection();
}

void MainWindow::on_trainingAddMonday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Monday");
    mAddExerciseWindow->show();
}

void MainWindow::on_trainingAddTuesday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Tuesday");
    mAddExerciseWindow->show();
}

void MainWindow::on_trainingAddWednesday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Wednesday");
    mAddExerciseWindow->show();
}

void MainWindow::on_trainingAddThursday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Thursday");
    mAddExerciseWindow->show();
}

void MainWindow::on_trainingAddFriday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Friday");
    mAddExerciseWindow->show();
}

void MainWindow::on_trainingAddSaturday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Saturday");
    mAddExerciseWindow->show();
}

void MainWindow::on_trainingAddSunday_clicked()
{
    CreateAddExerciseWindow();
    emit OpenAddExerciseWindow(mClientInstance, mCurrentUser->GetUserId(), "Sunday");
    mAddExerciseWindow->show();
}


void MainWindow::on_addPostButton_clicked()
{
    CreateAddPostWindow();
    emit OpenAddPostWindow(mClientInstance, mCurrentUser->GetUserId());
    mAddPostWindow->show();
}


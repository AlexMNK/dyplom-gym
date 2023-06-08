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

    if (mIsAddFriendWindowCreated)
    {
        delete mAddFriendWindow;
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
    PerformGetUserFriendRequestsOperation();
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
    bool ifInvalidFields = false;

    if (ui->userNameEdit->text().isEmpty() ||
        ui->userHashEdit->text().isEmpty() ||
        ui->userEmailEdit->text().isEmpty() ||
        ui->userPasswordEdit->text().isEmpty())
    {
         QMessageBox::warning(this, "Invalid data", "You cannot leave user fields empty!");
         ifInvalidFields = true;
    }

    if (ui->userHashEdit->text()[0] != '@' &&  !ui->userHashEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Invalid data", "Hashtag should stard with @ character!");
        ifInvalidFields = true;
    }

    const QRegularExpression regex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption);
    if(!regex.match(ui->userEmailEdit->text()).hasMatch() && !ui->userEmailEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Invalid data", "Invalid format of an email!");
        ifInvalidFields = true;
    }

    if (ifInvalidFields) // reset fields and do not perform operation
    {
        ui->userEditProfile->setText("Edit profile");
        ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
        mIsEditProfileButton = true;
        MakeProfileFieldsNonEdit();
        FillCurentUserDataFields();
    }
    else // send update request
    {
        int age = ui->userAgeEdit->text().isEmpty() ? 0 : ui->userAgeEdit->text().toInt();
        int height = ui->userHeightEdit->text().isEmpty() ? 0 : ui->userHeightEdit->text().toInt();
        float weight = ui->userWeightEdit->text().isEmpty() ? 0 : ui->userWeightEdit->text().toFloat();
        float bench = ui->userBenchEdit->text().isEmpty() ? 0 : ui->userBenchEdit->text().toFloat();
        float squat = ui->userSquatEdit->text().isEmpty() ? 0 : ui->userSquatEdit->text().toFloat();
        float deadlift = ui->userDeadliftEdit->text().isEmpty() ? 0 : ui->userDeadliftEdit->text().toFloat();

        json updateUserInfo;

        MessagingProtocol::BuildUpdateUserInfo(updateUserInfo, mCurrentUser->GetUserId(), ui->userNameEdit->text(), ui->userHashEdit->text(), ui->userEmailEdit->text(),
                                               ui->userPasswordEdit->text(), age, height, weight, bench, squat, deadlift);

        mClientInstance->SendDataToServer(updateUserInfo);                      // SEND update request

        const json reply = mClientInstance->ReceiveDataFromServer();            // RCV status

        if (QString::fromStdString(reply["Status"]) == "NameExists")
        {
            QMessageBox::information(this, "Update data", "User Profile was updated except for user name as the same name already exists");
        }
        else if (QString::fromStdString(reply["Status"]) == "HashExists")
        {
            QMessageBox::information(this, "Update data", "User Profile was updated except for user hashtag as the same hashtag already exists");
        }
        else if (QString::fromStdString(reply["Status"]) == "NameHashExists")
        {
            QMessageBox::information(this, "Update data", "User Profile was updated except for user name and hashtag as the same name and hashtag already exist");
        }
        else if (QString::fromStdString(reply["Status"]) == "Error")
        {
            QMessageBox::warning(this, "Failure", "Failed to update User Profile");
            return;
        }
        else // OK
        {
            QMessageBox::information(this, "Update data", "User Profile was successfully updated!");
        }

        ui->userEditProfile->setText("Edit profile");
        ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
        mIsEditProfileButton = true;
        MakeProfileFieldsNonEdit();
        PerformGetUserDataOperation();
    }
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

void MainWindow::on_addFriend_clicked()
{
    CreateAddFriendWindow();
    emit OpenAddFriendWindow(mClientInstance);
    mAddFriendWindow->show();
}


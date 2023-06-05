#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QFontMetrics>
#include <QDate>

#include "utils/inihelper.h"
#include "models/frienduser.h"


void MainWindow::SetupUiDesign()
{
    // side buttons
    ui->myTrainingButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->myProfileButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->postsButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->addPostButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->signOutButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->addFriend->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");

    // my profile fields
    ui->userNameLabel->setStyleSheet("border: none;");
    ui->userHashLabel->setStyleSheet("border: none;");
    ui->userEmailLabel->setStyleSheet("border: none;");
    ui->userPassLabel->setStyleSheet("border: none;");
    ui->userPointsLabel->setStyleSheet("border: none;");
    ui->userRankLabel->setStyleSheet("border: none;");
    ui->userRankProfile->setStyleSheet("border: none;");
    ui->userHeightLabel->setStyleSheet("border: none;");
    ui->userWeightLabel->setStyleSheet("border: none;");
    ui->userAgeLabel->setStyleSheet("border: none;");
    ui->userBenchLabel->setStyleSheet("border: none;");
    ui->userSquatLabel->setStyleSheet("border: none;");
    ui->userDeadliftLabel->setStyleSheet("border: none;");

    ui->userEditPicture->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->userEditProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->userSaveProfile->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");

    ui->userHeightEdit->setValidator( new QIntValidator(0, 250, this) );
    ui->userWeightEdit->setValidator( new QIntValidator(0, 500, this) );
    ui->userAgeEdit->setValidator( new QIntValidator(0, 120, this) );
    ui->userBenchEdit->setValidator( new QIntValidator(0.0, 1000, this) );
    ui->userSquatEdit->setValidator( new QIntValidator(0.0, 1000, this) );
    ui->userDeadliftEdit->setValidator( new QIntValidator(0.0, 1000, this) );

    MakeProfileFieldsNonEdit();

    this->setStyleSheet("background-color: white;");

    //exercises
    ui->trainingAddMonday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->trainingAddTuesday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->trainingAddWednesday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->trainingAddThursday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->trainingAddFriday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->trainingAddSaturday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->trainingAddSunday->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");

    // signals
    connect(ui->friendList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnFriendClicked(QListWidgetItem*)));
    connect(ui->postsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnPostClicked(QListWidgetItem*)));

    connect(ui->traningMondayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnMondayExerciseClicked(QListWidgetItem*)));
    connect(ui->traningTuesdayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnTuesdayExerciseClicked(QListWidgetItem*)));
    connect(ui->traningWednesdayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnWednesdayExerciseClicked(QListWidgetItem*)));
    connect(ui->traningThursdayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnThursdayExerciseClicked(QListWidgetItem*)));
    connect(ui->traningFridayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnFridayExerciseClicked(QListWidgetItem*)));
    connect(ui->traningSaturdayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnSaturdayExerciseClicked(QListWidgetItem*)));
    connect(ui->traningSundayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnSundayExerciseClicked(QListWidgetItem*)));
}

void MainWindow::FillCurentUserDataFields()
{
    QPixmap pixmap;

    if (mCurrentUser->GetUserPicture() == nullptr)
    {
        pixmap = QPixmap(":/img/noimage.jpg");
    }
    else
    {
        bool loadImage = pixmap.loadFromData(QByteArray::fromHex(*mCurrentUser->GetUserPicture()));

        if (!loadImage)
        {
            QMessageBox::warning(this, "Warning", "Failed to get image data from server");
        }
    }

    // side panel
    ui->userImage->setPixmap(pixmap);
    ui->userRankImage->setPixmap(GetProperRankIcon(mCurrentUser->GetUserPoints()));
    ui->userName->setText(mCurrentUser->GetUserName());
    ui->userPoints->setText("Points: " + QString::number(mCurrentUser->GetUserPoints()));

    // my profile
    ui->userProfilePicture->setPixmap(pixmap);
    ui->userRankProfile->setPixmap(GetProperRankIcon(mCurrentUser->GetUserPoints()));
    ui->userNameEdit->setText(mCurrentUser->GetUserName());
    ui->userHashEdit->setText(mCurrentUser->GetUserHashtag());
    ui->userEmailEdit->setText(mCurrentUser->GetUserEmail());
    ui->userPasswordEdit->setText(mCurrentUser->GetUserPassword());
    ui->userPointsLabel->setText("Points: " + QString::number(mCurrentUser->GetUserPoints()));
    ui->userRankLabel->setText("Rank: " + GetProperRankName(mCurrentUser->GetUserPoints()));
    ui->userHeightEdit->setText(QString::number(mCurrentUser->GetUserHeight()) + " sm");
    ui->userWeightEdit->setText(QString::number(mCurrentUser->GetUserWeight()) + " kg");
    ui->userAgeEdit->setText(mCurrentUser->GetUserAge() != 0 ? QString::number(mCurrentUser->GetUserAge()) : "Not stated");
    ui->userBenchEdit->setText(mCurrentUser->GetUserMaxBench() != 0 ? QString::number(mCurrentUser->GetUserMaxBench()) + " kg" : "Not stated");
    ui->userSquatEdit->setText(mCurrentUser->GetUserMaxSquat() != 0 ? QString::number(mCurrentUser->GetUserMaxSquat()) + " kg" : "Not stated");
    ui->userDeadliftEdit->setText(mCurrentUser->GetUserMaxDeadlift() != 0 ? QString::number(mCurrentUser->GetUserMaxDeadlift()) + " kg" : "Not stated");
}

QPixmap MainWindow::GetProperRankIcon(int points)
{
    if (points < 2000) return QPixmap(":/ranks/ranks/silver.png");
    if (points < 5000) return QPixmap(":/ranks/ranks/gold.png");
    if (points < 10000) return QPixmap(":/ranks/ranks/platinum.png");
    if (points < 20000) return QPixmap(":/ranks/ranks/diamond.png");
    // else it is immortal
    return QPixmap(":/ranks/ranks/immortal.png");
}

QString MainWindow::GetProperRankName(int points)
{
    if (points < 2000) return "Silver";
    if (points < 5000) return "Gold";
    if (points < 10000) return "Platinum";
    if (points < 20000) return "Diamond";
    // else it is immortal
    return "Imoortal";
}

void MainWindow::ShowPostsSection()
{
    ui->postsList->show();
    this->setWindowTitle("Posts");
    ui->postsButton->setStyleSheet("QPushButton {border: 1px solid darkgreen; }");
}

void MainWindow::ShowMyProfileSection()
{
    ui->userProfileFrame->show();
    this->setWindowTitle("My profile");
    ui->myProfileButton->setStyleSheet("QPushButton {border: 1px solid darkgreen; }");
}

void MainWindow::ShowTrainingSection()
{
    ui->traningMondayList->show();
    ui->traningMondayLabel->show();
    ui->traningTuesdayList->show();
    ui->traningTuesdayLabel->show();
    ui->traningWednesdayList->show();
    ui->traningWednesdayLabel->show();
    ui->traningThursdayList->show();
    ui->traningThursdayLabel->show();
    ui->traningFridayList->show();
    ui->traningFridayLabel->show();
    ui->traningSaturdayList->show();
    ui->traningSaturdayLabel->show();
    ui->traningSundayList->show();
    ui->traningSundayLabel->show();

    ui->trainingAddMonday->show();
    ui->trainingAddTuesday->show();
    ui->trainingAddWednesday->show();
    ui->trainingAddThursday->show();
    ui->trainingAddFriday->show();
    ui->trainingAddSaturday->show();
    ui->trainingAddSunday->show();

    this->setWindowTitle("Training program");
    ui->myTrainingButton->setStyleSheet("QPushButton {border: 1px solid darkgreen; }");
}

void MainWindow::HidePostsSection()
{
    ui->postsList->hide();
    ui->postsButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
}

void MainWindow::HideMyProfileSection()
{
    ui->userProfileFrame->hide();
    ui->myProfileButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
}

void MainWindow::HideTrainingSection()
{
    ui->traningMondayList->hide();
    ui->traningMondayLabel->hide();
    ui->traningTuesdayList->hide();
    ui->traningTuesdayLabel->hide();
    ui->traningWednesdayList->hide();
    ui->traningWednesdayLabel->hide();
    ui->traningThursdayList->hide();
    ui->traningThursdayLabel->hide();
    ui->traningFridayList->hide();
    ui->traningFridayLabel->hide();
    ui->traningSaturdayList->hide();
    ui->traningSaturdayLabel->hide();
    ui->traningSundayList->hide();
    ui->traningSundayLabel->hide();

    ui->trainingAddMonday->hide();
    ui->trainingAddTuesday->hide();
    ui->trainingAddWednesday->hide();
    ui->trainingAddThursday->hide();
    ui->trainingAddFriday->hide();
    ui->trainingAddSaturday->hide();
    ui->trainingAddSunday->hide();

    ui->myTrainingButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
}

void MainWindow::MakeProfileFieldsEdit()
{
    ui->userNameEdit->setStyleSheet("border: 1px solid black;");
    ui->userHashEdit->setStyleSheet("border: 1px solid black;");
    ui->userEmailEdit->setStyleSheet("border: 1px solid black;");
    ui->userPasswordEdit->setStyleSheet("border: 1px solid black;");
    ui->userHeightEdit->setStyleSheet("border: 1px solid black;");
    ui->userWeightEdit->setStyleSheet("border: 1px solid black;");
    ui->userAgeEdit->setStyleSheet("border: 1px solid black;");
    ui->userBenchEdit->setStyleSheet("border: 1px solid black;");
    ui->userSquatEdit->setStyleSheet("border: 1px solid black;");
    ui->userDeadliftEdit->setStyleSheet("border: 1px solid black");

    ui->userNameEdit->setEnabled(true);
    ui->userHashEdit->setEnabled(true);
    ui->userEmailEdit->setEnabled(true);
    ui->userPasswordEdit->setEnabled(true);
    ui->userHeightEdit->setEnabled(true);
    ui->userWeightEdit->setEnabled(true);
    ui->userAgeEdit->setEnabled(true);
    ui->userBenchEdit->setEnabled(true);
    ui->userSquatEdit->setEnabled(true);
    ui->userDeadliftEdit->setEnabled(true);

    ui->userHeightEdit->setText(QString::number(mCurrentUser->GetUserHeight()));
    ui->userWeightEdit->setText(QString::number(mCurrentUser->GetUserWeight()));
    ui->userAgeEdit->setText(QString::number(mCurrentUser->GetUserAge()));
    ui->userBenchEdit->setText(QString::number(mCurrentUser->GetUserMaxBench()));
    ui->userSquatEdit->setText(QString::number(mCurrentUser->GetUserMaxSquat()));
    ui->userDeadliftEdit->setText(QString::number(mCurrentUser->GetUserMaxDeadlift()));

    ui->userSaveProfile->setEnabled(true);
}

void MainWindow::MakeProfileFieldsNonEdit()
{
    ui->userNameEdit->setStyleSheet("border: none;");
    ui->userHashEdit->setStyleSheet("border: none;");
    ui->userEmailEdit->setStyleSheet("border: none;");
    ui->userPasswordEdit->setStyleSheet("border: none;");
    ui->userHeightEdit->setStyleSheet("border: none;");
    ui->userWeightEdit->setStyleSheet("border: none;");
    ui->userAgeEdit->setStyleSheet("border: none;");
    ui->userBenchEdit->setStyleSheet("border: none;");
    ui->userSquatEdit->setStyleSheet("border: none;");
    ui->userDeadliftEdit->setStyleSheet("border: none;");

    ui->userNameEdit->setEnabled(false);
    ui->userHashEdit->setEnabled(false);
    ui->userEmailEdit->setEnabled(false);
    ui->userPasswordEdit->setEnabled(false);
    ui->userHeightEdit->setEnabled(false);
    ui->userWeightEdit->setEnabled(false);
    ui->userAgeEdit->setEnabled(false);
    ui->userBenchEdit->setEnabled(false);
    ui->userSquatEdit->setEnabled(false);
    ui->userDeadliftEdit->setEnabled(false);

    ui->userSaveProfile->setEnabled(false);
}

void MainWindow::FillFriendList()
{
    ui->friendList->clear();

    for (const auto& friendInstance : mCurrentUser->GetUserFriends())
    {
        if (friendInstance->GetUserFriendStatus() == FriendUser::FriendStatusIsFriend)
        {
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(friendInstance->GetUserName() + " " + friendInstance->GetUserHashtag()); //newItem->setIcon(QIcon(":/Img/Ranks/silver.png"));
            ui->friendList->addItem(newItem);
        }
    }
}

void MainWindow::FillFriendRequestsList()
{
    ui->friendList_2->clear();

    for (const auto& friendRequestInstance : mCurrentUser->GetUserFriendRequests())
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(friendRequestInstance.first->GetUserName() + " " + friendRequestInstance.first->GetUserHashtag());
        ui->friendList_2->addItem(newItem);
    }
}

void MainWindow::CreateFriendWindow()
{
    if (!mIsFriendWindowCreated)
    {
        this->mFriendWindow = new FriendWindow();
        connect(this, &MainWindow::OpenFriendWindow, mFriendWindow, &FriendWindow::FriendWindowOpened);
        connect(mFriendWindow, SIGNAL(BackToMainWindow()), this, SLOT(BackToMainWindowFromFriendSlot()));
        mIsFriendWindowCreated = true;
    }
}

void MainWindow::OnFriendClicked(QListWidgetItem* item)
{
    CreateFriendWindow();

    int index = ui->friendList->row(item);
    FriendUser* foundFriendUser = nullptr;
    int currentIndex = 0;

    for (const auto& friendInstance : mCurrentUser->GetUserFriends())
    {
        if (friendInstance->GetUserFriendStatus() == FriendUser::FriendStatusIsFriend)
        {
            if (currentIndex == index)
            {
                foundFriendUser = friendInstance;
                break;
            }

            currentIndex++;
        }
    }

    emit OpenFriendWindow(mClientInstance, mCurrentUser, foundFriendUser);

    mFriendWindow->show();
}

void MainWindow::CreateAddPostWindow()
{
    if (!mIsAddPostWindowCreated)
    {
        this->mAddPostWindow = new AddPostWindow();
        connect(this, &MainWindow::OpenAddPostWindow, mAddPostWindow, &AddPostWindow::AddPostWindowOpened);
        connect(mAddPostWindow, SIGNAL(BackToMainWindow()), this, SLOT(BackToMainWindowFromAddPostSlot()));
        mIsAddPostWindowCreated = true;
    }
}

void MainWindow::BackToMainWindowFromAddPostSlot()
{
    mAddPostWindow->hide();
    mClientInstance->SetCurrentWindow(this);

    PerformGetPostsOperation();
}

QString MainWindow::GetUsernameByPost(UserPost* post)
{
    const FriendUser* friendUser = mCurrentUser->GetFriendById(post->GetPostUserId());
    if (friendUser != nullptr)
    {
        return friendUser->GetUserName();
    }
    else
    {
        return "Me"; // case when post was written by me;
    }
}

QPixmap MainWindow::GetUserImageByPost(UserPost* post)
{
    QPixmap pixmap;

    const FriendUser* friendUser = mCurrentUser->GetFriendById(post->GetPostUserId());
    if (friendUser != nullptr)
    {
        if (friendUser->GetUserPicture() == nullptr)
        {
            pixmap = QPixmap(":/img/noimage.jpg");
            return pixmap;
        }
        pixmap.loadFromData(QByteArray::fromHex(*friendUser->GetUserPicture()));
    }
    else
    {
        if (mCurrentUser->GetUserPicture() == nullptr)
        {
            pixmap = QPixmap(":/img/noimage.jpg");
            return pixmap;
        }
        pixmap.loadFromData(QByteArray::fromHex(*mCurrentUser->GetUserPicture())); // case when post was written by me;
    }

    return pixmap;
}

QString MainWindow::GetFormattedPostmessage(UserPost* post)
{
    QString message = GetUsernameByPost(post) + ": " + post->GetPostText();

    if (message.size() > 40)
    {
        message.truncate(40);
        message.append("...");
        message = message.leftJustified(50, ' ');
        QFont font("Times New Roman", 16);
        QFontMetrics fm(font);
        int pixelsWide = fm.horizontalAdvance(message);

        while(pixelsWide < 390)
        {
            pixelsWide = fm.horizontalAdvance(message.append(" "));
        }
    }
    else
    {
        message = message.leftJustified(55, ' ');
        QFont font("Times New Roman", 16);
        QFontMetrics fm(font);
        int pixelsWide = fm.horizontalAdvance(message);

        while(pixelsWide < 390)
        {
            pixelsWide = fm.horizontalAdvance(message.append(" "));
        }
    }

    message.append(post->GetPostTime().sliced(0, 17));

    return message;
}

void MainWindow::FillPostsList()
{
    ui->postsList->clear();

    for (const auto& postInstance : mPosts)
    {
         QListWidgetItem *newItem = new QListWidgetItem;
         newItem->setSizeHint(QSize(620, 50));
         newItem->setText(GetFormattedPostmessage(postInstance));
         newItem->setIcon(QIcon(GetUserImageByPost(postInstance).scaled(QSize(50, 50))));
         ui->postsList->addItem(newItem);
    }
}

void MainWindow::BackToMainWindowFromFriendSlot()
{
    mFriendWindow->hide();
    mClientInstance->SetCurrentWindow(this);
}

void MainWindow::CreateExerciseWindow()
{
    if (!mIsExerciseWindowCreated)
    {
        this->mExerciseWindow = new ExerciseWindow();
        connect(this, &MainWindow::OpenExerciseWindow, mExerciseWindow, &ExerciseWindow::ExerciseWindowOpened);
        connect(mExerciseWindow, SIGNAL(BackToMainWindow()), this, SLOT(BackToMainWindowFromExerciseSlot()));
        mIsExerciseWindowCreated = true;
    }
}

void MainWindow::ShowExerciseWindowDependingOnDay(int index, QString dayOfTheWeek)
{
    CreateExerciseWindow();

    Exercise* currentExercise = nullptr;
    for(const auto& exercise: mExercises)
    {
        if (exercise->GetDayOfTheWeek() == dayOfTheWeek)
        {
            if (index == 0)
            {
                currentExercise = exercise;
                break;
            }
            else
            {
                index--;
            }
        }
    }

    emit OpenExerciseWindow(mClientInstance, currentExercise);

    mExerciseWindow->show();
}

void MainWindow::OnMondayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningMondayList->row(item), "Monday");
}

void MainWindow::OnTuesdayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningTuesdayList->row(item), "Tuesday");
}

void MainWindow::OnWednesdayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningWednesdayList->row(item), "Wednesday");
}

void MainWindow::OnThursdayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningThursdayList->row(item), "Thursday");
}

void MainWindow::OnFridayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningFridayList->row(item), "Friday");
}

void MainWindow::OnSaturdayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningSaturdayList->row(item), "Saturday");
}

void MainWindow::OnSundayExerciseClicked(QListWidgetItem* item)
{
    ShowExerciseWindowDependingOnDay(ui->traningSundayList->row(item), "Sunday");
}

void MainWindow::BackToMainWindowFromExerciseSlot()
{
    mExerciseWindow->hide();
    mClientInstance->SetCurrentWindow(this);

    PerformGetUserExercises();
    PerformGetUserDataOperation();
}

void MainWindow::CreateAddExerciseWindow()
{
    if (!mIsAddExerciseWindowCreated)
    {
        this->mAddExerciseWindow = new AddExercise();
        connect(this, &MainWindow::OpenAddExerciseWindow, mAddExerciseWindow, &AddExercise::AddExerciseWindowOpened);
        connect(mAddExerciseWindow, SIGNAL(BackToMainWindow()), this, SLOT(BackToMainWindowFromAddExerciseSlot()));
        mIsAddExerciseWindowCreated = true;
    }
}

void MainWindow::BackToMainWindowFromAddExerciseSlot()
{
    mAddExerciseWindow->hide();
    mClientInstance->SetCurrentWindow(this);

    PerformGetUserExercises();
}

void MainWindow::CreatePostWindow()
{
    if (!mIsPostWindowCreated)
    {
        this->mPostWindow = new PostWindow();
        connect(this, &MainWindow::OpenPostWindow, mPostWindow, &PostWindow::OpenPostWindowSlot);
        connect(mPostWindow, SIGNAL(BackToMainWindow()), this, SLOT(BackToMainWindowFromPostSlot()));
        mIsPostWindowCreated = true;
    }
}

void MainWindow::OnPostClicked(QListWidgetItem* item)
{
    CreatePostWindow();

    int index = ui->postsList->row(item);
    UserPost* post = mPosts[index];

    emit OpenPostWindow(mClientInstance, post, GetUsernameByPost(post));

    mPostWindow->show();
}

void MainWindow::BackToMainWindowFromPostSlot()
{
    mPostWindow->hide();
    mClientInstance->SetCurrentWindow(this);

    PerformGetPostsOperation();
}

QPixmap MainWindow::GetProperExerciseStatusIcon(Exercise::ExerciseStatuses status)
{
    if (status == Exercise::StatusDone)
    {
       return QPixmap(":/exerciseStates/exercise_states/done.jpg");
    }
    else if (status == Exercise::StatusInProgress)
    {
        return QPixmap(":/exerciseStates/exercise_states/in_progress.jpg");
    }
    else // StatusNotDone
    {
        return QPixmap(":/exerciseStates/exercise_states/not_done.jpg");
    }
}

void MainWindow::FillTrainingLists()
{
    ui->traningMondayList->clear();
    ui->traningTuesdayList->clear();
    ui->traningWednesdayList->clear();
    ui->traningThursdayList->clear();
    ui->traningFridayList->clear();
    ui->traningSaturdayList->clear();
    ui->traningSundayList->clear();

    for (const auto& exercise : mExercises)
    {
        QString exerciseName = exercise->GetExerciseName();
        if (exerciseName.size() > 11)
        {
            exerciseName = " " + exerciseName.sliced(0, exerciseName.lastIndexOf(" ")) + "\n " + exerciseName.sliced(exerciseName.lastIndexOf(" ") + 1);
        }
        else
        {
            exerciseName = " " + exerciseName;
        }

        QString exerciseData = exerciseName + "\n " + QString::number(exercise->GetDuration()) + " minutes";
        QListWidgetItem *newItem = new QListWidgetItem(exerciseData);
        newItem->setIcon(GetProperExerciseStatusIcon(exercise->GetExerciseStatus()));

        if (exercise->GetDayOfTheWeek() == "Monday")
        {
            ui->traningMondayList->addItem(newItem);
        }
        else if (exercise->GetDayOfTheWeek() == "Tuesday")
        {
            ui->traningTuesdayList->addItem(newItem);
        }
        else if (exercise->GetDayOfTheWeek() == "Wednesday")
        {
            ui->traningWednesdayList->addItem(newItem);
        }
        else if (exercise->GetDayOfTheWeek() == "Thursday")
        {
            ui->traningThursdayList->addItem(newItem);
        }
        else if (exercise->GetDayOfTheWeek() == "Friday")
        {
            ui->traningFridayList->addItem(newItem);
        }
        else if (exercise->GetDayOfTheWeek() == "Saturday")
        {
            ui->traningSaturdayList->addItem(newItem);
        }
        else if (exercise->GetDayOfTheWeek() == "Sunday")
        {
            ui->traningSundayList->addItem(newItem);
        }
    }
}

void MainWindow::ResetTrainingWeekIfNeeded()
{
    int iniWeekNumber = IniHelper::GetCurrentWeekNumber();
    int iniYearNumber = IniHelper::GetCurrentYearNumber();

    if (iniWeekNumber != QDate::currentDate().weekNumber() || iniYearNumber != QDate::currentDate().year())
    {
        json refreshExercisesStatus;
        MessagingProtocol::BuildRefreshUserTrainingWeek(refreshExercisesStatus, mCurrentUser->GetUserId());

        mClientInstance->SendDataToServer(refreshExercisesStatus);                  // SEND request to update

        mClientInstance->ReceiveDataFromServer();                                   // RCV ignore value

        IniHelper::SetCurrentWeekAndYearNumber(QDate::currentDate().weekNumber(), QDate::currentDate().year());
    }
}


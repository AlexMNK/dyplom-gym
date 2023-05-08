#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>

#include "models/frienduser.h"


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

void MainWindow::FillFriendList()
{
    ui->friendList->clear();

    for (const auto& friendInstance : mCurrentUser->GetUserFriends())
    {
        if (friendInstance->GetUserFriendStatus() == FriendStatusIsFriend)
        {
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(friendInstance->GetUserName() + " " + friendInstance->GetUserHashtag()); //newItem->setIcon(QIcon(":/Img/Ranks/silver.png"));
            ui->friendList->addItem(newItem);
        }
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
        if (friendInstance->GetUserFriendStatus() == FriendStatusIsFriend)
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

void MainWindow::BackToMainWindowFromFriendSlot()
{
    mFriendWindow->hide();
    mClientInstance->SetCurrentWindow(this);
}


void MainWindow::FillPostsList()
{
    ui->postsList->clear();

    for (const auto& postInstance : mPosts)
    {
         QListWidgetItem *newItem = new QListWidgetItem;
         newItem->setText(GetUsernameByPost(postInstance) + ": " + postInstance->GetPostText() + "  " + postInstance->GetPostTime());
         ui->postsList->addItem(newItem);
    }
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

    emit OpenPostWindow(post, GetUsernameByPost(post));

    mPostWindow->show();
}

void MainWindow::BackToMainWindowFromPostSlot()
{
    mPostWindow->hide();
}




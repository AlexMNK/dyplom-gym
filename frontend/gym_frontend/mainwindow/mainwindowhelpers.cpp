#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>

#include "models/frienduser.h"

void MainWindow::FillFriendList()
{
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
        connect(mFriendWindow, SIGNAL(BackToMainWindow()), this, SLOT(BackToMainWindowSlot()));
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

    this->hide();
    mFriendWindow->show();
}

void MainWindow::BackToMainWindowSlot()
{
    this->show();
    mFriendWindow->hide();
    mClientInstance->SetCurrentWindow(this);
}




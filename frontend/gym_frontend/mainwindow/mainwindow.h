#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>

#include "transport/socketconnection.h"
#include "utils/QByteArrayConverter.hpp"
#include "utils/DataPartImageHelper.hpp"
#include "mainwindow/mainwindow.h"
#include "client-logic/client.h"
#include "messaging-protocol/messagingprotocol.h"
#include "models/mainuser.h"
#include "models/frienduser.h"
#include "friendwindow/friendwindow.h"
#include "models/userpost.h"
#include "postwindow/postwindow.h"
#include "models/exercise.h"

#include <QMainWindow>

#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QBuffer>
#include <QListWidgetItem>

using json = nlohmann::json;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void BackToAuthorization();
    void OpenFriendWindow(Client* clientInstance, MainUser* mainUser, FriendUser* friendUser);
    void OpenPostWindow(UserPost* userPost, QString userName);

public slots:
    void AuthorizationSuccess(Client* clientInstance, int userId);
    void OnFriendClicked(QListWidgetItem* item);
    void OnPostClicked(QListWidgetItem* item);
    void BackToMainWindowFromFriendSlot();
    void BackToMainWindowFromPostSlot();

private slots:
    void on_signOutButton_clicked();

private:
    QString GetUsernameByPost(UserPost* post);
    void FillFriendList();
    void CreateFriendWindow();
    void CreatePostWindow();
    void FillPostsList();
    void SetupUiDesign();

private: // user operations
    void PerformGetUserDataOperation();
    void PerformUpdateUserImageOperation();
    void PerformGetUserFriendsOperation();
    void PerformGetPostsOperation();
    void PerformGetUserExercises();

private:
    Ui::MainWindow *ui;
    Client* mClientInstance;
    MainUser* mCurrentUser;
    std::vector<UserPost*> mPosts;
    std::vector<Exercise*> mExercises;

    FriendWindow* mFriendWindow;
    bool mIsFriendWindowCreated{false};

    PostWindow* mPostWindow;
    bool mIsPostWindowCreated{false};
};

#endif // MAINWINDOW_H

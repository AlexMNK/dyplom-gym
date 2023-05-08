#ifndef FRIENDWINDOW_H
#define FRIENDWINDOW_H

#include <QMainWindow>

#include "client-logic/client.h"
#include "models/frienduser.h"
#include "models/mainuser.h"

namespace Ui {
class FriendWindow;
}

class FriendWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendWindow(QWidget *parent = nullptr);
    ~FriendWindow();

signals:
    void BackToMainWindow();

public slots:
    void FriendWindowOpened(Client* clientInstance, MainUser* mainUser, FriendUser* friendUser);

private slots:
    void on_backButton_clicked();

private:
    Ui::FriendWindow *ui;

    Client* mClientInstance;
    MainUser* mCurrentUser;
    FriendUser* mFriendInstance;
};

#endif // FRIENDWINDOW_H

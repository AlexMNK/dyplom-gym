#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include "client-logic/client.h"
#include "models/exercise.h"

#include <QMainWindow>

#include <nlohmann/json.hpp>

namespace Ui {
class AddFriend;
}

class AddFriend : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddFriend(QWidget *parent = nullptr);
    ~AddFriend();

signals:
    void BackToMainWindow();

public slots:
    void AddFriendWindowOpened(Client* clientInstance, int userId);

private slots:
    void on_Add_clicked();
    void on_cancel_clicked();

private:
    Ui::AddFriend *ui;

    int mUserId;
    Client* mClientInstance;
};

#endif // ADDFRIEND_H

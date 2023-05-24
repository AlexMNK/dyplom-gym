#ifndef POSTWINDOW_H
#define POSTWINDOW_H

#include <QMainWindow>

#include "models/userpost.h"
#include "client-logic/client.h"

namespace Ui {
class PostWindow;
}

class PostWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PostWindow(QWidget *parent = nullptr);
    ~PostWindow();

public slots:
    void OpenPostWindowSlot(Client* clientInstance, UserPost* userPost, QString userName);

signals:
    void BackToMainWindow();

private slots:


    void on_back_clicked();

    void on_edit_clicked();

    void on_editPicture_clicked();

    void on_delete_2_clicked();

private:
    Ui::PostWindow *ui;

    Client* mClientInstance;
    UserPost* mPost;
    QString mUserName;
    bool mIsEditable{false};
};

#endif // POSTWINDOW_H

#ifndef POSTWINDOW_H
#define POSTWINDOW_H

#include <QMainWindow>

#include "models/userpost.h"

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
    void OpenPostWindowSlot(UserPost* userPost, QString userName);

signals:
    void BackToMainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PostWindow *ui;

    UserPost* mPost;
    QString mUserName;
};

#endif // POSTWINDOW_H

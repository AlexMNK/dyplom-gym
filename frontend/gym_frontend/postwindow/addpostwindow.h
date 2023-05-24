#ifndef ADDPOSTWINDOW_H
#define ADDPOSTWINDOW_H

#include "client-logic/client.h"

#include <QMainWindow>
#include <QByteArray>

namespace Ui {
class AddPostWindow;
}

class AddPostWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddPostWindow(QWidget *parent = nullptr);
    ~AddPostWindow();

public slots:
    void AddPostWindowOpened(Client* clientInstance, int userId);

signals:
    void BackToMainWindow();

private slots:
    void on_back_clicked();

    void on_AddImage_clicked();

    void on_Add_clicked();

private:
    Ui::AddPostWindow *ui;

    Client* mClientInstance;
    int mUserId;

    QByteArray* mImage{nullptr};
};

#endif // ADDPOSTWINDOW_H

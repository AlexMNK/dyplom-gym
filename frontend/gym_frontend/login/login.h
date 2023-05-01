#ifndef LOGIN_H
#define LOGIN_H

#include <nlohmann/json.hpp>

#include "transport/socketconnection.h"
#include "utils/QByteArrayConverter.hpp"
#include "mainwindow/mainwindow.h"
#include "client-logic/client.h"

#include <QMainWindow>

#include <QDebug>
#include <QImage>
#include <QMessageBox>

using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void OpenMainWindow(Client* clientInstance, int userId);

public slots:
    void LogOutSlot();

private slots:
    void on_pushButton_clicked();

private:
    void CreateMainWindow();

private:
    Ui::Login *ui;
    Client* mClientInstance;
    MainWindow* mMainWindow;

};
#endif // LOGIN_H

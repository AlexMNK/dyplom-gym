#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>

#include "transport/socketconnection.h"
#include "utils/QByteArrayConverter.hpp"
#include "mainwindow/mainwindow.h"

#include <QMainWindow>

#include <QTcpSocket>
#include <QDebug>
#include <QImage>
#include <QMessageBox>

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

public slots:
    void AuthorizationSuccess(SocketConnection* connection, int userId);


private slots:
    void on_pushButton_clicked();

    void on_Gay_button_clicked();

private:
    Ui::MainWindow *ui;
    SocketConnection* mConnection;
    int mUserId;
};

#endif // MAINWINDOW_H

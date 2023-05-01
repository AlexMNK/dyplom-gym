#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>

#include "transport/socketconnection.h"
#include "utils/QByteArrayConverter.hpp"
#include "mainwindow/mainwindow.h"
#include "client-logic/client.h"
#include "messaging-protocol/messagingprotocol.h"

#include <QMainWindow>

#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QBuffer>

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
    void AuthorizationSuccess(Client* clientInstance, int userId);


private slots:
    void on_pushButton_clicked();

    void on_Gay_button_clicked();

    void on_update_img_clicked();

private:
    Ui::MainWindow *ui;
    Client* mClientInstance;
    int mUserId;
};

#endif // MAINWINDOW_H

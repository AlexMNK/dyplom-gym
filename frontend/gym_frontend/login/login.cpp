#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    if (!ConnectToServer("localhost", 1234, 30000))
    {
        QMessageBox::critical(this, "Error", "Failed to connect to a server, try again later");
        exit(1);
    }
}

Login::~Login()
{
    delete ui;
    delete mConnection;
    delete mMainWindow;
}

void Login::on_pushButton_clicked()
{
    CreateMainWindow();

    emit OpenMainWindow(mConnection, 1);
    this->hide();
    mMainWindow->show();
}


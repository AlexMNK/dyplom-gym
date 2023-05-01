#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    this->mClientInstance = new Client();

    if (!mClientInstance->ConnectToServer("localhost", 1234))
    {
        QMessageBox::critical(this, "Error", "Failed to connect to a server, try again later");
        exit(1);
    }
}

Login::~Login()
{
    delete ui;
    delete mClientInstance;
    delete mMainWindow;
}

void Login::CreateMainWindow()
{
    this->mMainWindow = new MainWindow();
    connect(this, &Login::OpenMainWindow, mMainWindow, &MainWindow::AuthorizationSuccess);
    connect(mMainWindow, SIGNAL(BackToAuthorization()), this, SLOT(LogOutSlot()));
}

void Login::LogOutSlot()
{
    mMainWindow->hide();
    this->show();
    mClientInstance->SetCurrentWindow(this);
}


void Login::on_pushButton_clicked()
{
    CreateMainWindow();

    emit OpenMainWindow(mClientInstance, 1);

    this->hide();
    mMainWindow->show();
}


#include "login.h"
#include "ui_login.h"

#include "utils/IniHelper.hpp"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    SetupUiDesign();

    this->mClientInstance = new Client();

    QString serverHostname, clientLogin, clientPassword;
    int port;

    if (!ReadIniFile(serverHostname, port, clientLogin, clientPassword))
    {
        QMessageBox::critical(this, "Error", "Error reading configuration from ini file");
        exit(1);
    }

    QString userName, userPassword;

    if (!clientLogin.isEmpty() && !clientPassword.isEmpty())
    {      
        userName = clientLogin;
        userPassword = clientPassword;
    }

    if (!mClientInstance->ConnectToServer("localhost", 1234))
    {
        QMessageBox::critical(this, "Error", "Failed to connect to a server, try again later");
        exit(1);
    }

    ShowLogIn(userName, userPassword);
}

Login::~Login()
{
    delete ui;
    delete mClientInstance;
    delete mMainWindow;
}

void Login::HideAllErrorLabels()
{
    ui->no_user_found->hide();
    ui->invalid_password->hide();
}

void Login::CreateMainWindow()
{
    if (!mIsMainWindowCreated)
    {
        this->mMainWindow = new MainWindow();
        connect(this, &Login::OpenMainWindow, mMainWindow, &MainWindow::AuthorizationSuccess);
        connect(mMainWindow, SIGNAL(BackToAuthorization()), this, SLOT(LogOutSlot()));
        mIsMainWindowCreated = true;
    }
}

void Login::LogOutSlot()
{
    mMainWindow->hide();
    this->show();
    mClientInstance->SetCurrentWindow(this);
}

void Login::on_loginButton_clicked()
{
    int userId = 0;
    PerformAuthorizeOperation(userId);

    if (userId != 0)
    {
        QString userLogin, userPassword;

        if (ui->rememberMe->isChecked())
        {
            userLogin = ui->user_name->text();
            userPassword = ui->user_password->text();
        }
        else
        {
            ui->user_name->clear();
            ui->user_password->clear();
        }

        SetIniLogCredentials(userLogin, userPassword);

        CreateMainWindow();
        emit OpenMainWindow(mClientInstance, userId);
        this->hide();
        mMainWindow->show();
    }
}


void Login::on_GoSignUpButton_clicked()
{
    ShowSignUp();
}


void Login::on_GoLogInButton_clicked()
{
    ShowLogIn("", "");
}


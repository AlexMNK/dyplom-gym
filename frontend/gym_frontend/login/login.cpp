#include "login.h"
#include "ui_login.h"

#include <QSettings>

#include "utils/inihelper.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    SetupUiDesign();

    this->mClientInstance = new Client();

    QString serverHostname = "";
    int port = 0;

    if (!IniHelper::ReadIniFile(serverHostname, port, mSavedLogin, mSavedPassword))
    {
        QSettings* settings = new QSettings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
        settings->sync();
        IniHelper::SetDefaultIniConnectionOptions();
        IniHelper::ReadIniFile(serverHostname, port, mSavedLogin, mSavedPassword); // read again
        delete settings;
    }

    if (!mClientInstance->ConnectToServer(serverHostname, port))
    {
        QMessageBox::critical(this, "Error", "Failed to connect to a server, try again later");
        exit(1);
    }

    ShowLogIn(mSavedLogin, mSavedPassword);
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
        if (ui->rememberMe->isChecked())
        {
            mSavedLogin = ui->user_name->text();
            mSavedPassword = ui->user_password->text();
        }
        else
        {
            ui->user_name->clear();
            ui->user_password->clear();

            mSavedLogin = "";
            mSavedPassword = "";
        }

        IniHelper::SetIniLogCredentials(mSavedLogin, mSavedPassword);

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
    ShowLogIn(mSavedLogin, mSavedPassword);
}


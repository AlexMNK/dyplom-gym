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

    ui->sign_error_user->hide();
    ui->sign_error_email->hide();
    ui->sign_error_pass1->hide();
    ui->sign_error_pass2->hide();
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


void Login::on_SignUp_clicked()
{
    HideAllErrorLabels();

    bool ifError = false;

    if (ui->sign_username->text().isEmpty())
    {
        ifError = true;
        ui->sign_error_user->show();
    }

    const QRegularExpression regex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption);
    if(!regex.match(ui->sign_user_email->text()).hasMatch())
    {
        ifError = true;
        ui->sign_error_email->show();
        ui->sign_error_email->setText("Email format \nis wrong!");
    }

    if (ui->sign_user_email->text().isEmpty())
    {
        ifError = true;
        ui->sign_error_email->show();
        ui->sign_error_email->setText("Enter your email!");
    }

    if (ui->sign_user_pass1->text().isEmpty())
    {
        ifError = true;
        ui->sign_error_pass1->show();
    }

    if (ui->sign_user_pass2->text().isEmpty())
    {
        ifError = true;
        ui->sign_error_pass2->show();
        ui->sign_error_pass2->setText("Repeat your \npassword!");
    }

    if (ui->sign_user_pass1->text() != ui->sign_user_pass2->text() && (!ui->sign_user_pass1->text().isEmpty() && !ui->sign_user_pass2->text().isEmpty()))
    {
        ifError = true;
        ui->sign_error_pass2->show();
        ui->sign_error_pass2->setText("Passwords should \nbe equal!");
    }

    if (!ifError)
    {
        json signUpMessage;

        MessagingProtocol::BuildSignUp(signUpMessage, ui->sign_username->text(), ui->sign_user_email->text(), ui->sign_user_pass1->text());

        mClientInstance->SendDataToServer(signUpMessage);                           // SEND sign up request

        const json reply = mClientInstance->ReceiveDataFromServer();                // RCV status

        QString status, newName;

        MessagingProtocol::AcquireSignUpReply(reply, status, newName);

        if (status == "NewName")
        {
            QMessageBox::information(this, "New name", "Current name has already been taken. System has recommended you a new available name.");
            ui->sign_username->setText(newName);
        }
        else if (status != "OK")
        {
            QMessageBox::warning(this, "Failure", "Failed to sign up!");
        }
        else
        {
            QMessageBox::information(this, "Success", "Sign up succeeded!");
            ShowLogIn(ui->sign_username->text(), ui->sign_user_pass1->text());
        }
    }
}




#include "login.h"
#include "ui_login.h"

void Login::SetupUiDesign()
{
    ui->loginButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->GoSignUpButton->setStyleSheet("QPushButton {border: none;color: palette(window-text);background: transparent;} QPushButton:hover { color:darkgreen;}");
    ui->backgroundLabel->raise();
    ui->user_name->setStyleSheet("QLineEdit {border: 1px solid black; } QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->user_password->setStyleSheet("QLineEdit {border: 1px solid black; padding-bottom:3px;} QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->user_password->setEchoMode(QLineEdit::Password);

    ui->SignUp->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->GoLogInButton->setStyleSheet("QPushButton {border: none;color: palette(window-text);background: transparent;} QPushButton:hover { color:darkgreen;}");
    ui->sign_username->setStyleSheet("QLineEdit {border: 1px solid black; } QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->sign_user_email->setStyleSheet("QLineEdit {border: 1px solid black; } QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->sign_user_pass1->setStyleSheet("QLineEdit {border: 1px solid black; padding-bottom:3px;} QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->sign_user_pass1->setEchoMode(QLineEdit::Password);
    ui->sign_user_pass2->setStyleSheet("QLineEdit {border: 1px solid black; padding-bottom:3px;} QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->sign_user_pass2->setEchoMode(QLineEdit::Password);

    this->setStyleSheet("background-color: white;");
}

void Login::ShowLogIn(QString userName, QString userPassword)
{
    ui->backgroundLabel->move(0, 0);
    ui->user_name->setText(userName);
    ui->user_password->setText(userPassword);

    ui->user_name->show();
    ui->user_password->show();
    ui->LogInLabel->show();
    ui->userNameLabel->show();
    ui->userPassLabel->show();
    ui->remerberLabel->show();
    ui->rememberMe->show();
    ui->loginButton->show();
    ui->goToSignupLabel->show();
    ui->goToSignupLabel->show();

    HideAllErrorLabels();

    ui->SignUpLabel->hide();
    ui->sign_username->hide();
    ui->sign_user_email->hide();
    ui->sign_user_pass1->hide();
    ui->sign_user_pass2->hide();
    ui->sign_name_label->hide();
    ui->sign_email_label->hide();
    ui->sign_pass_label1->hide();
    ui->sign_pass_label2->hide();
    ui->SignUp->hide();
    ui->alrRegister->hide();
    ui->GoLogInButton->hide();

    this->setWindowTitle("Log in");
}

void Login::ShowSignUp()
{
    ui->backgroundLabel->move(432, 0);

    ui->user_name->hide();
    ui->user_password->hide();
    ui->LogInLabel->hide();
    ui->userNameLabel->hide();
    ui->userPassLabel->hide();
    ui->remerberLabel->hide();
    ui->rememberMe->hide();
    ui->loginButton->hide();
    ui->goToSignupLabel->hide();
    ui->goToSignupLabel->hide();

    HideAllErrorLabels();

    ui->SignUpLabel->show();
    ui->sign_username->show();
    ui->sign_user_email->show();
    ui->sign_user_pass1->show();
    ui->sign_user_pass2->show();
    ui->sign_username->clear();
    ui->sign_user_email->clear();
    ui->sign_user_pass1->clear();
    ui->sign_user_pass2->clear();
    ui->sign_name_label->show();
    ui->sign_email_label->show();
    ui->sign_pass_label1->show();
    ui->sign_pass_label2->show();
    ui->SignUp->show();
    ui->alrRegister->show();
    ui->GoLogInButton->show();

    this->setWindowTitle("Sign up");
}

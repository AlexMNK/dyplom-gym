#include "addfriend.h"
#include "ui_addfriend.h"

#include <QMessageBox>

AddFriend::AddFriend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);

    ui->cancel->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->Add->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->lineEdit->setStyleSheet("QLineEdit {border: 1px solid black; } QLineEdit:focus { border: 1px solid darkgreen;}");

    this->setStyleSheet("background-color: white;");
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::AddFriendWindowOpened(Client* clientInstance)
{
    this->mClientInstance = clientInstance;
    mClientInstance->SetCurrentWindow(this);

    ui->lineEdit->clear();
}

void AddFriend::on_Add_clicked()
{
    QMessageBox::information(this, "Success", "A friend request was sent to " + ui->lineEdit->text());
    emit BackToMainWindow();
}


void AddFriend::on_cancel_clicked()
{
    emit BackToMainWindow();
}


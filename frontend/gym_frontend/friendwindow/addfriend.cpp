#include "addfriend.h"
#include "ui_addfriend.h"

#include "messaging-protocol/messagingprotocol.h"

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

void AddFriend::AddFriendWindowOpened(Client* clientInstance, int userId)
{
    this->mClientInstance = clientInstance;
    this->mUserId = userId;
    mClientInstance->SetCurrentWindow(this);

    ui->lineEdit->clear();
}

void AddFriend::on_Add_clicked()
{
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Wrong input", "Enter user name or hashtag!");
        return;
    }

    json addFriend;
    MessagingProtocol::BuildAddFriend(addFriend, mUserId, ui->lineEdit->text());

    mClientInstance->SendDataToServer(addFriend);                           // SEND add request

    const json reply = mClientInstance->ReceiveDataFromServer();            // RCV status

    if (QString::fromStdString(reply["Status"]) == "NOUSER")
    {
        QMessageBox::warning(this, "Failure", "Failed to find a user!");
    }
    else if (QString::fromStdString(reply["Status"]) == "ERROR")
    {
       QMessageBox::warning(this, "Failure", "Failed to send friend request!");
    }
    else // OK
    {
         QMessageBox::information(this, "Success", "A friend request was sent to " + ui->lineEdit->text());
    }

    emit BackToMainWindow();
}


void AddFriend::on_cancel_clicked()
{
    emit BackToMainWindow();
}


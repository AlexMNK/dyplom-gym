#include "login.h"
#include "ui_login.h"
#include "QDebug"

void Login::PerformAuthorizeOperation(int& outUserId)
{
    HideAllErrorLabels();

    bool ifNameEmpty = false;
    bool ifPassEmpty = false;

    if (ui->user_name->text().isEmpty())
    {
        ui->no_user_found->show();
        ui->no_user_found->setText("Enter a name!");
        ifNameEmpty = true;
    }

    if (ui->user_password->text().isEmpty())
    {
        ui->invalid_password->show();
        ui->invalid_password->setText("Enter a password!");
        ifPassEmpty = true;
    }

    if (ifNameEmpty || ifPassEmpty)
    {
        return;
    }

    json authorizeMessage;
    MessagingProtocol::BuildAuthorize(authorizeMessage, ui->user_name->text(), ui->user_password->text());

    qDebug() << "Sending " <<  ui->user_name->text() << ui->user_password->text();
    mClientInstance->SendDataToServer(authorizeMessage);                        // SEND user login and password
    qDebug() << "after send";

    int userId = 0;
    QString status;
    qDebug() << "Receiving ";
    const json serverResponse = mClientInstance->ReceiveDataFromServer();       // RCV User Id and status
    MessagingProtocol::AcquireAuthorizeReply(serverResponse, userId, status);
    qDebug() << "Received " << userId << status;

    if (status == "OK")
    {
        outUserId = userId;
    }
    else if (status == "NOUSER")
    {
        ui->no_user_found->show();
        ui->no_user_found->setText("No user found!");
        return;
    }
    else // invalid password
    {
        ui->invalid_password->show();
        ui->invalid_password->setText("Invalid password!");
        return;
    }
}

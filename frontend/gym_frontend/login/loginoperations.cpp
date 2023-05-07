#include "login.h"
#include "ui_login.h"
#include "QDebug"

void Login::PerformAuthorizeOperation(int& outUserId)
{
    HideAllErrorLabels();

    if (ui->user_name->text().isEmpty())
    {
        QMessageBox::warning(this, "GAY!", "Enter name!");
        return;
    }

    if (ui->user_password->text().isEmpty())
    {
        QMessageBox::warning(this, "GAY!", "Enter password!");
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
        return;
    }
    else // invalid password
    {
        ui->invalid_password->show();
        return;
    }
}

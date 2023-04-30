#include "login.h"

bool Login::ConnectToServer(const QString hostname, int port, int operationTimeoutDelay)
{
    QTcpSocket* socket = new QTcpSocket();

    socket->connectToHost(hostname, port);

    if (!socket->waitForConnected(operationTimeoutDelay))
    {
        qDebug() << "Error: " << socket->errorString();
        return false;
    }

    qDebug() << "Connected to server";
    this->mConnection = new SocketConnection(*socket, operationTimeoutDelay);
    return true;
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
}

#include "client.h"
#include <QDebug>
#include <QMessageBox>

static constexpr int MAX_CLIENT_BLOCKING_PERIOD = 3000; // 3 sec max for server responce

Client::~Client()
{
    if (mSocketConnection != nullptr)
    {
        delete mSocketConnection;
    }
}

void Client::SetCurrentWindow(QMainWindow* window)
{
    this->mCurrentWindow = window;
}

SocketConnection* Client::GetSocketConnection()
{
    return mSocketConnection;
}

bool Client::ConnectToServer(const QString& hostname, int port)
{
    this->mRawConnection = new QTcpSocket();
    mRawConnection->connectToHost(hostname, port);

    if (!mRawConnection->waitForConnected(MAX_CLIENT_BLOCKING_PERIOD))
    {
        return false;
    }

    this->mSocketConnection = new SocketConnection(*mRawConnection, MAX_CLIENT_BLOCKING_PERIOD);

    qDebug() << "Connected to server";

    return true;
}

void Client::SendDataToServer(const json &message)
{
    if (!mSocketConnection->Write(message))
    {
        QMessageBox::critical(mCurrentWindow, "Error", "Failed send data to a server, exiting...");
        exit(1);
    }
}

json Client::ReceiveDataFromServer()
{
    auto readResult = mSocketConnection->Read();

    if (!readResult)
    {
        QMessageBox::critical(mCurrentWindow, "Error", "Failed to receive data from a server, exiting...");
        exit(1);
    }
    else
    {
        return readResult.value();
    }
}



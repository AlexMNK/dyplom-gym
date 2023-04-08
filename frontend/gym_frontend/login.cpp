#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    QTcpSocket socket;
    socket.connectToHost("localhost", 1234); // Connect to the server on port 1234

    if (!socket.waitForConnected()) {
        qDebug() << "Error: " << socket.errorString();
    }

    qDebug() << "Connected to server";

    socket.write("Hello, server!");
    socket.waitForBytesWritten();
    qDebug() << "Written data";

    socket.waitForReadyRead();
    QByteArray data = socket.readAll();
    qDebug() << "Received data from server:" << data;

    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState)
    {
        socket.waitForDisconnected();
    }


    qDebug() << "Disconnected from server";
}

Login::~Login()
{
    delete ui;
}


#include "login.h"
#include "ui_login.h"

#include <QThread>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    //QThread::sleep(10);

    this->socket = new QTcpSocket();
    this->socket->connectToHost("localhost", 1234); // Connect to the server on port 1234

    if (!this->socket->waitForConnected()) {
        qDebug() << "Error: " << socket->errorString();
    }

    qDebug() << "Connected to server";
}

Login::~Login()
{
    delete ui;
}


void Login::on_pushButton_clicked()
{
    SocketConnection newConnection{*this->socket, 30000};

    auto optionalJson = newConnection.Read();

    if (optionalJson)
    {
        json jsonValue(std::move(optionalJson.value()));
        qDebug() << QString::fromStdString(jsonValue["status"]);
        qDebug() << QByteConverter::StringToQByte(jsonValue["image"]);

        QPixmap pixmap;
        bool res = pixmap.loadFromData(QByteArray::fromHex(QByteConverter::StringToQByte(jsonValue["image"])));
        qDebug() << res;

        ui->img_test->setPixmap(pixmap);
    }
}


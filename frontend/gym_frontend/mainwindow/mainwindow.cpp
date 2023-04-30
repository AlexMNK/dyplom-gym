#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AuthorizationSuccess(SocketConnection* connection, int userId)
{
    this->mConnection = connection;
    this->mUserId = userId;

    const json GetUserData =
    {
        {"Operation", "GetUserData"},
        {"UserId", mUserId},
    };

    mConnection->Write(GetUserData);
    const auto Result = mConnection->Read();

    if (Result)
    {
        const json userData(std::move(Result.value()));
        ui->name->setText(QString::fromStdString(userData["name"]));
        ui->password->setText(QString::fromStdString(userData["password"]));

        QPixmap pixmap;
        bool res = pixmap.loadFromData(QByteArray::fromHex(QByteConverter::StringToQByte(userData["image"])));
        qDebug() << res;
        ui->image->setPixmap(pixmap);
    }
}

void MainWindow::on_pushButton_clicked()
{
    emit BackToAuthorization();
}


void MainWindow::on_Gay_button_clicked()
{
    const json GayJson =
    {
        {"Operation", "GAY"},
    };

    mConnection->Write(GayJson);

    const auto Result = mConnection->Read();

    if (Result)
    {
         const json userData(std::move(Result.value()));
         const QString result = QString::fromStdString(userData["GAY"]);
         QMessageBox::information(this, "Data", result);
    }
}


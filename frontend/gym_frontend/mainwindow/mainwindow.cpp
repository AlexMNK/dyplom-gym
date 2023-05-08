#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->friendList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnFriendClicked(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mCurrentUser;
    delete mFriendWindow;
}

void MainWindow::AuthorizationSuccess(Client* clientInstance, int userId)
{
    this->mClientInstance = clientInstance;
    mClientInstance->SetCurrentWindow(this);
    this->mCurrentUser = new MainUser(userId);

    PerformGetUserDataOperation();
    PerformGetUserFriendsOperation();
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

    mClientInstance->SendDataToServer(GayJson);

    const json serverReply = mClientInstance->ReceiveDataFromServer();

    const QString result = QString::fromStdString(serverReply["GAY"]);
    QMessageBox::information(this, "Data", result);
}


void MainWindow::on_update_img_clicked()
{
    PerformUpdateUserImageOperation();
    PerformGetUserDataOperation();
}



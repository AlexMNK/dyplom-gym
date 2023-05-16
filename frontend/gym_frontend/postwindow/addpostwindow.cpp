#include "addpostwindow.h"
#include "ui_addpostwindow.h"

AddPostWindow::AddPostWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddPostWindow)
{
    ui->setupUi(this);
}

AddPostWindow::~AddPostWindow()
{
    delete ui;
}

void AddPostWindow::AddPostWindowOpened(Client* clientInstance, int userId)
{
    this->mClientInstance = clientInstance;
    this->mUserId = userId;
    mClientInstance->SetCurrentWindow(this);
}

void AddPostWindow::on_back_clicked()
{
    emit BackToMainWindow();
}


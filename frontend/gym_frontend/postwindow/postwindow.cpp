#include "postwindow.h"
#include "ui_postwindow.h"
#include <QMessageBox>

PostWindow::PostWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostWindow)
{
    ui->setupUi(this);
}

PostWindow::~PostWindow()
{
    delete ui;
}

void PostWindow::OpenPostWindowSlot(UserPost* userPost, QString userName)
{
    this->mPost = userPost;
    this->mUserName = userName;

    ui->username->setText(userName);
    ui->text->setText(userPost->GetPostText());
    ui->time->setText(userPost->GetPostTime());
    QPixmap pixmap;
    bool loadImage = pixmap.loadFromData(QByteArray::fromHex(*userPost->GetPostPicture()));

    if (!loadImage)
    {
        QMessageBox::warning(this, "Warning", "Failed load user image");
    }

    ui->image->setPixmap(pixmap);
}

void PostWindow::on_pushButton_clicked()
{
    emit BackToMainWindow();
}


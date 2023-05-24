#include "addpostwindow.h"
#include "ui_addpostwindow.h"

#include "messaging-protocol/messagingprotocol.h"
#include "utils/DataPartImageHelper.hpp"

#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QBuffer>

AddPostWindow::AddPostWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddPostWindow)
{
    ui->setupUi(this);

    ui->back->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->Add->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->AddImage->setStyleSheet ("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->textEdit->setStyleSheet ("QTextEdit {border: 1px solid black; } QTextEdit:focus { border: 1px solid darkgreen;}");

    this->setStyleSheet("background-color: white;");
}

AddPostWindow::~AddPostWindow()
{
    if (mImage != nullptr)
    {
        delete mImage;
    }
    delete ui;
}

void AddPostWindow::AddPostWindowOpened(Client* clientInstance, int userId)
{
    this->mClientInstance = clientInstance;
    this->mUserId = userId;
    ui->textEdit->clear();
    ui->image->setPixmap(QPixmap(":/img/noimage.jpg"));
    mClientInstance->SetCurrentWindow(this);
}

void AddPostWindow::on_back_clicked()
{
    emit BackToMainWindow();
}


void AddPostWindow::on_AddImage_clicked()
{
    const QString file_name = QFileDialog::getOpenFileName(this, "Choose image");

    QImageReader imageReader{file_name};

    QImage image = imageReader.read();

    if(image.isNull())
    {
         QMessageBox::warning(this, "Warning", "Failed to load image, formats supported: JPEG, PNG");
         return;
    }

    QByteArray* byteImage = new QByteArray();
    QBuffer buffer(byteImage);
    buffer.open(QIODevice::WriteOnly);

    if (!image.save(&buffer, "JPEG"))
    {
        QMessageBox::warning(this, "Warning", "Invalid format of image, formats supported: JPEG, PNG");
        return;
    }

    mImage = byteImage;

    QPixmap pixmap;
    pixmap.loadFromData(*mImage);

    ui->image->setPixmap(pixmap);
}


void AddPostWindow::on_Add_clicked()
{
    if (ui->textEdit->toPlainText().size() == 0)
    {
        QMessageBox::warning(this, "Warning", "You cannot create a post without a text!");
        return;
    }

    if (ui->textEdit->toPlainText().size() >= 300)
    {
        QMessageBox::warning(this, "Warning", "Post text size should be below 300 characters!");
        return;
    }

    int imageSize = mImage == nullptr ? 0 : mImage->size();

    json addPost;
    MessagingProtocol::BuildAddPost(addPost, mUserId, ui->textEdit->toPlainText(), imageSize);

    mClientInstance->SendDataToServer(addPost);                                                     // SEND post data name

    if (imageSize != 0)
    {
        DataPartImageHelper::SendImageByParts(mClientInstance->GetSocketConnection(), *mImage);     // SEND img by parts
    }

    const json reply = mClientInstance->ReceiveDataFromServer();                                    // RCV status

    if (QString::fromStdString(reply["Status"]) != "OK")
    {
        QMessageBox::warning(this, "Failure", "Failed to add a post!");

        emit BackToMainWindow();
    }

    QMessageBox::information(this, "Success", "Post was added successfully!");

    emit BackToMainWindow();
}


#include "postwindow.h"
#include "ui_postwindow.h"

#include "messaging-protocol/messagingprotocol.h"
#include "utils/DataPartImageHelper.hpp"

#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QBuffer>

PostWindow::PostWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostWindow)
{
    ui->setupUi(this);

    ui->back->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->edit->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->editPicture->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->delete_2->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->textEdit->setStyleSheet ("QTextEdit {border: none; }");

    this->setStyleSheet("background-color: white;");
}

PostWindow::~PostWindow()
{
    delete ui;
}

void PostWindow::OpenPostWindowSlot(Client* clientInstance, UserPost* userPost, QString userName)
{
    this->mClientInstance = clientInstance;
    mClientInstance->SetCurrentWindow(this);
    this->mPost = userPost;
    this->mUserName = userName;

    ui->textEdit->setStyleSheet ("QTextEdit {border: none; }");
    ui->edit->setText("Edit");
    mIsEditable = false;

    ui->info->setText(" Post by " + userName + "\n " + userPost->GetPostTime().sliced(0, 17) + ":");
    ui->textEdit->setText(userPost->GetPostText());
    ui->textEdit->setEnabled(false);

    QPixmap pixmap;

    if (userPost->GetPostPicture() == nullptr)
    {
        pixmap = QPixmap(":/img/noimage.jpg");
    }
    else
    {
        bool loadImage = pixmap.loadFromData(QByteArray::fromHex(*userPost->GetPostPicture()));

        if (!loadImage)
        {
            QMessageBox::warning(this, "Warning", "Failed to load user image");
        }
    }

    ui->image->setPixmap(pixmap);

    if (mUserName == "Me")
    {
        this->setMaximumWidth(649);
        ui->back->move(540, 18);
        this->resize(649, 219);
    }
    else
    {
        this->setMaximumWidth(539);
        ui->back->move(430, 20);
        this->resize(539, 219);
    }
}

void PostWindow::on_back_clicked()
{
    emit BackToMainWindow();
}


void PostWindow::on_edit_clicked()
{
    if (mIsEditable) // save
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

        json editPost;
        MessagingProtocol::BuildEditPost(editPost, mPost->GetPostId(), ui->textEdit->toPlainText());

        mClientInstance->SendDataToServer(editPost);                                                    // SEND edit message

        const json reply = mClientInstance->ReceiveDataFromServer();                                    // RCV status

        if (QString::fromStdString(reply["Status"]) != "OK")
        {
            QMessageBox::warning(this, "Failure", "Failed to edit a post!");

            emit BackToMainWindow();
        }

        QMessageBox::information(this, "Success", "Post was edited successfully!");

        emit BackToMainWindow();
    }
    else // edit
    {
        ui->textEdit->setEnabled(true);
        ui->textEdit->setStyleSheet ("QTextEdit {border: 1px solid black; } QTextEdit:hover { border: 1px solid darkgreen;}");
        ui->edit->setText("Save");
        mIsEditable = true;
    }
}


void PostWindow::on_editPicture_clicked()
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

    int imageSize = byteImage->size();

    json updateImageMessage;
    MessagingProtocol::BuildUpdatePostImage(updateImageMessage, mPost->GetPostId(), imageSize);

    mClientInstance->SendDataToServer(updateImageMessage);                                      // SEND post id and image size

    DataPartImageHelper::SendImageByParts(mClientInstance->GetSocketConnection(), *byteImage);  // SEND img by parts

    const json reply = mClientInstance->ReceiveDataFromServer();                                // RCV status

    if (QString::fromStdString(reply["Status"]) != "OK")
    {
        QMessageBox::warning(this, "Failure", "Failed to edit image in a post!");

        emit BackToMainWindow();
    }

    QMessageBox::information(this, "Success", "Post image was edited successfully!");

    QPixmap pixmap;
    pixmap.loadFromData(*byteImage);

    ui->image->setPixmap(pixmap);
}


void PostWindow::on_delete_2_clicked()
{
    auto reply = QMessageBox::question(this, "Delete exercise", "Are you sure you want to delete this post?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        json deleteMessage;
        MessagingProtocol::BuildDeletePost(deleteMessage, mPost->GetPostId());

        mClientInstance->SendDataToServer(deleteMessage);                                           // SEND post id

        const json reply = mClientInstance->ReceiveDataFromServer();                                // RCV status

        if (QString::fromStdString(reply["Status"]) != "OK")
        {
            QMessageBox::warning(this, "Failure", "Failed to delete a post!");

            emit BackToMainWindow();
        }

        QMessageBox::information(this, "Success", "Post was deleted successfully!");

        emit BackToMainWindow();
    }
    else
    {
        return;
    }
}


#include "exercisewindow.h"
#include "ui_exercisewindow.h"

#include "messaging-protocol/messagingprotocol.h"

#include <QMessageBox>
#include <QIntValidator>

ExerciseWindow::ExerciseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExerciseWindow)
{
    ui->setupUi(this);

    ui->duration->setStyleSheet("border: none;");
    ui->duration->setEnabled(false);
    ui->back->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->Delete->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->Edit->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->Mark->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->duration->setValidator( new QIntValidator(0, 999, this) );

    this->setStyleSheet("background-color: white;");
}

ExerciseWindow::~ExerciseWindow()
{
    delete ui;
}

void ExerciseWindow::ExerciseWindowOpened(Client* clientInstance, Exercise* exercise)
{
    this->mClientInstance = clientInstance;
    this->mCurrentExercise = exercise;
    mClientInstance->SetCurrentWindow(this);

    ui->day->setText("Edit an exercise for a " + mCurrentExercise->GetDayOfTheWeek());
    ui->exercise->setText("Exercise:  " + mCurrentExercise->GetExerciseName());
    ui->duration->setText(QString::number(mCurrentExercise->GetDuration()) + " minutes");

    if (static_cast<int>(mCurrentExercise->GetExerciseStatus()) == 0)
    {
       ui->Mark->setText("Mark as not done");
       ui->Mark->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    }
    else
    {
        ui->Mark->setText("Mark as done");
        ui->Mark->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    }

    mIfEditable = false;

    ui->Edit->setText("Edit");
    ui->duration->setStyleSheet("border: none;");
    ui->duration->setEnabled(false);
}


void ExerciseWindow::on_back_clicked()
{
    emit BackToMainWindow();
}


void ExerciseWindow::on_Delete_clicked()
{
    auto reply = QMessageBox::question(this, "Delete exercise", "Are you sure you want to delete this exercise?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        json deleteMessage;
        MessagingProtocol::BuildDeleteExercise(deleteMessage, mCurrentExercise->GetExerciseId());

        mClientInstance->SendDataToServer(deleteMessage);                         // SEND delete request

        const json reply = mClientInstance->ReceiveDataFromServer();              // RCV status

        if (QString::fromStdString(reply["Status"]) != "OK")
        {
            QMessageBox::warning(this, "Failure", "Failed to delete an exercise");
        }
        else
        {
            QMessageBox::information(this, "Success", "Exercise " + mCurrentExercise->GetExerciseName() + " was successfully deleted");
        }

        emit BackToMainWindow();
    }
    else
    {
        return;
    }
}


void ExerciseWindow::on_Edit_clicked()
{
    if (mIfEditable)
    {
        json editMessage;
        MessagingProtocol::BuildEditExercise(editMessage, mCurrentExercise->GetExerciseId(), ui->duration->text().toInt(), static_cast<int>(mCurrentExercise->GetExerciseStatus()));

        mClientInstance->SendDataToServer(editMessage);                         // SEND delete request

        const json reply = mClientInstance->ReceiveDataFromServer();            // RCV status

        if (QString::fromStdString(reply["Status"]) != "OK")
        {
            QMessageBox::warning(this, "Failure", "Failed to edit an exercise");
        }
        else
        {
            QMessageBox::information(this, "Success", "Exercise " + mCurrentExercise->GetExerciseName() + " was successfully edited");
        }

        emit BackToMainWindow();
    }
    else
    {
        ui->Edit->setText("Save");
        ui->duration->setStyleSheet ("QLineEdit {border: 1px solid black; } QLineEdit:focus { border: 1px solid darkgreen;}");
        ui->duration->setText(QString::number(mCurrentExercise->GetDuration()));
        ui->duration->setEnabled(true);
        mIfEditable = true;
    }
}


void ExerciseWindow::on_Mark_clicked()
{
    json editMessage;

    if (static_cast<int>(mCurrentExercise->GetExerciseStatus()) == 0)
    {
        MessagingProtocol::BuildEditExercise(editMessage, mCurrentExercise->GetExerciseId(), mCurrentExercise->GetDuration(), 2);
    }
    else
    {
        MessagingProtocol::BuildEditExercise(editMessage, mCurrentExercise->GetExerciseId(), mCurrentExercise->GetDuration(), 0);
    }

    mClientInstance->SendDataToServer(editMessage);                         // SEND edit request

    const json reply = mClientInstance->ReceiveDataFromServer();            // RCV status

    if (QString::fromStdString(reply["Status"]) != "OK")
    {
        QMessageBox::warning(this, "Failure", "Failed edit an exercise status");
    }
    else
    {
        QString message;
        if (static_cast<int>(mCurrentExercise->GetExerciseStatus()) == 0)
        {
            message = "Exercise " + mCurrentExercise->GetExerciseName() + " was successfully marked as not done";
        }
        else
        {
            message = "Exercise " + mCurrentExercise->GetExerciseName() + " was successfully marked as done";
        }

        QMessageBox::information(this, "Success", message);
    }

    emit BackToMainWindow();
}


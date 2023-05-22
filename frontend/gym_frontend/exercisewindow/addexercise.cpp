#include "addexercise.h"
#include "ui_addexercise.h"

#include <vector>
#include <QMessageBox>
#include <QIntValidator>
#include "messaging-protocol/messagingprotocol.h"

AddExercise::AddExercise(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddExercise)
{
    ui->setupUi(this);

    ui->add->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkgreen;}");
    ui->cancel->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->duration->setStyleSheet("QLineEdit {border: 1px solid black; } QLineEdit:focus { border: 1px solid darkgreen;}");
    ui->duration->setValidator( new QIntValidator(0, 999, this) );
    ui->exercises->setStyleSheet ("QComboBox {border: 1px solid black;} QComboBox::drop-down {border-width: 0px;} QComboBox::down-arrow {image: url(noimg); border-width: 0px;} QComboBox:!enabled{color:black}");

    this->setStyleSheet("background-color: white;");
}

AddExercise::~AddExercise()
{
    delete ui;
}

void AddExercise::AddExerciseWindowOpened(Client* clientInstance, int userId, QString dayOfTheWeek)
{
    this->mClientInstance = clientInstance;
    this->mUserId = userId;
    this->mDayOfTheWeek = dayOfTheWeek;
    mClientInstance->SetCurrentWindow(this);

    ui->day->setText("Add an exercise for a " + mDayOfTheWeek);

    ui->duration->clear();
    FillExercisesComboBox();
}

void AddExercise::FillExercisesComboBox()
{
    ui->exercises->clear();

    std::vector<QString> allExercises;
    json getAllExercises;
    MessagingProtocol::BuildGetAllExercises(getAllExercises);

    mClientInstance->SendDataToServer(getAllExercises);                                 // SEND operation name

    const json serverReply = mClientInstance->ReceiveDataFromServer();                  // RCV all exercises

    MessagingProtocol::AcquireGetAllExercisesReply(serverReply, allExercises);

    for (const auto& exercise: allExercises)
    {
        ui->exercises->addItem(exercise);
    }
}

void AddExercise::on_cancel_clicked()
{
     emit BackToMainWindow();
}


void AddExercise::on_add_clicked()
{
    if (ui->duration->text().toInt() == 0)
    {
        QMessageBox::warning(this, "Wrong input", "Enter a positive number!");
        return;
    }

    json addExercise;
    MessagingProtocol::BuildAddExercise(addExercise, mUserId, mDayOfTheWeek, ui->exercises->currentText(), ui->duration->text().toInt());

    mClientInstance->SendDataToServer(addExercise);                         // SEND add request

    const json reply = mClientInstance->ReceiveDataFromServer();            // RCV status

    if (QString::fromStdString(reply["Status"]) != "OK")
    {
        QMessageBox::warning(this, "Failure", "Failed to add an exercise");
    }
    else
    {
        QMessageBox::information(this, "Success", "Exercise " + ui->exercises->currentText() + " added to a " + mDayOfTheWeek + " training");
    }

    emit BackToMainWindow();
}


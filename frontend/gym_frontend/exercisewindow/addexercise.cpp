#include "addexercise.h"
#include "ui_addexercise.h"

#include <vector>
#include "messaging-protocol/messagingprotocol.h"

AddExercise::AddExercise(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddExercise)
{
    ui->setupUi(this);
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

    ui->day->setText(mDayOfTheWeek);

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

void AddExercise::on_back_clicked()
{
    emit BackToMainWindow();
}


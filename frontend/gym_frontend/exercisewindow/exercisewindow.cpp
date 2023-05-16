#include "exercisewindow.h"
#include "ui_exercisewindow.h"

ExerciseWindow::ExerciseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExerciseWindow)
{
    ui->setupUi(this);
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

    ui->name->setText(mCurrentExercise->GetExerciseName());
    ui->day->setText(mCurrentExercise->GetDayOfTheWeek());
    ui->duration->setText(QString::number(mCurrentExercise->GetDuration()));
}

void ExerciseWindow::on_pushButton_clicked()
{
    emit BackToMainWindow();
}


#ifndef ADDEXERCISE_H
#define ADDEXERCISE_H

#include "client-logic/client.h"
#include "models/exercise.h"

#include <QMainWindow>

namespace Ui {
class AddExercise;
}

class AddExercise : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddExercise(QWidget *parent = nullptr);
    ~AddExercise();

signals:
    void BackToMainWindow();

public slots:
    void AddExerciseWindowOpened(Client* clientInstance, int userId, QString dayOfTheWeek);

private slots:
    void on_back_clicked();

private:
    void FillExercisesComboBox();

private:
    Ui::AddExercise *ui;

    Client* mClientInstance;
    int mUserId;
    QString mDayOfTheWeek;
};

#endif // ADDEXERCISE_H

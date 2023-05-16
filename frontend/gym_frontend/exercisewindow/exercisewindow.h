#ifndef EXERCISEWINDOW_H
#define EXERCISEWINDOW_H

#include "client-logic/client.h"
#include "models/exercise.h"

#include <QMainWindow>

namespace Ui {
class ExerciseWindow;
}

class ExerciseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExerciseWindow(QWidget *parent = nullptr);
    ~ExerciseWindow();

signals:
    void BackToMainWindow();

public slots:
    void ExerciseWindowOpened(Client* clientInstance, Exercise* exercise);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ExerciseWindow *ui;

    Client* mClientInstance;
    Exercise* mCurrentExercise;
};

#endif // EXERCISEWINDOW_H

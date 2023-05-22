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
    void on_back_clicked();
    void on_Delete_clicked();
    void on_Edit_clicked();
    void on_Mark_clicked();

private:
    Ui::ExerciseWindow *ui;

    Client* mClientInstance;
    Exercise* mCurrentExercise;

    bool mIfEditable{false};
};

#endif // EXERCISEWINDOW_H

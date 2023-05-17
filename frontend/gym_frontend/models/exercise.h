#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Exercise
{

public:
    enum ExerciseStatuses
    {
        StatusDone = 0,
        StatusInProgress = 1,
        StatusNotDone = 2,
    };

public:
    Exercise(int userId, int exerciseId);
    void AcquireGetExerciseDataReply(const json& message);

    int GetExerciseId() { return mExerciseId; }
    int GetUserId() { return mUserId; }
    QString GetDayOfTheWeek() { return mDayOfTheWeek; }
    QString GetExerciseName() { return mExerciseName; }
    float GetPointsPerHour() { return mPointsPerHour; }
    int GetDuration() { return mDuration; }
    ExerciseStatuses GetExerciseStatus() { return mExerciseStatus; }

private:
    int mExerciseId;
    int mUserId;
    QString mDayOfTheWeek;
    QString mExerciseName;
    float mPointsPerHour;
    int mDuration;
    ExerciseStatuses mExerciseStatus;
};

#endif // EXERCISE_H

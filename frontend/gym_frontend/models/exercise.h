#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Exercise
{
public:
    Exercise(int userId, int exerciseId);
    void AcquireGetExerciseDataReply(const json& message);

    int GetExerciseId() { return mExerciseId; }
    int GetUserId() { return mUserId; }
    QString GetDayOfTheWeek() { return mDayOfTheWeek; }
    QString GetExerciseName() { return mExerciseName; }
    float GetPointsPerHour() { return mPointsPerHour; }
    int GetDuration() { return mDuration; }

private:
    int mExerciseId;
    int mUserId;
    QString mDayOfTheWeek;
    QString mExerciseName;
    float mPointsPerHour;
    int mDuration;
};

#endif // EXERCISE_H

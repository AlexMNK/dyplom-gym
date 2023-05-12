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

private:
    int mExerciseId;
    int mUserId;
    QString mDayOfTheWeek;
    QString mExerciseName;
    int mDuration;
};

#endif // EXERCISE_H

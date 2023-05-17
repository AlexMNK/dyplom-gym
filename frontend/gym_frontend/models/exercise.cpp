#include "exercise.h"

#include "messaging-protocol/messagingprotocol.h"
#include <QDebug>

Exercise::Exercise(int userId, int exerciseId)
{
    this->mUserId = userId;
    this->mExerciseId = exerciseId;
}

void Exercise::AcquireGetExerciseDataReply(const json& message)
{
    QString exerciseStatus;

    MessagingProtocol::AcquireGetExerciseDataReply(message, mDayOfTheWeek, mExerciseName, exerciseStatus, mPointsPerHour, mDuration);

    if (exerciseStatus == "Done")
    {
        mExerciseStatus = StatusDone;
    }
    else if (exerciseStatus == "InProgress")
    {
        mExerciseStatus = StatusInProgress;
    }
    else // NotDone
    {
        mExerciseStatus = StatusNotDone;
    }

    qDebug() << "Got exercise " << mExerciseName << " on " << mDayOfTheWeek << " with duration " << mDuration << "and points " << mPointsPerHour << " with status " << exerciseStatus;
}

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
    MessagingProtocol::AcquireGetExerciseDataReply(message, mDayOfTheWeek, mExerciseName, mPointsPerHour, mDuration);

    qDebug() << "Got exercise " << mExerciseName << " on " << mDayOfTheWeek << " with duration " << mDuration << "and points " << mPointsPerHour;
}

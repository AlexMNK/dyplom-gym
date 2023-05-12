#ifndef DATAPARTIMAGEHELPER
#define DATAPARTIMAGEHELPER

#include "transport/socketconnection.h"
#include "messaging-protocol/messagingprotocol.h"

#include <nlohmann/json.hpp>

#include <QByteArray>
#include <QDebug>


using json = nlohmann::json;

static constexpr int SEGMENT_SIZE = 30000;

class DataPartImageHelper
{
public:
    static void SendImageByParts(SocketConnection* socketConnection, QByteArray& imageData)
    {
        int imageTotalSize = imageData.size();
        int dataPartCount = (imageTotalSize / SEGMENT_SIZE) + 1;
        int currentDataOffset = 0;

        socketConnection->ReadRaw();

        while (dataPartCount)
        {
            if (dataPartCount == 1)
            {
                socketConnection->WriteRaw(imageData.sliced(currentDataOffset));
                qDebug() << "Last part size" << imageData.sliced(currentDataOffset).size();
            }
            else
            {
                socketConnection->WriteRaw(imageData.sliced(currentDataOffset, SEGMENT_SIZE));
                qDebug() << "Part Data size" << imageData.sliced(currentDataOffset, SEGMENT_SIZE).size();
                currentDataOffset += SEGMENT_SIZE;
            }

            dataPartCount -= 1;

            socketConnection->ReadRaw();
            qDebug() << "Got unblocked from readraw";
        }
    }

    static QByteArray& ReceiveImageByParts(SocketConnection* socketConnection, int imageSize)
    {
        int dataPartCount = (imageSize / SEGMENT_SIZE) + 1;
        int currentDataOffset = 0;

        qDebug() << "Part Count " << dataPartCount;

        socketConnection->WriteRaw({"any data"});

        QByteArray* imageData = new QByteArray();
        imageData->reserve(imageSize);

        while (dataPartCount)
        {
            qDebug() << "Before read";
            QByteArray imagePart = socketConnection->ReadRaw();
            qDebug() << "After read";

            qDebug() << "Got part of QByteArray with size " << imagePart.size();
            imageData->insert(currentDataOffset, imagePart);
            currentDataOffset += SEGMENT_SIZE;

            dataPartCount -= 1;

            socketConnection->WriteRaw({"any data"}); // send any data to unblock next iteration of WriteRaw from other size
        }

        return *imageData;
    }
};



#endif // DATAPARTIMAGEHELPER

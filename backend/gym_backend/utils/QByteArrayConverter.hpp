#ifndef QBYTEARRAYCONVERTER
#define QBYTEARRAYCONVERTER

#include <nlohmann/json.hpp>

#include <QByteArray>
#include <QString>

using json = nlohmann::json;

class QByteConverter
{
public:
    static json QByteToJson(QByteArray byteArray)
    {
        return json::parse(QString(byteArray).toStdString());
    }

    static QByteArray JsonToQByte(json jsonObject)
    {
        return QString::fromStdString(jsonObject.dump()).toUtf8();
    }

    static std::string QByteToString(QByteArray byteArray)
    {
        return QString(byteArray).toStdString();
    }

    static QByteArray StringToQByte(std::string stringValue)
    {
        return QString::fromStdString(stringValue).toUtf8();
    }
};


#endif // QBYTEARRAYCONVERTER

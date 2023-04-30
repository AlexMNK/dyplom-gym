#include "serverhandlers.h"

#include "db/dbhelper.h"
#include "utils/QByteArrayConverter.hpp"

#include <QDebug>

typedef bool (*Handler)(DBTransport* dbTransport, const json&, json&);

static bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);
static bool HandleGetUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage);


static std::map<QString, Handler> operationPointers =
{
    {"GAY", &HandleGayOperation},
    {"GetUserData", &HandleGetUserDataOperation},
};


bool HandleMessage(DBTransport* dbTransport, const QString& operation, const json& userMessage, json& outResultMessage)
{
    return operationPointers[operation](dbTransport, userMessage, outResultMessage);
}


// -------------------------------------------- ALL HANDLERS ---------------------------------------------------//

bool HandleGayOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    outResultMessage =
    {
        {"GAY", "you are gae"},
    };

    return true;
}

bool HandleGetUserDataOperation(DBTransport* dbTransport, const json& userMessage, json& outResultMessage)
{
    int id = userMessage["UserId"];
    qDebug() << id;

    // Get user data

    auto optionalQuery = dbTransport->ExecuteQuery("SELECT user_name, user_password, user_profile_picture FROM Users WHERE id = " + QString::number(id));
    if (optionalQuery)
    {
        qDebug() << "1st query executed";
        QSqlQuery query(std::move(optionalQuery.value()));

        if (DBHelper::GetNextQueryResultRow(query))
        {
            std::string user_name = DBHelper::GetQueryData(query, 0).toString().toStdString();
            std::string user_password = DBHelper::GetQueryData(query, 1).toString().toStdString();
            int user_picture = DBHelper::GetQueryData(query, 2).toInt();

            qDebug() << QString::fromStdString(user_name);
            qDebug() << QString::fromStdString(user_password);
            qDebug() << user_picture;

            // Get image as QByteArray

            auto optionalQuery2 = dbTransport->ExecuteQuery("SELECT image from Images WHERE id = " + QString::number(user_picture));
            if (optionalQuery2)
            {
                qDebug() << "2st query executed";
                QSqlQuery query2(std::move(optionalQuery2.value()));
                std::string user_image;

                if (DBHelper::GetNextQueryResultRow(query2))
                {
                    user_image = QByteConverter::QByteToString(DBHelper::GetQueryData(query2, 0).toByteArray());
                }

                outResultMessage  =
                {
                    {"name", user_name},
                    {"password", user_password},
                    {"image", user_image}
                };

                return true;
            }
        }
    }

    return false;
}




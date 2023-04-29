#include "dbtransport.h"

DBTransport::DBTransport(const QString& dbType, const QString& dbName)
{
    mDbInstance = new QSqlDatabase(QSqlDatabase::addDatabase(dbType));
    mDbInstance->setDatabaseName(dbName);
}

DBTransport::~DBTransport()
{
    if (mDbInstance != nullptr)
    {
        delete mDbInstance;
    }
}

bool DBTransport::OpenConnection()
{
    return mDbInstance->open();
}

void DBTransport::CloseConnection()
{
    mDbInstance->close();
}

std::optional<QSqlQuery> DBTransport::ExecuteQuery(const QString& queryString)
{
    QSqlQuery sqlQuery(*mDbInstance);

    if (!sqlQuery.exec(queryString))
    {
       qDebug() << sqlQuery.lastError().text();
       return std::nullopt;
    }

    return sqlQuery;
}

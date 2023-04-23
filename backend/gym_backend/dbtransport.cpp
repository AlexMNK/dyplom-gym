#include "dbtransport.h"

DBTransport::DBTransport(const QString& dbType, const QString& dbName)
{
    dbInstance = new QSqlDatabase(QSqlDatabase::addDatabase(dbType));
    dbInstance->setDatabaseName(dbName);
}

DBTransport::~DBTransport()
{
    if (dbInstance != nullptr)
    {
        delete dbInstance;
    }
}

bool DBTransport::OpenConnection()
{
    return dbInstance->open();
}

void DBTransport::CloseConnection()
{
    dbInstance->close();
}

std::optional<QSqlQuery> DBTransport::ExecuteQuery(const QString& queryString)
{
    QSqlQuery sqlQuery(*dbInstance);

    if (!sqlQuery.exec(queryString))
    {
       return std::nullopt;
    }

    return sqlQuery;
}

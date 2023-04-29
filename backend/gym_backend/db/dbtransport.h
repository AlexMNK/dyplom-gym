#ifndef DBTRANSPORT_H
#define DBTRANSPORT_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

#include <optional>

class DBTransport
{
public:
    DBTransport(const QString& dbType, const QString& dbName);
    ~DBTransport();

    bool OpenConnection();
    void CloseConnection();

    std::optional<QSqlQuery> ExecuteQuery(const QString& queryString);

private:
    QSqlDatabase* mDbInstance;
};

#endif // DBTRANSPORT_H

#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlQuery>

class DBHelper
{
public:
    static bool GetNextQueryResultRow(QSqlQuery& query);
    static QVariant GetQueryData(QSqlQuery& query, int columnIndex);

private:
    DBHelper();
    ~DBHelper();
};

#endif // DBHELPER_H

#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlQuery>

class DBHelper
{
public:
    static bool GetNextQueryResultRow(QSqlQuery& query);
    static QVariant GetQueryData(QSqlQuery& query, int columnIndex);
};

#endif // DBHELPER_H

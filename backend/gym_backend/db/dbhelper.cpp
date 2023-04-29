#include "dbhelper.h"

QVariant DBHelper::GetQueryData(QSqlQuery& query, int columnIndex)
{
    return query.value(columnIndex);
}

bool DBHelper::GetNextQueryResultRow(QSqlQuery& query)
{
    return query.next();
}

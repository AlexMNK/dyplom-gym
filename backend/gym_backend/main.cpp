#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    return a.exec();


    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={ODBC Driver 17 for SQL Server};server=localhost;database=GymDatabase;trusted_connection=Yes;");
    if(db.open())
    {
        qDebug() << "db opened OK...";

        QString query = "SELECT * FROM Users";
        QSqlQuery sqlQ(db);

        if(!sqlQ.exec(query))
        {
            qDebug() << "query failed...";
        }
        else
        {
            while(sqlQ.next()) {
                qDebug() << sqlQ.value(0).toInt();
                qDebug() << sqlQ.value(1).toString();
            }
        }
     qDebug() << "end...";
    }
}

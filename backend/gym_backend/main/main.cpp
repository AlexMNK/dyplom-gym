#include "main.h"

#include <QtCore/QCoreApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dbType, dbName;
    int port = 0;

    // READ INI

    if (!ReadIniFile(dbType, dbName, port))
    {
         qDebug() << "Error reading data from ini file";
         return -1;
    }

    // START SERVER

    DBTransport* transport = new DBTransport(dbType, dbName);
    MainServerThread* serverInstance = new MainServerThread(transport, port);

    if (!serverInstance->StartServer())
    {
         qDebug() << "Error starting server";
         return -1;
    }

    delete serverInstance;
}

#include "main.h"

#include <QtCore/QCoreApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); // needed for event loop

    DBTransport* transport = new DBTransport("QODBC", "Driver={ODBC Driver 17 for SQL Server};server=localhost;database=GymDatabase;trusted_connection=Yes;");

    MainServerThread* serverInstance = new MainServerThread(transport, 1234);

    qDebug() << serverInstance->StartServer();
    delete serverInstance;
}

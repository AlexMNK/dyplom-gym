#include <QCoreApplication>
#include <QTest>
#include "testclass.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new TestClass());

    return a.exec();
}

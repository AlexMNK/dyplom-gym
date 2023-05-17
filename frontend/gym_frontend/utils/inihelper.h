#ifndef INIHELPER
#define INIHELPER

#include "mini/ini.h"

#include <QString>
#include <QDebug>

class IniHelper
{
public:
    static bool ReadIniFile(QString& serverHostname, int& port, QString& clientLogin, QString& clientPassword);
    static void SetIniLogCredentials(const QString& clientLogin, const QString& clientPassword);
    static void SetDefaultIniConnectionOptions();
    static int GetCurrentWeekNumber();
    static int GetCurrentYearNumber();
    static void SetCurrentWeekAndYearNumber(int week, int year);
};



#endif // INIHELPER

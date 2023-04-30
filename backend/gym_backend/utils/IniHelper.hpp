#ifndef INIHELPER
#define INIHELPER

#include "mini/ini.h"

#include <QString>

bool ReadIniFile(QString& dbType, QString& dbName, int& port)
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;

    if (!file.read(ini))
    {
        return false;
    }

    dbType = QString::fromStdString(ini["Options"]["DBType"]);
    dbName = QString::fromStdString(ini["Options"]["DBName"]);
    port = QString::fromStdString(ini["Options"]["ServerPort"]).toInt();

    if (dbType.isEmpty() || dbName.isEmpty() || port == 0)
    {
        return false;
    }

    return true;
}


#endif // INIHELPER

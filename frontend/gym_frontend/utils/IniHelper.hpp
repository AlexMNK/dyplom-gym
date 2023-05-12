#ifndef INIHELPER
#define INIHELPER

#include "mini/ini.h"

#include <QString>
#include <QDebug>

bool ReadIniFile(QString& serverHostname, int& port, QString& clientLogin, QString& clientPassword)
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;

    if (!file.read(ini))
    {
        qDebug() << "failed";
        return false;
    }

    serverHostname = QString::fromStdString(ini["Options"]["ServerHostname"]);
    port = QString::fromStdString(ini["Options"]["ServerPort"]).toInt();

    clientLogin = QString::fromStdString(ini["LogInCredentials"]["Login"]);
    clientPassword = QString::fromStdString(ini["LogInCredentials"]["Password"]);

    if (serverHostname.isEmpty() || port == 0)
    {
        return false;
    }

    return true;
}

void SetIniLogCredentials(const QString& clientLogin, const QString& clientPassword)
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;
    file.read(ini);

    ini["LogInCredentials"]["Login"] = clientLogin.toStdString();
    ini["LogInCredentials"]["Password"] = clientPassword.toStdString();

    file.generate(ini);
}

#endif // INIHELPER

#include "inihelper.h"

bool IniHelper::ReadIniFile(QString& serverHostname, int& port, QString& clientLogin, QString& clientPassword)
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

void IniHelper::SetIniLogCredentials(const QString& clientLogin, const QString& clientPassword)
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;
    file.read(ini);

    ini["LogInCredentials"]["Login"] = clientLogin.toStdString();
    ini["LogInCredentials"]["Password"] = clientPassword.toStdString();

    file.generate(ini);
}

void IniHelper::SetDefaultIniConnectionOptions()
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;
    file.read(ini);

    ini["options"]["serverhostname"] = "localhost";
    ini["options"]["serverport"] = QString::number(1234).toStdString();

    file.generate(ini);
}

int IniHelper::GetCurrentWeekNumber()
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;
    file.read(ini);

    return QString::fromStdString(ini["trainingtimedata"]["weeknumber"]).toInt();
}

int IniHelper::GetCurrentYearNumber()
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;
    file.read(ini);

    return QString::fromStdString(ini["trainingtimedata"]["yearnumber"]).toInt();
}

void IniHelper::SetCurrentWeekAndYearNumber(int week, int year)
{
    mINI::INIFile file("config.ini");
    mINI::INIStructure ini;
    file.read(ini);

    ini["trainingtimedata"]["weeknumber"] = QString::number(week).toStdString();
    ini["trainingtimedata"]["yearnumber"] = QString::number(year).toStdString();

    file.generate(ini);
}

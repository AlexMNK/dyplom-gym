#ifndef MAIN_H
#define MAIN_H

#include "dbtransport.h"
#include "dbhelper.h"

#include <nlohmann/json.hpp>

#include <QCoreApplication>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

using json = nlohmann::json;

#endif // MAIN_H

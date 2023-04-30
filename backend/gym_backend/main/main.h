#ifndef MAIN_H
#define MAIN_H

#include "db/dbtransport.h"
#include "db/dbhelper.h"
#include "transport/socketconnection.h"

#include "server-logic/mainserverthread.h"
#include "server-logic/serverworkerthread.h"

#include "utils/QByteArrayConverter.hpp"

#include <nlohmann/json.hpp>

#include <QCoreApplication>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

using json = nlohmann::json;

#endif // MAIN_H

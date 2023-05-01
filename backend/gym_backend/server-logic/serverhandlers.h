#ifndef SERVERHANDLERS_H
#define SERVERHANDLERS_H

#include "db/dbtransport.h"

#include <nlohmann/json.hpp>

#include <map>

using json = nlohmann::json;

class ServerMessageHandler
{
public:
    static bool HandleMessage(DBTransport* dbTransport, const QString& operation, const json& userMessage, json& outResultMessage);
};

#endif // SERVERHANDLERS_H

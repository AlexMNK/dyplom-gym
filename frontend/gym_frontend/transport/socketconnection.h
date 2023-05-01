#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include <QTcpSocket>
#include <QByteArray>

#include <nlohmann/json.hpp>

#include <optional>

using json = nlohmann::json;

class SocketConnection
{
public:
    SocketConnection(QTcpSocket &socket, int maxBlockingTimeout);
    ~SocketConnection();

    std::optional<json> Read();
    bool Write(const json& jsonObject);

private:
    QTcpSocket& mTcpSocket;
    int mBlockingTimeout;
};

#endif // SOCKETCONNECTION_H

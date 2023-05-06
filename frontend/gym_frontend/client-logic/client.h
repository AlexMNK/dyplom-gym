#ifndef CLIENT_H
#define CLIENT_H

#include "transport/socketconnection.h"

#include <QMainWindow>
#include <QTcpSocket>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Client
{
public:
    Client() {};
    ~Client();

    void SetCurrentWindow(QMainWindow* window);
    SocketConnection* GetSocketConnection();

    bool ConnectToServer(const QString& hostname, int port);

    void SendDataToServer(const json &message);
    json ReceiveDataFromServer();

private:
    SocketConnection* mSocketConnection;
    QTcpSocket* mRawConnection;

    QMainWindow* mCurrentWindow;
};

#endif // CLIENT_H

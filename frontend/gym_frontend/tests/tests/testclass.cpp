#include "testclass.h"

#include <QTest>
#include <QTcpServer>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include <vector>

static constexpr int MAX_SERVER_BLOCKING_PERIOD = -1;

TestClass::TestClass(QObject *parent) :
    QObject(parent)
{
    QFuture<void> future = QtConcurrent::run([=](){StartServerThread();});

    auto rawConnection = new QTcpSocket();
    rawConnection->connectToHost("localhost", 4444);
    rawConnection->waitForConnected(MAX_SERVER_BLOCKING_PERIOD);

    mCliendConnection = new SocketConnection(*rawConnection, MAX_SERVER_BLOCKING_PERIOD);
}

void TestClass::StartServerThread()
{
    QTcpServer* tcpServer = new QTcpServer();
    tcpServer->listen(QHostAddress::Any, 4444);

    tcpServer->waitForNewConnection(MAX_SERVER_BLOCKING_PERIOD);

    if (tcpServer->hasPendingConnections())
    {
       QTcpSocket *socket = tcpServer->nextPendingConnection();
       SocketConnection* newConnection = new SocketConnection{*socket, MAX_SERVER_BLOCKING_PERIOD};

       while(1)
       {
           auto readResult = newConnection->Read();
           if (readResult)
           {
               newConnection->Write(readResult.value());
           }
           else
           {
               break;
           }
       }
    }
}

void TestClass::testSimpleJsonSendReceiveSucceded()
{
    json object = {"data"};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testEmptyJsonSendReceiveSucceded()
{
    json object = {""};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testMessagingProtocolJsonSendReceiveSucceded()
{
    json object = {{"Operation", "SomeOperation"},{"Data", 123}};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testOneCharJsonSendReceiveSucceded()
{
    json object = {{"a"}};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testJsonArraySendReceiveSucceded()
{
    json jsonArray;
    std::vector<int> arrayData{1, 2, 3, 4, 5};

    for (const auto& data : arrayData)
    {
       jsonArray.push_back(data);
    }

    json object =
    {
        {"Array", jsonArray},
    };

    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testStdStringFromQStringJsonSendReceiveSucceded()
{
    json object = {{QString{"abc"}.toStdString()}};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testStdStringFromQByteArrayJsonSendReceiveSucceded()
{
    json object = {{QByteArray{"abc"}.toStdString()}};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

void TestClass::testStdStringFromQByteArray26KBJsonSendReceiveSucceded()
{
    QByteArray emptyArray{};
    QByteArray sendArray = emptyArray.rightJustified(26000, 'a');

    json object = {sendArray.toStdString()};
    mCliendConnection->Write(object);
    auto received = mCliendConnection->Read();
    if (received)
    {
        QCOMPARE(object, received.value());
    }
    else
    {
        QFAIL("Failed to receive data from server");
    }
}

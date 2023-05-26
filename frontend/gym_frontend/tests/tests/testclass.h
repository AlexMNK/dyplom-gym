#ifndef TESTCLASS_H
#define TESTCLASS_H

#include "socketconnection.h"

#include <QObject>

class TestClass : public QObject
{
    Q_OBJECT
public:
    explicit TestClass(QObject *parent = 0);

    void StartServerThread();

private slots:
    void testSimpleJsonSendReceiveSucceded();
    void testEmptyJsonSendReceiveSucceded();
    void testMessagingProtocolJsonSendReceiveSucceded();
    void testOneCharJsonSendReceiveSucceded();
    void testJsonArraySendReceiveSucceded();
    void testStdStringFromQStringJsonSendReceiveSucceded();
    void testStdStringFromQByteArrayJsonSendReceiveSucceded();
    void testStdStringFromQByteArray26KBJsonSendReceiveSucceded();

private:
    SocketConnection* mCliendConnection;

};

#endif // TESTCLASS_H

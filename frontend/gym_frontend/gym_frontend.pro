QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += C:\Users\alex2\Desktop\dyplom\json\include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client-logic/client.cpp \
    main.cpp \
    login/login.cpp \
    mainwindow/mainwindow.cpp \
    messaging-protocol/messagingprotocol.cpp \
    transport/socketconnection.cpp

HEADERS += \
    client-logic/client.h \
    login/login.h \
    mainwindow/mainwindow.h \
    messaging-protocol/messagingprotocol.h \
    transport/socketconnection.h \
    utils/DataPartImageHelper.hpp \
    utils/QByteArrayConverter.hpp

FORMS += \
    login/login.ui \
    mainwindow/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += C:\Users\alex2\Desktop\dyplom\json\include
INCLUDEPATH += C:\Users\alex2\Desktop\dyplom\mINI\src

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client-logic/client.cpp \
    exercisewindow/addexercise.cpp \
    exercisewindow/exercisewindow.cpp \
    friendwindow/addfriend.cpp \
    friendwindow/friendwindow.cpp \
    login/loginhelpers.cpp \
    login/loginoperations.cpp \
    main.cpp \
    login/login.cpp \
    mainwindow/mainwindow.cpp \
    mainwindow/mainwindowhelpers.cpp \
    mainwindow/mainwindowoperations.cpp \
    messaging-protocol/messagingprotocol.cpp \
    models/exercise.cpp \
    models/frienduser.cpp \
    models/mainuser.cpp \
    models/userpost.cpp \
    postwindow/addpostwindow.cpp \
    postwindow/postwindow.cpp \
    transport/socketconnection.cpp \
    utils/inihelper.cpp

HEADERS += \
    client-logic/client.h \
    exercisewindow/addexercise.h \
    exercisewindow/exercisewindow.h \
    friendwindow/addfriend.h \
    friendwindow/friendwindow.h \
    login/login.h \
    mainwindow/mainwindow.h \
    messaging-protocol/messagingprotocol.h \
    models/IUser.h \
    models/exercise.h \
    models/frienduser.h \
    models/mainuser.h \
    models/userpost.h \
    postwindow/addpostwindow.h \
    postwindow/postwindow.h \
    transport/socketconnection.h \
    utils/DataPartImageHelper.hpp \
    utils/QByteArrayConverter.hpp \
    utils/inihelper.h

FORMS += \
    exercisewindow/addexercise.ui \
    exercisewindow/exercisewindow.ui \
    friendwindow/addfriend.ui \
    friendwindow/friendwindow.ui \
    login/login.ui \
    mainwindow/mainwindow.ui \
    postwindow/addpostwindow.ui \
    postwindow/postwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    login/resources/depositphotos_73365001-stock-illustration-sport-seamless-pattern.jpg

RESOURCES += \
    resources/res.qrc

QT -= gui

CONFIG += c++17 console
CONFIG += qtestlib
QT += network

INCLUDEPATH += C:\Users\alex2\Desktop\dyplom\json\include

SOURCES += \
        main.cpp \
        socketconnection.cpp \
        testclass.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    socketconnection.h \
    testclass.h

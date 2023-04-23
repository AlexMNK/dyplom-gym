QT -= gui
QT += sql
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle
INCLUDEPATH += C:\Users\alex2\Desktop\dyplom\json\include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        dbhelper.cpp \
        dbtransport.cpp \
        main.cpp \
        sockettransport.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    dbhelper.h \
    dbtransport.h \
    main.h \
    sockettransport.h

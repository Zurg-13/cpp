#-------------------------------------------------
# Project created by QtCreator 2017-06-06T18:19:23
#-------------------------------------------------

DLG = ../../_dlg
WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt

QT += core gui widgets network concurrent httpserver
CONFIG += c++14

TARGET = zServer
TEMPLATE = app

include($$SRV/Srv/Srv.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    env.h           \
    FMain.h         \
    WHandler.h      \
    DHeader.h       \
    WLogEntry.h     \
    WLogBoard.h     \
    WHdlBoard.h     \

SOURCES += \
    main.cpp        \
    FMain.cpp       \
    WHandler.cpp    \
    DHeader.cpp     \
    WLogEntry.cpp   \
    WLogBoard.cpp   \
    WHdlBoard.cpp   \

FORMS += \
    FMain.ui        \
    WHandler.ui     \
    DHeader.ui      \
    WLogEntry.ui    \
    WLogBoard.ui    \
    WHdlBoard.ui    \

RESOURCES += \
    res.qrc


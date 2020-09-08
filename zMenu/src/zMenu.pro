#-------------------------------------------------
# Project created by QtCreator 2020-08-28T13:04:00
#-------------------------------------------------

LIB = ../../_lib
WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
REP = ../../_rep

QT += core gui widgets network concurrent httpserver sql websockets
CONFIG += c++14

TARGET = zMenu
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$SRV/Sql/Sql.pri)
include($$WGT/Brd/WBrd.pri)
include($$REP/About/RAbout.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    env.h       \
    ver.h       \
    FMain.h     \

SOURCES += \
    main.cpp    \
    FMain.cpp   \

FORMS += \
    FMain.ui    \

RESOURCES += \
    res.qrc     \

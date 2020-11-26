#-------------------------------------------------
# Project created by QtCreator 2020-11-26T15:44:00
#-------------------------------------------------

WGT = ../../_wgt
SRV = ../../_srv
REP = ../../_rep

QT += core gui widgets
CONFIG += c++14

TARGET = creature
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$WGT/Brd/WBrd.pri)
include($$REP/About/RAbout.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    env.h       \
    ver.h       \
    Part.h      \
    FMain.h     \

SOURCES += \
    main.cpp    \
    Part.cpp    \
    FMain.cpp   \

FORMS += \
    FMain.ui    \

RESOURCES += \
    res.qrc     \

#-------------------------------------------------
# Project created by QtCreator 2018-09-18T19:12:48
#-------------------------------------------------

WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
LIB = ../../_lib

QT += core gui widgets network
CONFIG += c++14

TARGET = medium
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$SRV/Log/Log.pri)
include($$WGT/Log/WLog.pri)
include($$LIB/QtUsb/src/QtUsb.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    FMain.h     \

SOURCES += \
    main.cpp    \
    FMain.cpp   \

FORMS += \
    FMain.ui    \

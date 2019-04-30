#-------------------------------------------------
# Project created by QtCreator 2017-09-07T15:16:30
#-------------------------------------------------

QT += core gui widgets
CONFIG += c++14

TARGET = Kross
TEMPLATE = app

WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
LIB = ../../_lib

INCLUDEPATH += ../../
DEPENDPATH  += ../../

include($$SRV/Srv.pri)
include($$WGT/Log/WLog.pri)

HEADERS += \
    FMain.h         \
    WLine.h         \
    WChain.h \
    Plate.h

SOURCES += \
    main.cpp        \
    FMain.cpp       \
    WLine.cpp       \
    WChain.cpp \
    Plate.cpp

FORMS += \
    FMain.ui        \
    WLine.ui        \
    WChain.ui

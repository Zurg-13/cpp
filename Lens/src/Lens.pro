#-------------------------------------------------
# Project created by QtCreator 2019-03-06T17:35:37
#-------------------------------------------------

WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
LIB = ../../_lib

QT += core gui widgets
CONFIG += c++14

TARGET = Lens
TEMPLATE = app

include($$SRV/Srv.pri)
include($$WGT/Log/WLog.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    FMain.h         \
    WLens.h \
    WClr.h

SOURCES += \
    main.cpp        \
    FMain.cpp \
    WLens.cpp \
    WClr.cpp


FORMS += \
    FMain.ui        \
    WLens.ui \
    WClr.ui

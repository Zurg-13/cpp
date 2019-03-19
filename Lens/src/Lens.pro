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

include($$SRV/Srv/Srv.pri)
include($$WGT/Log/WLog.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    AdvanceEdit.h   \
    FMain.h         \
    WLens.h         \
    WClr.h          \
    WBox.h

SOURCES += \
    AdvanceEdit.cpp \
    main.cpp        \
    FMain.cpp       \
    WLens.cpp       \
    WClr.cpp        \
    WBox.cpp


FORMS += \
    FMain.ui        \
    WLens.ui        \
    WClr.ui         \
    WBox.ui

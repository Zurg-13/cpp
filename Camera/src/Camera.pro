#-------------------------------------------------
# Project created by QtCreator 2019-10-11T16:19:51
#-------------------------------------------------

LIB = ../../_lib
WGT = ../../_wgt
SRV = ../../_srv
REP = ../../_rep

QT += core gui widgets multimedia multimediawidgets
CONFIG += c++14

TARGET = Camera
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$WGT/Log/WLog.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    env.h       \
    Cam.h       \
    FMain.h     \

SOURCES += \
    main.cpp    \
    Cam.cpp     \
    FMain.cpp   \

FORMS += \
    FMain.ui    \

RESOURCES += \
    res.qrc     \

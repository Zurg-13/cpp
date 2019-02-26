#-------------------------------------------------
# Project created by QtCreator 2019-01-22T17:43:27
#-------------------------------------------------

WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
LIB = ../../_lib

QT += core gui widgets
CONFIG += c++14

TARGET = Mark
TEMPLATE = app

include($$SRV/Srv/Srv.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    FMain.h         \
    WAttention.h    \

SOURCES += \
    main.cpp        \
    FMain.cpp       \
    WAttention.cpp  \

FORMS += \
    FMain.ui        \
    WAttention.ui   \

RESOURCES += \
    res.qrc         \

LIBS += \
    -lUser32

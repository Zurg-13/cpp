#-------------------------------------------------
# Project created by QtCreator 2017-09-01T18:13:16
#-------------------------------------------------

WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt
LIB = ../../_lib

QT += core gui widgets #multimedia
CONFIG += c++14

TARGET = TestQt
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$SRV/Log/Log.pri)
include($$WGT/Log/WLog.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    FMain.h         \
    WAnimation.h    \
    WSound.h        \
    FProc.h         \
    WTextBrowzer.h \
    WAttention.h

SOURCES += \
    main.cpp        \
    FMain.cpp       \
    WAnimation.cpp  \
    WSound.cpp      \
    FProc.cpp       \
    WTextBrowzer.cpp \
    WAttention.cpp

FORMS += \
    FMain.ui        \
    WAnimation.ui   \
    WSound.ui       \
    FProc.ui        \
    WTextBrowzer.ui \
    WAttention.ui
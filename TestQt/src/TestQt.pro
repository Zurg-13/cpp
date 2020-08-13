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
include($$WGT/Brd/WBrd.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
    FMain.h         \
    WAnimation.h    \
    WBall.h \
    WSound.h        \
    FProc.h         \
    WTextBrowzer.h \
    WAttention.h \
    WActive.h

SOURCES += \
    WBall.cpp \
    main.cpp        \
    FMain.cpp       \
    WAnimation.cpp  \
    WSound.cpp      \
    FProc.cpp       \
    WTextBrowzer.cpp \
    WAttention.cpp \
    WActive.cpp

FORMS += \
    FMain.ui        \
    WAnimation.ui   \
    WBall.ui \
    WSound.ui       \
    FProc.ui        \
    WTextBrowzer.ui \
    WAttention.ui \
    WActive.ui

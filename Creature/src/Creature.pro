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

INCLUDEPATH += ../lib/boost_1_72_0/
INCLUDEPATH += ../lib/eigen-3.3.8/
INCLUDEPATH += ../../
DEPENDPATH  += ../../


HEADERS += \
    tqdm.hpp    \
    two_dim_rd.h        \
    reaction_fitzhugh_nagumo.h \
    reaction_barkley.h \
    reaction_system.h   \
    env.h       \
    ver.h       \
    Part.h      \
    FMain.h     \

SOURCES += \
    two_dim_rd.cpp      \
    reaction_fitzhugh_nagumo.cpp \
    reaction_barkley.cpp \
    reaction_system.cpp \
    main.cpp    \
    Part.cpp    \
    FMain.cpp   \

FORMS += \
    FMain.ui    \

RESOURCES += \
    res.qrc     \

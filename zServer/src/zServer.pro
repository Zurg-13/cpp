#-------------------------------------------------
# Project created by QtCreator 2017-06-06T18:19:23
#-------------------------------------------------

QT += core gui widgets network concurrent
CONFIG += c++14

TARGET = zServer
TEMPLATE = app

SOURCES += \
    main.cpp        \
    FMain.cpp       \
    WHandler.cpp    \
    DHeader.cpp

HEADERS += \
    FMain.h         \
    WHandler.h      \
    DHeader.h

FORMS += \
    FMain.ui        \
    WHandler.ui     \
    DHeader.ui

RESOURCES += \
    res.qrc


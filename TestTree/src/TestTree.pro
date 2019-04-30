#-------------------------------------------------
# Project created by QtCreator 2018-04-27T18:44:06
#-------------------------------------------------

DLG = ../../_dlg
WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt

QT += core gui widgets xml sql
CONFIG += c++14

TARGET = TestTree
TEMPLATE = app

include($$SRV/Srv.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

HEADERS += \
        FMain.h                 \
        unt/FileSystemModel.h   \
        unt/ListTreeModel.h     \
    unt/DomTreeModel.h

SOURCES += \
        main.cpp                \
        FMain.cpp               \
        unt/FileSystemModel.cpp \
        unt/ListTreeModel.cpp   \
    unt/DomTreeModel.cpp

FORMS += \
        FMain.ui                \

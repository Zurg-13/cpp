#-------------------------------------------------
# Project created by QtCreator 2016-10-18T18:12:40
#-------------------------------------------------

DLG = ../../_dlg
WGT = ../../_wgt
SRV = ../../_srv
UNT = ../../_unt

QT += core gui widgets sql
CONFIG += c++14

TARGET = zQuery
TEMPLATE = app

include($$SRV/Srv/Srv.pri)
include($$SRV/Sql/Sql.pri)
include($$WGT/Log/WLog.pri)
include($$DLG/Sel/DSel.pri)

INCLUDEPATH += ../../
DEPENDPATH  += ../../

SOURCES += \
    main.cpp            \
    FMain.cpp           \
    SqlHighlighter.cpp  \
    wgt/WEdit.cpp       \
    wgt/WOut.cpp        \
    wgt/WCnn.cpp        \
    wgt/WConnect.cpp    \

HEADERS += \
    FMain.h             \
    SqlHighlighter.h    \
    wgt/WEdit.h         \
    wgt/WOut.h          \
    wgt/WCnn.h          \
    wgt/WConnect.h      \

FORMS += \
    FMain.ui            \
    wgt/WEdit.ui        \
    wgt/WOut.ui         \
    wgt/WCnn.ui         \
    wgt/WConnect.ui     \

RESOURCES += \
    res.qrc             \
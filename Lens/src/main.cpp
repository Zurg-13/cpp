// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QApplication>

#include "_wgt/Log/WLog.h"

#include "FMain.h"
#include "WLens.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
FMain *fmMain;
WLog *wgLog;
WLens * wgLens;

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    WLog _wgLog; wgLog = &_wgLog;
    WLens _wgLens; wgLens = &_wgLens;

    FMain _fmMain; fmMain = &_fmMain;
    fmMain->show();

    return app.exec();
}// main

//------------------------------------------------------------------------------


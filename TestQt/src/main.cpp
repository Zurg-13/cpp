// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QApplication>

#include "FMain.h"
#include "WAnimation.h"
#include "WSound.h"
#include "FProc.h"
#include "WTextBrowzer.h"
#include "WAttention.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
FMain *fmMain;
WAnimation *wgAnimation;
WSound *wgSound;
FProc *fmProc;
WTextBrowzer *wgTextBrowzer;
WAttention *wgAttention;

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WAnimation _wgAnimation; wgAnimation = &_wgAnimation;
    WSound _wgSound; wgSound = &_wgSound;
    FProc _fmProc; fmProc = &_fmProc;
    WTextBrowzer _wgTextBrowzer; wgTextBrowzer = &_wgTextBrowzer;
    WAttention _wgAttention; wgAttention = &_wgAttention;

    FMain _fmMain; fmMain = &_fmMain; fmMain->show();
    return app.exec();
}// main

//------------------------------------------------------------------------------

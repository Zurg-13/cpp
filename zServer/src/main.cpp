#include <QApplication>
#include <QMimeDatabase>

#include "FMain.h"
#include "DHeader.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
QMimeDatabase *mime;

FMain *fmMain;
DHeader *dgHeader;


// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMimeDatabase _mime; mime = &_mime;

    DHeader _dgHeader; dgHeader = &_dgHeader;
    FMain _fmMain; fmMain = &_fmMain;
    fmMain->show();

    return app.exec();
}// main

//------------------------------------------------------------------------------

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>

#include "WLog.h"
#include "FMain.h"
#include "DSel.h"

#include "wgt/WCnn.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
WLog *wgLog;
DSel *dgSel;
WCnn *wgCnn = nullptr; // Инициализируется в конструкторе FMain.

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Установить кодек текста.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Создать класс приложения.
    QTranslator *translator = new QTranslator;
    translator->load(QString(":/tr/qtbase_ru.qm"));
    app.installTranslator(translator);

    // Создать формы.
    DSel _dgSel; dgSel = &_dgSel;
    WLog _wgLog; wgLog = &_wgLog;
    FMain fmMain;
    fmMain.show();

    // Запуск приложения.
    return app.exec();
}// main

//------------------------------------------------------------------------------


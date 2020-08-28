#include <QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "env.h" // Глобальная среда приложения.

#include "Item.h"

// Инициализация статических переменных среды. ---------------------------------
//------------------------------------------------------------------------------
unsigned int E::port = E::PORT;
QMimeDatabase* E::mime;

WBrd* E::Log;
FMain* E::Main;

int Item::count = 0;

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMimeDatabase mime; E::mime = &mime;

    // Установить кодек текста.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Создать класс приложения.
    QTranslator *translator = new QTranslator;
    translator->load(QString(":/tr/qtbase_ru.qm"));
    app.installTranslator(translator);

    WBrd wgLog; E::Log = &wgLog; E::Log->show();
    FMain fmMain; E::Main = &fmMain; E::Main->show();

    return app.exec();
}// main

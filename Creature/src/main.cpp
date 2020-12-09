
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "env.h" // Глобальная среда приложения.
#include "ver.h"



// Инициализация статических переменных среды. ---------------------------------
//------------------------------------------------------------------------------
WBrd* E::Log;
FMain* E::Main;



// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QApplication::setApplicationName("z-menu");
    QApplication::setApplicationVersion(
        MAJOR_STR "." MINOR_STR "." REVISION_STR );

    // Установить кодек текста.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Создать класс приложения.
    QTranslator *translator = new QTranslator;
    translator->load(QString(":/tr/qtbase_ru.qm"));
    app.installTranslator(translator);

    WBrd wgBrd; E::Log = &wgBrd;
    FMain fmMain; E::Main = &fmMain; E::Main->show();

    return app.exec();
}// main

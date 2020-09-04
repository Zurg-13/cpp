#include <QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "env.h" // Глобальная среда приложения.
#include "ver.h"

#include "Item.h"

// Инициализация статических переменных среды. ---------------------------------
//------------------------------------------------------------------------------
unsigned int E::port = E::PORT;
QMimeDatabase* E::mime;
QSqlDatabase* E::sldb;

WBrd* E::Log;
FMain* E::Main;

int Item::count = 0;

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("z-menu");
    QApplication::setApplicationVersion(
        MAJOR_STR "." MINOR_STR "." REVISION_STR );

    QMimeDatabase mime; E::mime = &mime;
    QSqlDatabase sldb = QSqlDatabase::addDatabase("QSQLITE"); E::sldb = &sldb;
        sldb.setDatabaseName("db_name.sqlite");

    // Установить кодек текста.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Создать класс приложения.
    QTranslator *translator = new QTranslator;
    translator->load(QString(":/tr/qtbase_ru.qm"));
    app.installTranslator(translator);

    WBrd wgLog; E::Log = &wgLog;
    FMain fmMain; E::Main = &fmMain; E::Main->show();

    return app.exec();
}// main

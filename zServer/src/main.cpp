#include <QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "env.h" // Глобальная среда приложения.

// Инициализация статических переменных среды. ---------------------------------
//------------------------------------------------------------------------------
unsigned int E::port = E::PORT;
QMimeDatabase* E::mime;

FMain* E::Main;
DHeader* E::Header;

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMimeDatabase _mime; E::mime = &_mime;

    // Установить кодек текста.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Создать класс приложения.
    QTranslator *translator = new QTranslator;
    translator->load(QString(":/tr/qtbase_ru.qm"));
    app.installTranslator(translator);

    DHeader dgHeader; E::Header = &dgHeader;
    FMain fmMain; E::Main = &fmMain;
    E::Main->show();

    return app.exec();
}// main

//------------------------------------------------------------------------------

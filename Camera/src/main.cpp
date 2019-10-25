#include <QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "env.h" // Глобальная среда приложения.

// Инициализация статических переменных среды. ---------------------------------
//------------------------------------------------------------------------------
FMain* E::Main;

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

    FMain fmMain; E::Main = &fmMain;
    E::Main->show();

    return app.exec();
}// main

#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDebug>

#include "WAttention.h"
#include "FMain.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
WAttention *wgAttention;
FMain *fmMain;

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
    WAttention _wgAttention; wgAttention = &_wgAttention;
    FMain _fmMain; fmMain = &_fmMain;
    fmMain->show();

/*
    app.installNativeEventFilter(&fmMain->rawEvt);
*/

/*
    UINT nDevices;
    PRAWINPUTDEVICELIST pRawInputDeviceList;

    if(!GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST))) {
        qDebug() << "ERROR -- GetRawInputDeviceList ...";
        return 1;
    }

    if(!(pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)))
    {
      qDebug() << "Initialization failed...";
      return 1;
    }
*/

/*
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = reinterpret_cast<HWND>(fmMain->effectiveWinId());
    if(!RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])))
        { qDebug() << "Huston Problem."; }

    qDebug() << QString::number(GetLastError());
*/

    // Запуск приложения.
    return app.exec();

}// main

//------------------------------------------------------------------------------


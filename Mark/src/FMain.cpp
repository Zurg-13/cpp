// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
#include <QTime>

#include "_srv/Srv/dbg.h"
#include "_srv/Srv/std.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "WAttention.h"


// Обработчик "сырых" событий. -------------------------------------------------
//------------------------------------------------------------------------------
bool RawEvent::nativeEventFilter(
    const QByteArray &eventType, void *message, long* /* result */)
{
    if(eventType != "windows_generic_MSG") { return false; }

    MSG *msg = reinterpret_cast<MSG*>(message);
    if(msg->message == WM_INPUT) {
        UINT dwSize = 40;
        static BYTE lpb[40];
        if(!GetRawInputData(
            reinterpret_cast<HRAWINPUT>(msg->lParam)
          , RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER) )
        ) {
            DBG << "Error GetRawInputData";
        } else {
            RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);
            if(raw->header.dwType == RIM_TYPEMOUSE) {

            DBG << "cde:" << raw->data.keyboard.MakeCode
                << "flg:" << raw->data.keyboard.Flags
                << "rvt:" << raw->data.keyboard.Reserved
                << "key:" << raw->data.keyboard.VKey
                << "msg:" << raw->data.keyboard.Message
                << "ext:" << raw->data.keyboard.ExtraInformation
                << "tme:" << QTime::currentTime().second() << QTime::currentTime().msec();

            }// if(raw->header.dwType == RIM_TYPEMOUSE)
        }// else
    }// if(msg->message == WM_INPUT)

    return false;
}// nativeEventFilter


// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern FMain *fmMain;
extern WAttention *wgAttention;

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);

    //Регистрация глобальной горячей клавиши (WinAPI).
    RegisterHotKey(
        reinterpret_cast<HWND>(FMain::winId())
      , 100                     // Учтановка идентификатора HotKey
      , MOD_ALT | MOD_NOREPEAT  // Установка модификаторов
      , 'Z');                   // Определить Горячую Клавишу для HotKey

}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}// ~FMain

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Отладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    DBG << R"(/bgn)";


    QTime bgn = QTime::currentTime();
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) != 0) {

        if(msg.message == WM_HOTKEY) {
            DBG << "| msg:" << msg.message << ", prm:" << msg.wParam
                << ", tme:" << QTime::currentTime().msecsSinceStartOfDay();
        }

        if(bgn.secsTo(QTime::currentTime()) >= 5) { break; }
    }


    DBG << R"(\end)";
}// on_aTest_triggered

// Добавить. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btAdd_clicked() {
    wgAttention->show();
}// on_btAdd_clicked


// Нативный поток событий. -----------------------------------------------------
//------------------------------------------------------------------------------
bool FMain::nativeEvent(const QByteArray &type, void *message, long*) {
    if(type != "windows_generic_MSG") { return false; }

//    MSG *msg = static_cast<MSG*>(message);
    MSG *msg = (MSG*)(message);

    DBG << "msg:" << msg->message << "prm:" << msg->wParam
        << "tme:" << QTime::currentTime().msecsSinceStartOfDay();

    return false;
}// nativeEvent


//------------------------------------------------------------------------------


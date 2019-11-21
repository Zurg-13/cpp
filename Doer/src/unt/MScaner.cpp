// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "env.h" // Глобальная среда приложения.
#include "std.h"
#include "dbg.h"
#include "xml.h"
#include "tme.h"

#include "Event.h"
#include "MScaner.h"

// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define LG E::log
#define INF(m) if(LG) { LG->add("SCAN-INF", STR(SYSDATE) + STR(": ") + m); }
#define ERR(m) if(LG) { LG->add("SCAN-ERR", STR(SYSDATE) + STR(": ") + m); }
#define FCE(m) E::Log->inf(STR(SYSDATE) + ": " + m)

// Константы. ------------------------------------------------------------------
//------------------------------------------------------------------------------
const int BUFSIZE = 2048;

/* Дополнительные функции. ****************************************************/
/******************************************************************************/

uint NUM(QString str) {
    return (str.contains("0x",Qt::CaseInsensitive)
        ? str.mid(2).toUInt(nullptr, 16)
        : str.toUInt(nullptr, 10) );
}// NUM

// Сшить буфер, выбросив из него разметку на чанки. ----------------------------
//------------------------------------------------------------------------------
QByteArray SEW(const QByteArray& arr) {
    QByteArray ret; ret.reserve(arr.size());

    int cnt = 0, end = arr.size();
    while(cnt < end) {

        if(arr[cnt] == 0x02) {  /* ReportID == 0x02. */
            cnt++;              /* Length of the output data. */
            int sze = arr[cnt]; /* Size data chunk. */
            cnt++;              /* Begin data chunk. */
            for(int i=cnt; i < cnt+sze; i++) { ret.append(arr[i]); }
            cnt += sze + 5;     /* End data chunk. */
            if(arr[cnt] == 0x00) { break; }
        }// if(arr[cnt] == 0x02)

        cnt++;
    }// while(cnt < end)

    return ret;
}// SEW


/* MScaner. *******************************************************************/
/******************************************************************************/

// Обработчик команд. ----------------------------------------------------------
//------------------------------------------------------------------------------
Result MScaner::wrk(Command command) {
    if(NOT(this->ready)) { return Result(); }
    FNC << "cmd:" << command.toString();

    // Маршрутизация.
    switch(this->cmd[command.name].toInt()) {

     // scan_qrcode
     case 1: return Result(
        command.ref, STR("err")
      , "try scan_qrcode" );

     // scan_status
     case 2: return Result(
        command.ref, STR("err")
      , "status" );

    }// switch(this->cmd[cmd].toInt())

    return Result();
}// wrk

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
MScaner::MScaner(const QString &cfg, QObject *prn) : QObject(prn), Module() {

    // Инициализация.
    this->init(cfg);
    this->usb = new QUsbDevice();

    // Подключить сигналы/слоты.
    connect(&tmr, &QTimer::timeout, this, &MScaner::on_poll); // Опрос.

}// MScaner

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
MScaner::~MScaner(void) {
    delete usb;
}//~MScaner

// Задать конфигурацию. --------------------------------------------------------
//------------------------------------------------------------------------------
void MScaner::init(const QString& cfg) {
    auto REQ = [=](const QString &nme) -> uint {
        const QString msg("Отсутствует обязательный параметр: %1");
        QString ret = VAL(nme, cfg);
        if(ret.isEmpty()) { E::Log->err(msg.arg(nme)); ERR(msg.arg(nme)); }
        return NUM(ret);
    };// REQ

    fltr.vid = static_cast<quint16>(REQ("vid"));
    fltr.pid = static_cast<quint16>(REQ("vid"));

    conf.alternate = static_cast<quint8>(REQ("alternate"));
    conf.config    = static_cast<quint8>(REQ("config"));
    conf.interface = static_cast<quint8>(REQ("interface"));
    conf.readEp    = static_cast<quint8>(REQ("read_ep"));
    conf.writeEp   = static_cast<quint8>(REQ("write_ep"));
}// init

// Запустить службу. -----------------------------------------------------------
//------------------------------------------------------------------------------
void MScaner::open(void) {
    this->tmr.setInterval(0); this->tmr.start(); // Пуск опроса.
    this->ready = false;
}// open

// Остановить службу. ----------------------------------------------------------
//------------------------------------------------------------------------------
void MScaner::close(void){
    this->tmr.stop(); // // Остановка опроса.
    this->usb_close(); this->ready = false;
}// close

// Сканировать. ----------------------------------------------------------------
//------------------------------------------------------------------------------
QString MScaner::scan(void) {
    QByteArray arr;
    usb_read(&arr);
    return SEW(arr);
}// scan

// Состояние. ------------------------------------------------------------------
//------------------------------------------------------------------------------
QString MScaner::info(void) {
    return QString(); //todo
}// info

// Открыть USB-устройство. -----------------------------------------------------
//------------------------------------------------------------------------------
bool MScaner::usb_open(void) {

    if(this->usb) {
        this->usb->setConfig(this->conf);
        this->usb->setFilter(this->fltr);
        if(this->usb->open() == 0) { return true; }
    }// if(this->dev)

    return false;
}// usb_open

// Закрыть USB-устройство. -----------------------------------------------------
//------------------------------------------------------------------------------
void MScaner::usb_close(void) { this->usb->close(); }

// Считать данные с USB-устройства. --------------------------------------------
//------------------------------------------------------------------------------
void MScaner::usb_read(QByteArray *buf) { this->usb->read(buf, BUFSIZE); }

// Записать данные на USB-устройство. ------------------------------------------
//------------------------------------------------------------------------------
void MScaner::usb_write(QByteArray *buf)
    { usb->write(buf, static_cast<quint32>(buf->size())); }

// Опрос сканера. --------------------------------------------------------------
//------------------------------------------------------------------------------
void MScaner::on_poll(void) {
    QByteArray arr;

    if(this->isDeviceOpened) {
        usb_read(&arr);
    } else {

        if(this->usb_open()) {
            QString msg("Подключение к сканеру установлено.");
            FCE(msg); INF(msg);
            this->isDeviceOpened = true;
            this->tmr.setInterval(500*MSC);
        } else {
            QString msg("Не удалось подключиться к сканеру.");
            FCE(msg); ERR(msg);
            this->isDeviceOpened = false;
            this->tmr.setInterval(30*SEC);
        }// if(this->open())

    }// else // if(this->isDeviceOpened)

    // Отправка, если код считан.
    if(arr.length())
        { E::Main->evt_send(Event ("qr_scan", {{"cde", SEW(arr)}})); }

}// on_poll

//------------------------------------------------------------------------------


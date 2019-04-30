// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QMessageBox>
#include <QCommandLineParser>
#include <QTimer>
#include <QNetworkProxy>

#include "dbg.h"
#include "tme.h"
#include "std.h"

#include "ui_FMain.h"
#include "FMain.h"


// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define PR(n, v) qMakePair(QString(n), QString(v))

#define INF(m) if(log) { log->add("INF", STR(SYSDATE) + STR(": ") + m); }
#define ERR(m) if(log) { log->add("ERR", STR(SYSDATE) + STR(": ") + m); }

// Константы. ------------------------------------------------------------------
//------------------------------------------------------------------------------
const int BUFSIZE = 2048;
const int LOGSIZE = 1024 * 1024;

/* Дополнительные функции. ****************************************************/
/******************************************************************************/
uint NUM(QString str) {
    return (str.contains("0x",Qt::CaseInsensitive)
        ? str.mid(2).toUInt(nullptr, 16)
        : str.toUInt(nullptr, 10) );
}// NUM

// Оформить http-адрес. --------------------------------------------------------
//------------------------------------------------------------------------------
QString SURROUND(const QString &url, const QList<QPair<QString, QString>> &prm){
    QStringList ret;
    auto FSN = [](const QString &lft, const QString &rgt)
        -> QString { return lft + STR("=") + rgt; };

    ret.append("http://");
    ret.append(url);

    if(prm.size()) {
        QList<QPair<QString, QString>>::const_iterator
            it = prm.constBegin()
          , end = prm.constEnd();

        ret.append("?");
        ret.append(FSN(it->first, it->second));
        it++;
        while(it != end) {
            ret.append("&");
            ret.append(FSN(it->first, it->second));
            it++;
        }// while(it != end)

    }// if(prm.size())

    return ret.join("");
}// SURROUND

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
}// sew

/* FMain. *********************************************************************/
/******************************************************************************/

// Задать общие переменные. ----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::SET_PRM(QList<QString> args) {

    QCommandLineParser parser;

    auto HVE = [&parser](QString nme) -> bool { return  parser.isSet(nme); };
    auto VAL = [&parser](QString nme)
        -> QString { return parser.value(nme).trimmed(); };
    auto LOG = [=](QString nme)
        -> void {
            ui->wgLog->add(STR("%1: %2").arg(nme).arg(VAL(nme)));
            INF(STR("%1: %2").arg(nme).arg(VAL(nme)));
        };

    FNC << "args:" << args;

    parser.setApplicationDescription("Агент прослушивания сканера кодов.");

    parser.addOptions({
        {{"s", "show"}, tr("Показать интерфейс."), "show" }

      , {{"d", "dir"}, tr("Путь до папки логирования."), "dir" }
      , {{"u", "url"}, tr("Адрес сервиса приёма данных."), "url" }
      , {{"n", "num"}, tr("Номер терминала."), "num" }

      , {{"V", "vid"}, tr("Идентификатор производителя."), "vid" }
      , {{"p", "pid"}, tr("Идентификатор продукта."), "pid" }

      , {{"a", "alternate"}, tr("Номер набора данных."), "alternate" }
      , {{"c", "config"},    tr("Номер конфигурации."), "config" }
      , {{"i", "interface"}, tr("Номер интерфейса."), "interface" }
      , {{"r", "read_ep"},   tr("Адрес конечной точки чтения."), "read_ep" }
      , {{"w", "write_ep"},  tr("Адрес конечной точки записи."), "write_ep" }
    });
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(args);

    // show.
    if(HVE("show")) {
        if(VAL("show") == "true") { QTimer::singleShot(0, this, SLOT(show())); }
    }// if(HVE("show"))

    //log
    if(HVE("dir")) { log = new ZLogger(QDir(VAL("dir")), "medium", LOGSIZE); }

    // host.
    if(HVE("url")) {
        req.setUrl(QUrl(SURROUND(VAL("url"), { PR("id", VAL("num")) })));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    }// if(HVE("url"))

    // fltr.
    if(HVE("vid")) { fltr.vid = static_cast<quint16>(NUM(VAL("vid"))); }
    if(HVE("pid")) { fltr.pid = static_cast<quint16>(NUM(VAL("pid"))); }

    // conf.
    if(HVE("alternate"))
        { conf.alternate = static_cast<quint8>(NUM(VAL("alternate"))); }
    if(HVE("config"))
        { conf.config = static_cast<quint8>(NUM(VAL("config"))); }
    if(HVE("interface"))
        { conf.interface = static_cast<quint8>(NUM(VAL("interface"))); }
    if(HVE("read_ep"))
        { conf.readEp = static_cast<quint8>(NUM(VAL("read_ep"))); }
    if(HVE("write_ep"))
        { conf.writeEp = static_cast<quint8>(NUM(VAL("write_ep"))); }

    // Логирование.
    LOG("show");
    LOG("dir"); LOG("url");
    LOG("pid"); LOG("vid");
    LOG("alternate"); LOG("config"); LOG("interface");
    LOG("read_ep"); LOG("write_ep");

}// SET_PRM

// Опрос сканера. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_poll(void) {
    QByteArray arr;

    if(this->isDeviceOpened) {
        dev->read(&arr, BUFSIZE);
    } else {
        if(QtUsb::deviceOK  != this->usb->openDevice(dev, fltr, conf)) {
            QString msg("Не удалось подключиться к сканеру.");
            ui->wgLog->add(msg, Qt::red); ERR(msg);

            this->isDeviceOpened = false;
            this->timer.setInterval(30*SEC);
        } else {
            QString msg("Подключение к сканеру установлено.");
            ui->wgLog->add(msg, Qt::green); INF(msg);

            this->isDeviceOpened = true;
            this->timer.setInterval(500);
        }// if // if(QtUsb::deviceOK  != this->usb->openDevice(dev, fltr, conf))
    }// else // if(this->isDeviceOpened)

    if(arr.length()) {
        INF(STR("Отправка данных на: %1").arg(req.url().toString()));

        QNetworkReply *rpl = this->net.post(req, SEW(arr));
        connect(rpl
          , QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
          , [=](QNetworkReply::NetworkError)
            -> void {
                ui->wgLog->add(rpl->errorString(), Qt::red);
                ERR(STR("Ошибка отправки: %1").arg(rpl->errorString()));

                rpl->deleteLater();
            });
    }// if(arr.length())

}// on_poll

// Завершение запроса к сервису. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_reply(QNetworkReply *reply) { reply->deleteLater(); }

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);

    // Инициализация.
    SET_PRM(QCoreApplication::arguments());
    CONNECT();
    DEV_INIT();

    // Пуск опроса.
    this->timer.setInterval(0); this->timer.start();

}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete dev;
    delete log;
    delete ui;
}//~FMain

// Подключить сигналы/слоты. ---------------------------------------------------
//------------------------------------------------------------------------------
void FMain::CONNECT(void) {
    connect(&timer, &QTimer::timeout, this, &FMain::on_poll); // Опрос.
    connect(&net, &QNetworkAccessManager::finished, this, &FMain::on_reply);
}// CONNECT

// Инициализировать сканер. ----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::DEV_INIT(void) {
    this->usb = new QUsbManager(this);
    this->dev = new QUsbDevice();

//    this->dev->setDebug(true);
}// INITIAL

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Файл -> Проба. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    FNC << R"(/ bgn)";



    FNC << R"(\ end)";
}// on_aTest_triggered

// Начать/прекратить слежение за USB. ------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btNotifyUSB_clicked() {
    if(this->usb) {
        FNC << "stop";
        ui->wgLog->add("ntf: end");

        delete this->usb; this->usb = nullptr;
    } else {
        FNC << "start";
        ui->wgLog->add("ntf: bgn");

        this->usb = new QUsbManager(this);
        connect(
            this->usb, &QUsbManager::deviceInserted
          , this, &FMain::on_DevInsert );
        connect(
            this->usb, &QUsbManager::deviceRemoved
          , this, &FMain::on_DevRemove );
    }// else // if(this->mgr)

}// on_btNotifyUSB_clicked

// USB устройство подключено. --------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_DevInsert(QtUsb::FilterList lst) {
    DBG;
    FNC << "ok";
    ui->wgLog->add("ins: ok");

    for(QtUsb::DeviceFilter &flt: lst) {
        qDebug("VID_%04x / PID_%04x", flt.vid, flt.pid);
        FNC << STR("V: %1 / P: %2").arg(flt.vid).arg(flt.pid);
        FNC << STR("A: %1 / C: %2 / I: %3 / R: %4 / W: %5")
               .arg(flt.cfg.alternate).arg(flt.cfg.config)
               .arg(flt.cfg.interface)
               .arg(flt.cfg.readEp).arg(flt.cfg.writeEp);
        ui->wgLog->add(STR("dev: V: %1 / P: %2").arg(flt.vid).arg(flt.pid));
    }// flt

}// on_DevInsert

// USB устройство отключено. ---------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_DevRemove(QtUsb::FilterList lst) {
    FNC << "ok";

    for(QtUsb::DeviceFilter &flt: lst) {
        FNC << STR("V: %1 / P: %2").arg(flt.vid).arg(flt.pid);
    }// flt

}// on_DevRemove

// Инициализация устройства. ---------------------------------------------------
//------------------------------------------------------------------------------
void FMain::initDev() {
    FNC << "ok";

    // fltr
    fltr.vid = 0x1eab;
    fltr.pid = 0x0d10;

    // conf
    conf.alternate = 0;
    conf.config = 2;
    conf.interface = 0;
    conf.readEp = 0x84;
    conf.writeEp = 0x05;

}// initDev

// Открыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
bool FMain::openDev() {
    QtUsb::DeviceStatus ds = usb->openDevice(dev, fltr, conf);
    return (ds == QtUsb::deviceOK ? true : false);
}// openDev

// Закрыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
bool FMain::closeDev() {
    usb->closeDevice(dev);
    return false;
}// closeDev

// Считать данные с устройства. ------------------------------------------------
//------------------------------------------------------------------------------
void FMain::read(QByteArray *buf) { dev->read(buf, BUFSIZE); }

// Записать данные на устройство. ----------------------------------------------
//------------------------------------------------------------------------------
void FMain::write(QByteArray *buf)
    { dev->write(buf, static_cast<quint32>(buf->size())); }

// Записать. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btWrite_clicked() {
/*
    QByteArray send;

    send.append(static_cast<char>(0xAB));

    this->write(&send);
    ui->wgLog->add(STR("write:  %1").arg(QString::fromLatin1(send)));
*/
}// on_btWrite_clicked

// Открыть. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btOpen_clicked() {
    this->initDev();
    bool open = this->openDev();

    FNC << open;
    ui->wgLog->add((open ? "open: ok" : "open: no"));
}// on_btOpen_clicked

// Считать. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btRead_clicked() {
    QByteArray recv;
    this->read(&recv);

    ui->wgLog->add(STR("read:  %1").arg(QString::fromUtf8(SEW(recv))));
}// on_btRead_clicked

// Список USB устройств. -------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btListDev_clicked() {

    QtUsb::FilterList lst = this->usb->getPresentDevices();

    FNC << "list size:" << lst.size();

    for(QtUsb::DeviceFilter &flt: lst)
        { FNC << STR("V: %1 / P: %2").arg(flt.vid).arg(flt.pid); }

}// on_btListDev_clicked

//------------------------------------------------------------------------------


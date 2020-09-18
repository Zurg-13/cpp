// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringBuilder>

#include "dbg.h"
#include "std.h"


#include "WClientWS.h"
#include "ui_WClientWS.h"

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WClientWS::~WClientWS() {
    delete bd;
    delete ws;
    delete ui;
}//~WClientWS

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WClientWS::WClientWS(QWidget *parent) : QWidget(parent), ui(new Ui::WClientWS) {
    ui->setupUi(this);
    bd = new WBrd;
}// WClientWS

// Выполняется при показе. -----------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::showEvent(QShowEvent */*evt*/)
    { bd->place(this); bd->show(); }

// Пуск. -----------------------------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::on_btRun_clicked() {
    delete this->ws;
    this->ws = new QWebSocket("test-clt");

    connect(
        this->ws, &QWebSocket::connected
      , this, &WClientWS::ws_connect );
    connect(
        this->ws, &QWebSocket::disconnected
      , this, &WClientWS::ws_disconnect );
    connect(
        this->ws, QOverload<QAbstractSocket::SocketError>
                    ::of(&QWebSocket::error)
      , this, &WClientWS::ws_error );
    connect(
        this->ws, &QWebSocket::textMessageReceived
      , this, &WClientWS::ws_msg_rcv );
    connect(
        this->ws, &QWebSocket::binaryFrameReceived
      , this, &WClientWS::ws_bin_rcv );
    connect(
        this->ws, &QWebSocket::textFrameReceived
      , this, &WClientWS::ws_txt_rcv );

    this->ws->open(ui->edAdr->asTXT);
}// on_btRun_clicked

// WS: Подключение. ------------------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::ws_connect(void) {
    bd->post("connect");
}// ws_connect

// WS: Подключение разорвано. --------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::ws_disconnect(void) {
    bd->post("disconnect: " % ws->closeReason());
}// ws_disconnect

// WS: Ошибка. -----------------------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::ws_error(QAbstractSocket::SocketError err) {
    bd->post("error: " % QVariant::fromValue(err).asSTR)->clr(Qt::red);
}// ws_error

// WS: Получено текстовое сообщение. -------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::ws_msg_rcv(QString msg) {
    bd->post("msg: {" % msg % "}");
}// ws_msg_rcv

// WS: Получен двоичный фрагмент. ----------------------------------------------
//------------------------------------------------------------------------------
QByteArrayList bal;
QByteArray arr;
void WClientWS::ws_bin_rcv(const QByteArray &frm, bool last) {
    bd->post(
        "bin: { sze: " % STR(frm.size()) % ", "
      % "lst: " % (last ? "true" : "false") % "}")->clr(Qt::blue);

    bal.append(frm);
    if(last) {

        arr = bal.join();
        QPixmap pic; pic.loadFromData(arr, "JPG");
        ui->lbImg->setPixmap(pic);
        bal.clear();

        bd->post("bld: " % STR(arr.size()));
    } // if(last)

}// ws_bin_rcv

// WS: Получен символьный фрагмент. --------------------------------------------
//------------------------------------------------------------------------------
#include <QBuffer>
void WClientWS::ws_txt_rcv(const QString &frm, bool last) {
    bd->post(
        "txt: { sze: " % STR(frm.size()) % ", "
      % "lst: " % (last ? "true" : "false") % "}")->clr(Qt::green);

    QByteArray ba;
    QBuffer bf(&ba); bf.open(QIODevice::ReadWrite);
    QDataStream in(&bf);

    in << frm; bf.close();

    bal.append(ba);
    if(last) {

        arr = bal.join();
        QPixmap pic; pic.loadFromData(arr, "JPG");
        ui->lbImg->setPixmap(pic);
        bal.clear();

        bd->post("bld: " % STR(arr.size()));
    } // if(last)

}// ws_txt_rcv

// Стоп. -----------------------------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::on_btStp_clicked() {
    ws->close();
    delete ws; ws = nullptr;
}// on_btStp_clicked

// ОТправить. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void WClientWS::on_btSnd_clicked() {
    if(ws) {

        if(ws->isValid()) {
            ws->sendTextMessage(ui->edMsg->asTXT);
            bd->post("send: {" % ui->edMsg->asTXT % "}");
        } else {
            bd->post("WS не подключён")->clr(Qt::gray);
        }// else // if(ws->isValid())

    } else {
        bd->post("WS не запущен")->clr(Qt::gray);
    }// else // if(ws)
}// on_btSnd_clicked

//------------------------------------------------------------------------------


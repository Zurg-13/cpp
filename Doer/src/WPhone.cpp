// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QTimer>
#include <QFileDialog>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "tme.h"

#include "WPhone.h"
#include "ui_WPhone.h"


/* WPhone. ********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WPhone::WPhone(QWidget *parent) : QWidget(parent), ui(new Ui::WPhone) {

    // Внешний вид.
    ui->setupUi(this);

    // Инициализация.
    this->phone = new MPhone(ui->te->toPlainText(), nullptr);

}// WPhone

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WPhone::~WPhone() {
    delete phone;
    delete ui;
}//~WPhone

// Загрузить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::loadConf(const QString& fnme) {
    if(fnme.isEmpty()) { return; }
    QString conf = FLE(fnme);
    ui->te->setPlainText(conf);
    this->phone->init(conf);
}// loadConf

// Загрузить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btConfLoad_clicked() {
    loadConf(QFileDialog::getOpenFileName(
        this, "Открыть конфигурацию", APP_DIR, "*.xml" ));
}// on_btConfLoad_clicked

// Открыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btDevOpen_clicked() { this->phone->open(); }

// Закрыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btDevClose_clicked() { this->phone->close(); }

// Применить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btConfSet_clicked() { this->phone->init(ui->te->toPlainText());}

// Запросить информацию. -------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btInfo_clicked() {
    MPhone::Status stt = this->phone->info();
    E::Log->add(
        STR(SYSDATE) + " info status: "
          +( stt == MPhone::Status::ERR ? "error"
            :stt == MPhone::Status::BSY ? "call out"
            :stt == MPhone::Status::RDY ? "ready"
            :stt == MPhone::Status::RNG ? "ringing" : "???")
          + " / out: " + this->phone->lastOut()
          + " / err: " + this->phone->lastErr()
      , Qt::gray );
}// on_btInfo_clicked

// Завершить вызов. ------------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btStop_clicked() { this->phone->stop(); }

// Вызвать. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btCall_clicked() {
    E::Log->add(STR(SYSDATE) + " call bgn", Qt::gray);

    MPhone::Status stt = this->phone->call(ui->edNum->text());

    E::Log->add(
        STR(SYSDATE) + " call end: "
          +( stt == MPhone::Status::ERR ? "error"
            :stt == MPhone::Status::BSY ? "call out"
            :stt == MPhone::Status::RDY ? "ready"
            :stt == MPhone::Status::RNG ? "ringing" : "???")
      , Qt::gray );
}

//------------------------------------------------------------------------------



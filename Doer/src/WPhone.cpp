// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QTimer>
#include <QFileDialog>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "tme.h"
#include "xml.h"

#include "WPhone.h"
#include "ui_WPhone.h"


/* WPhone. ********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WPhone::WPhone(QWidget *parent) : QWidget(parent), ui(new Ui::WPhone) {

    // Внешний вид.
    ui->setupUi(this);
    ui->frExt->setVisible(false);

}// WPhone

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WPhone::~WPhone() {
    delete phone;
    delete ui;
}//~WPhone

// Задать конфигурацию. --------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::setConf(const QString& conf) {
    ui->te->setPlainText(conf);
    if(this->phone){ this->phone->init(conf); }
    else           { this->phone = new MPhone(conf, nullptr);}
}// loadConf

// Загрузить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btConfLoad_clicked() {
    [this](const QString& fnme)
        { if(fnme.isEmpty()) { return; } ui->te->setPlainText(FLE(fnme)); }
    (QFileDialog::getOpenFileName(
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
void WPhone::on_btConfSet_clicked()
    { this->phone->init(ui->te->toPlainText()); }

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

    QMap<QString, QString> map;
        map["target"] = ui->edNum->text();
    this->phone->call(Command("tst-call", "call_init", "call", map) );

    E::Log->add(STR(SYSDATE) + " call end", Qt::gray);
}// on_btCall_clicked

// Дополнительно. --------------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btExe_clicked() {

    if(ui->frExt->isVisible()) {
        ui->frExt->setVisible(false);
        ui->btExe->setStyleSheet("");
        ui->btExe->setText("exe");
    } else {
        ui->frExt->setVisible(true);
        ui->btExe->setText("exe [скрыть]");
        ui->btExe->setStyleSheet("color:red;");
    }// else // if(ui->frExt->isVisible())

}// on_pushButton_clicked

// Выполнить команду. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WPhone::on_btExtExe_clicked() {

    this->phone->EXE([](const QString &pfx, const QString &xml) {
        QList<Cmd> ret;
        for(const QString &command : LST("cmd", xml))
            { ret.append(Cmd(pfx, command)); }
        return ret;
    }(VAL("pfx", ui->te->toPlainText()), ui->teExt->toPlainText()));

}// on_btExtExe_clicked

//------------------------------------------------------------------------------


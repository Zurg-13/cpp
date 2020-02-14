// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QFileDialog>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"

#include "WScan.h"
#include "ui_WScan.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WScan::WScan(QWidget *parent) : QWidget(parent), ui(new Ui::WScan) {

    // Внешний вид.
    ui->setupUi(this);

    // Инициализация.
    this->scan = new MScaner(ui->te->toPlainText(), nullptr);

}// WScan

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WScan::~WScan() {
    delete scan;
    delete ui;
}//~WScan

// Загрузить конфиграцию. ------------------------------------------------------
//------------------------------------------------------------------------------
void WScan::loadConf(const QString& fnme) {
    QString conf = FLE(fnme);
    ui->te->setPlainText(conf);
    this->scan->init(conf);
}// loadConf

// Загрузить конфигурацию. ----------------------------------------------------
//-----------------------------------------------------------------------------
void WScan::on_btLoadConf_clicked() {
    loadConf(QFileDialog::getOpenFileName(
        this, "Открыть конфигурацию", APP_DIR, "*.xml" ));
}// on_btLoadConf_clicked

// Применить конфигурацию. ----------------------------------------------------
//-----------------------------------------------------------------------------
void WScan::on_btSetConf_clicked() { this->scan->init(ui->te->toPlainText()); }

// Запустить службу. ----------------------------------------------------------
//-----------------------------------------------------------------------------
void WScan::on_btOpen_clicked() { this->scan->open(); }

// Остановить службу. ---------------------------------------------------------
//-----------------------------------------------------------------------------
void WScan::on_btClose_clicked() { this->close(); }

// Список USB-устройств. -------------------------------------------------------
//-----------------------------------------------------------------------------
void WScan::on_btListDev_clicked() {

/* todo: отключено
    QtUsb::FilterList flt_l = this->scan->usb->getAvailableDevices();

    E::Log->add(STR("----------- USB -----------(%1)").arg(flt_l.size()));
    for(QtUsb::DeviceFilter &flt: flt_l) {

        QStringList str_l;
        QtUsb::ConfigList cfg_l = this->scan->usb->getAvailableConfigs(flt);
        for(QtUsb::DeviceConfig &cfg: cfg_l) {
            str_l.append(STR("cfg:%3 alt:%4 itf:%5 inp:%6 out:%7")
                .arg(cfg.config).arg(cfg.alternate).arg(cfg.interface)
                .arg(cfg.readEp).arg(cfg.writeEp) );
        }// cfg

        E::Log->add(STR("vid:%1 pid:%2 cfg:[%3]")
            .arg(flt.vid).arg(flt.pid).arg(str_l.join("  ")) );

    }// flt
    E::Log->add(STR("---------------------------"));
*/

}// on_btListDev_clicked

//-----------------------------------------------------------------------------


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QFileDialog>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "tme.h"
#include "xml.h"

#include "WPinpad.h"
#include "ui_WPinpad.h"

/* WPinpad. *******************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WPinpad::WPinpad(QWidget *parent) : QWidget(parent), ui(new Ui::WPinpad) {
    ui->setupUi(this);
}// WPinpad

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WPinpad::~WPinpad() {
    delete this->pinpad;
    delete ui;
}//~WPinpad

// Задать конфигурацию. --------------------------------------------------------
//------------------------------------------------------------------------------
void WPinpad::setConf(const QString &conf) {
    ui->te->setPlainText(conf);
    if(this->pinpad) { this->pinpad->init(conf); }
    else             { this->pinpad = new MPinpad(conf, nullptr); }
}// loadConf

// Загрузить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void WPinpad::on_btCfgLoad_clicked() {
    [this](const QString& fnme)
        { if(fnme.isEmpty()) { return; } ui->te->setPlainText(FLE(fnme)); }
    (QFileDialog::getOpenFileName(
        this, "Открыть файл конфигурации", APP_DIR, "*.xml" ));
}// on_btCfgLoad_clicked

// Открыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WPinpad::on_btDevOpen_clicked() { this->pinpad->open(); }

// Закрыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WPinpad::on_btDevClose_clicked() { this->pinpad->close(); }

// Применить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void WPinpad::on_btCfgSet_clicked()
    { this->pinpad->init(ui->te->toPlainText()); }

// Напечатать ZReport. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WPinpad::on_btZReport_clicked() {
    this->pinpad->zreport(
        Command("test", "zreport", "pinpad")
      , [](const Result &rslt) { E::Log->add("res:" + rslt.toString()); }
    );
}// on_btZReport_clicked

//------------------------------------------------------------------------------

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"

#include "WStart.h"
#include "ui_WStart.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WStart::WStart(QWidget *parent) : QWidget(parent), ui(new Ui::WStart) {
    ui->setupUi(this);
}// WStart

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WStart::~WStart() {
    delete ui;
}// ~WStart

// Загрузка конфигурации. ------------------------------------------------------
//------------------------------------------------------------------------------
void WStart::loadConf(const QString& fnme) {
    DBG << "заглушка"; //todo: заглушка
}// loadConf

//------------------------------------------------------------------------------

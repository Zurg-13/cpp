// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "ui_WActive.h"
#include "WActive.h"


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WActive::WActive(QWidget *parent) : QWidget(parent), ui(new Ui::WActive) {
    ui->setupUi(this);
}// WActive

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WActive::~WActive() {
    delete ui;
}//~WActive

//------------------------------------------------------------------------------

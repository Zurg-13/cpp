#include "ui_DHeader.h"
#include "DHeader.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
DHeader::DHeader(QWidget *parent) : QDialog(parent), ui(new Ui::DHeader) {
    ui->setupUi(this);
    this->setMaximumHeight(0);
}// DHeader

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
DHeader::~DHeader() {
    delete ui;
}// ~DHeader

// Выполняется при показе формы. -----------------------------------------------
//------------------------------------------------------------------------------
void DHeader::showEvent(QShowEvent*) {
    ui->edType->clear(); ui->edVal->clear();
    ui->edType->setFocus();
}// showEvent

//------------------------------------------------------------------------------
QString DHeader::type(void) { return ui->edType->text(); }
QString DHeader::value(void) { return ui->edVal->text(); }
QString DHeader::title(void) { return type() + QString(": ") + value(); }

//------------------------------------------------------------------------------

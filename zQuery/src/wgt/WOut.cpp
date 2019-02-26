// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "dbg.h"

#include "ui_WOut.h"
#include "WOut.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WOut::WOut(QWidget *parent) : QWidget(parent), ui(new Ui::WOut) {
    ui->setupUi(this);
    mdl = new QSqlQueryModel(this);

    ui->tv->setModel(mdl);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->resizeColumnsToContents();
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->horizontalHeader()->setStretchLastSection(true);
}// WOut

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WOut::~WOut() {
    DBG << "~WOut";

    delete ui;
}// ~WOut

// Установить запрос. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WOut::setQuery(QSqlQuery &qry){
    mdl->setQuery(qry);
    if(Qt::Checked == ui->cbAutoFit->checkState())
        { ui->tv->resizeColumnsToContents(); }
    ui->tv->resizeRowsToContents();
}// setQuery

// Переключение чекбокса "поджать". --------------------------------------------
//------------------------------------------------------------------------------
void WOut::on_cbAutoFit_clicked() {
    if(Qt::Checked == ui->cbAutoFit->checkState())
        { ui->tv->resizeColumnsToContents(); }
}// on_cbAutoFit_clicked

//------------------------------------------------------------------------------


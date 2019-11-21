#include "dbg.h"

#include "WLogEntry.h"
#include "ui_WLogEntry.h"

/* WLogEntry. *****************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry::WLogEntry(const QString &inp, const QString &out, QWidget *prn)
:   QWidget(prn), ui(new Ui::WLogEntry)
{
    ui->setupUi(this);
    ui->edInp->setText(inp); ui->edOut->setText(out);
//    this->setMaximumHeight(0);
}// WLogEntry

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry::~WLogEntry() {
    delete ui;

    FNC << "end";
}//~WLogEntry

// Выделить запись цветом. -----------------------------------------------------
//------------------------------------------------------------------------------
void WLogEntry::highlight(const QColor &clr) {
    static QString STYLE_BTM(
        "border-top: 0px;"
        "border-left: 2px solid %1;"
        "border-bottom : 2px solid %1;"
        "border-right: 2px solid %1;" );
    static QString STYLE_TOP(
        "border-top: 2px solid %1;"
        "border-left: 2px solid %1;"
        "border-bottom : 0px;"
        "border-right: 2px solid %1;" );

    ui->edOut->setStyleSheet(STYLE_TOP.arg(clr.name()));
    ui->edInp->setStyleSheet(STYLE_BTM.arg(clr.name()));
}// highlight

// Записать ввод. --------------------------------------------------------------
//------------------------------------------------------------------------------
void WLogEntry::inp(const QString &msg) { ui->edInp->setText(msg); }

// Записать вывод. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WLogEntry::out(const QString &msg) { ui->edOut->setText(msg); }

//------------------------------------------------------------------------------


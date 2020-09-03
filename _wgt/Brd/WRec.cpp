// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "dbg.h"
#include "tme.h"
#include "drw.h"

#include "WRec.h"
#include "ui_WRec.h"

/* WRec. **********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WRec::WRec(const QString &msg, const QString &adv, QWidget *prn)
    : QWidget(prn), ui(new Ui::WRec)
{
    ui->setupUi(this);
    ui->teAdv->setVisible(false);
    this->msg(msg); this->adv(adv); this->tme(SYSDATE);
}// WRec

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WRec::~WRec() {
    delete ui;
    FNC << "end";
}//~WRec

// Выделить запись цветом. -----------------------------------------------------
//------------------------------------------------------------------------------
void WRec::hgl(const QColor &clr) {
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

    ui->edMsg->setStyleSheet(STYLE_TOP.arg(clr.name()));
    ui->teAdv->setStyleSheet(STYLE_BTM.arg(clr.name()));
}// highlight

// Задать цвет записи. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WRec::clr(const QColor &clr) {
    const static QString CLR = "background-color: %1; color: %2";
    this->setStyleSheet(CLR.arg(clr.name()).arg(contrast_bw(clr).name()));
}// clr

// Записать сообщение. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WRec::msg(const QString &msg) { ui->edMsg->setText(msg); }

// Записать дополнительную информацию. -----------------------------------------
//------------------------------------------------------------------------------
void WRec::adv(const QString &adv) {
    ui->teAdv->setText(adv);
    ui->btAdv->setVisible(NOT(adv.isNull() || adv.isEmpty()));
}// adv

// Записать время. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WRec::tme(const QDateTime &tme) {
//    ui->edTme->setText(tme.toString("mm:ss"));
//    ui->edTme->setWhatsThis(tme.toString("yyyy.MM.dd HH:mm:ss z"));
    ui->edTme->setText(tme.toString("yyyy.MM.dd HH:mm:ss z"));
}// tme

// Показать / скрыть дополнительную информацию. --------------------------------
//------------------------------------------------------------------------------
void WRec::on_btAdv_clicked() {

    if(ui->teAdv->isVisible()) {
        ui->teAdv->setVisible(false);
        ui->btAdv->setText("++");
    } else {
        ui->teAdv->setVisible(true);
        ui->btAdv->setText("--");
    }// if(ui->teAdv->isVisible())

}// on_btAdv_clicked

//------------------------------------------------------------------------------


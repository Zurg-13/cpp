// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringBuilder>

#include "dbg.h"
#include "xml.h"
#include "lst.h"
#include "std.h"

#include "Item.h"
#include "ui_Item.h"


/* Item. **********************************************************************/
/******************************************************************************/

// Задать состояние. -----------------------------------------------------------
//------------------------------------------------------------------------------
void Item::set(const QString &xml) {
    ui->lbNote->setText(VAL("nme", xml));
    ui->lbNoteNote->setText(VAL("nte", xml));
    ui->lbPrice->setText(VAL("prc", xml));
    this->setEnabled(VAL("act", xml) == "0" ? false : true );
}// set

void Item::set(const QMap<QString, QString> &prm) {
    auto SET = [](QLabel *lbl, const QString &val)
        { if(NOT(val.isEmpty())) { lbl->setText(val); }};

    SET(ui->lbNote, prm["nme"]);
    SET(ui->lbNoteNote, prm["nte"]);
    SET(ui->lbPrice, prm["prc"]);
    this->setEnabled(prm["act"] == "0" ? false : true );
}// set

// Вернуть состояние. ----------------------------------------------------------
//------------------------------------------------------------------------------
QString Item::ELM(const QString &nme, const QString &val)
    { return '"' % nme % '"' % ":" % '"' % val % '"'; }
QString Item::jsn(void) {
    return "{" % BLD(QStringList({
        ELM("nme", ui->lbNote->text())
      , ELM("nte", ui->lbNoteNote->text())
      , ELM("prc", ui->lbPrice->text())
      , ELM("sel", (this->slct ? "1": "0"))
      , ELM("act", (this->isEnabled() ? "1" : "0"))
      , ELM("cnt", STR(this->cnt))
    })) % "}";
}// xml

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
Item::Item(QWidget *parent) : QWidget(parent), ui(new Ui::Item) {
    ui->setupUi(this); this->cnt = Item::count++;
}// Item

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
Item::~Item() {
    delete ui;

    FNC << "end";
}//~Item

// Нажатие кнопки выбора. ------------------------------------------------------
//------------------------------------------------------------------------------
void Item::on_btSel_clicked() {
    if(this->slct) {
        ui->btSel->setText("+");
        ui->btSel->setStyleSheet("background-color: white;");
        this->slct = false;
    } else {
        ui->btSel->setText("-");
        ui->btSel->setStyleSheet("background-color: limegreen;");
        this->slct = true;
    }// else // if(this->slct)
}// on_btSel_clicked

//------------------------------------------------------------------------------


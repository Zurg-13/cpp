// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QScrollArea>
#include <QLabel>

#include "dbg.h"
#include "std.h"
#include "wgt.h"
#include "drw.h"

#include "WLogBoard.h"
#include "ui_WLogBoard.h"

/* Дополнительные функции. ****************************************************/
/******************************************************************************/

/* WLogBoard. *****************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLogBoard::WLogBoard(QWidget *parent) : QWidget(parent), ui(new Ui::WLogBoard) {

    // Внешний вид.
    ui->setupUi(this);

    // Скроллинг.
    this->box = new QWidget(this);
        this->box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    QVBoxLayout *ly = new QVBoxLayout(this->box);
        ly->setContentsMargins(0, 0, 0, 0);
        ly->setSpacing(2);
    ui->sa->setWidget(this->box);

}// WLogBoard

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WLogBoard::~WLogBoard() {
    delete ui;
}//~WLogBoard

// Добавить разделитель. -------------------------------------------------------
//------------------------------------------------------------------------------
void WLogBoard::rift(void) {
    QFrame *frm = new QFrame();
        frm->setFrameShape(QFrame::HLine);
        frm->setFrameShadow(QFrame::Plain);
    static_cast<QBoxLayout*>(this->box->layout())
        ->insertWidget(this->inset_pos, frm);
}// rift

// Добавить ОДИНОЧНУЮ запись. --------------------------------------------------
//------------------------------------------------------------------------------
void WLogBoard::post(const QString &msg, const QColor &clr) {
    const static QString CLR = "background-color: %1; color: %2";
    QLabel* lbl = new QLabel(msg);
        lbl->setStyleSheet(CLR.arg(clr.name()).arg(contrast_bw(clr).name()));
    static_cast<QBoxLayout*>(this->box->layout())
        ->insertWidget(this->inset_pos, lbl);
}// post

// Добавить запись. ------------------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry* WLogBoard::post(WLogEntry *entry) {
    static_cast<QBoxLayout*>(this->box->layout())
        ->insertWidget(this->inset_pos, entry);
    return entry;
}// post

void WLogBoard::post(const QString &inp, const QString &out)
    { post(new WLogEntry(inp, out)); }

void WLogBoard::post(const QString &inp, const QString &out, const QColor &clr)
    { post(new WLogEntry(inp, out))->clr(clr); }

// Захватить виджет. -----------------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry* WLogBoard::grab(void) {
    WLogEntry *entry = post(new WLogEntry(EMPTY_STR, EMPTY_STR));
        entry->grab()->hgl(Qt::red);
    this->inset_pos++;
    return entry;
}// grab

// Освободить виджет. ----------------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry* WLogBoard::free(WLogEntry *entry)
    { this->inset_pos--; entry->hgl(Qt::green); return entry->free(); }

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void  WLogBoard::clear(void) {
    int pos = 0;
    while(QLayoutItem* itm = this->box->layout()->itemAt(pos)) {
        if(QWidget *wgt = itm->widget()) {
            QVariant is_grab = wgt->property("is_grab");
            if(is_grab.isValid() && is_grab.toBool())
                { pos++; continue; }
            else
                { delete wgt; delete this->box->layout()->takeAt(pos); }
        }// if(QWidget *wgt = itm->widget())
    }// while(QLayoutItem* itm = this->box->layout()->itemAt(pos))
}// clear

//------------------------------------------------------------------------------


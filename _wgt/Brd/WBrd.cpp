// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QScrollArea>
#include <QLabel>
#include <QMenu>

#include "dbg.h"

#include "WBrd.h"
#include "ui_WBrd.h"

/* WBrd. **********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WBrd::WBrd(QWidget *parent) : QWidget(parent), ui(new Ui::WBrd) {

    // Внешний вид.
    ui->setupUi(this);

    // Контекстное меню.
    ui->sa->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->sa, &QScrollArea::customContextMenuRequested
          , this, &WBrd::ctx_menu );

    // Скроллинг.
    this->box = new QWidget(this);
        this->box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    QVBoxLayout *ly = new QVBoxLayout(this->box);
        ly->setContentsMargins(0, 0, 0, 0);
        ly->setSpacing(2);
    ui->sa->setWidget(this->box);

}// WBrd

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WBrd::~WBrd() {
    delete ui;
}//~WBrd

// Добавить разделитель. -------------------------------------------------------
//------------------------------------------------------------------------------
void WBrd::rift(void) {
    QFrame *frm = new QFrame();
        frm->setFrameShape(QFrame::HLine);
        frm->setFrameShadow(QFrame::Plain);
    static_cast<QBoxLayout*>(this->box->layout())
        ->insertWidget(this->insert_pos, frm);
}// rift

// Добавить запись. ------------------------------------------------------------
//------------------------------------------------------------------------------
WRec* WBrd::post(WRec *rec) {
    static_cast<QBoxLayout*>(this->box->layout())
        ->insertWidget(this->insert_pos, rec);
    return rec;
}// post

WRec* WBrd::post(const QString &msg, const QString &adv)
    { return post(new WRec(msg, adv)); }

// Захватить виджет. -----------------------------------------------------------
//------------------------------------------------------------------------------
WRec* WBrd::grab(void) {
    WRec *rec = post(new WRec(EMPTY_STR, EMPTY_STR));
        rec->grab()->hgl(Qt::red);
    this->insert_pos++;
    return rec;
}// grab

// Освободить виджет. ----------------------------------------------------------
//------------------------------------------------------------------------------
WRec* WBrd::free(WRec *rec)
    { this->insert_pos--;  return rec->free(); }

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void  WBrd::clear(void) {
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

// Перехват вызова контекстного меню. ------------------------------------------
//------------------------------------------------------------------------------
void WBrd::ctx_menu(const QPoint &pos) {
    QMenu* menu = new QMenu();

//    menu->addAction(ui->aTest);
    menu->addAction(ui->aOver);
    menu->addSeparator(); //----------------
    menu->addAction(ui->aClear);
    menu->exec(ui->sa->mapToGlobal(pos));

    delete menu;
}// ctx_menu

// Разместить с привязкой. -----------------------------------------------------
//------------------------------------------------------------------------------
void WBrd::place(const QWidget *wgt, Place plc) {
    QRect loc = this->frameGeometry();
    QRect ext = wgt->frameGeometry();

    switch(plc) {
     case Place::top:
        this->move(ext.x(), ext.y()-loc.height());
        this->resize(wgt->width(), this->height()); break;

     case Place::btm:
        this->move(ext.x(), ext.y()+ext.height());
        this->resize(wgt->width(), this->height()); break;

     case Place::lft:
        this->move(ext.x()-loc.width(), ext.y());
        this->resize(this->width(), wgt->height()); break;

     case Place::rgt:
        this->move(ext.x()+ext.width(), ext.y());
        this->resize(this->width(), wgt->height()); break;

     case Place::ovr:
        this->move(ext.topLeft());
        this->resize(wgt->size()); break;

    }// switch(pos)

}// place

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WBrd::on_aClear_triggered() { clear(); }

// Проба. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void WBrd::on_aTest_triggered() {
    FNC << R"(/ bgn)";


    FNC << R"(\ end)";
}// on_aTest_triggered

// Порверх всех окон. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WBrd::on_aOver_triggered() {
    QAction *actn = SENDER(QAction*);

    if(windowFlags() & Qt::WindowStaysOnTopHint) { actn->setChecked(false); }
    else                                         { actn->setChecked(true); }

    this->setWindowFlags(windowFlags() ^ Qt::WindowStaysOnTopHint);
    this->show();
}// on_aOver_triggered

//------------------------------------------------------------------------------


// INCLUDE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include "_srv/Srv/dbg.h"

#include "ui_WBox.h"
#include "WBox.h"


// Добавить плашку. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WBox::addPlate(WClr *plate) {

    for(int i=ui->lyStd->count(); i >= this->size; i--) {
        WClr *plate = static_cast<WClr*>(ui->lyStd->itemAt(i-1)->widget());
        if(!plate->isSelect()) { plate->deleteLater(); }
    }// i

    (plate->isFixed() ? ui->lyFix : ui->lyStd)->insertWidget(0, plate);
    connect(plate, &WClr::remove, this, &WBox::remove_clr);
    connect(plate, &WClr::change_fix, this, &WBox::chahge_fix);
    connect(plate, &WClr::select_chg, this, &WBox::select_cng);

    if(!this->plate_sel) { this->plate_sel = plate; }
    if(plate->isSelect()) { this->select_cng(plate); }
}// addPlate

// Удалить контрол. ------------------------------------------------------------
//------------------------------------------------------------------------------
void WBox::remove_clr(WClr *clr) { delete clr; }

// Изменение фиксации. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WBox::chahge_fix(WClr *clr) {
    (clr->isFixed() ? ui->lyFix : ui->lyStd)->insertWidget(0, clr);
}// chahge_fix

// Выбор контрола. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WBox::select_cng(WClr *clr) {
    if(plate_sel) { plate_sel->setSelected(false); }
    clr->setSelected(true);
    plate_sel = clr;
}// select_clr

// Проверка на содержание цвета в контейнере. ----------------------------------
//------------------------------------------------------------------------------
bool WBox::contain(const QColor &clr) {
    static auto CONTAIN = [](QLayout *l, const QColor &c) -> bool {
        for(int i=0; i < l->count(); i++) {
            if(static_cast<WClr*>(l->itemAt(i)->widget())->color() == c)
                { return true; }
        }// i
        return false;
    };// CONTAIN

    return CONTAIN(ui->lyStd, clr) || CONTAIN(ui->lyFix, clr);
}// contain

// Выбрать цвет. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WBox::select(const QColor &clr) {
    static auto SELECT = [](QLayout *l, const QColor &c) -> void {
        for(int i=0; i < l->count(); i++) {
            WClr *p = static_cast<WClr*>(l->itemAt(i)->widget());
            if(p->color() == c) { p->setSelected(true); }
        }// i
    };// SELECT

    SELECT(ui->lyStd, clr); SELECT(ui->lyFix, clr);
}// select

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WBox::WBox(QWidget *parent) : QWidget(parent), ui(new Ui::WBox) {
    ui->setupUi(this);
}// WBox

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WBox::~WBox() {
    delete ui;
}//~WBox

//------------------------------------------------------------------------------


// INCLUDE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
//#include <QLayout>

#include "_srv/Srv/dbg.h"

#include "ui_WBox.h"
#include "WBox.h"


// Добавить плашку. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WBox::addPlate(WClr *plate) {
    (plate->isFixed() ? ui->lyFix : ui->lyStd)->insertWidget(0, plate);

    while(ui->lyStd->count() > this->size)
        { ui->lyStd->takeAt(ui->lyStd->count()-1)->widget()->deleteLater(); }

    connect(plate, &WClr::remove, this, &WBox::remove_clr);
    connect(plate, &WClr::change_fix, this, &WBox::chahge_fix);
    connect(plate, &WClr::select, this, &WBox::select_clr);
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
void WBox::select_clr(WClr *clr) {
    if(plate_sel) { plate_sel->setSelected(false); }
    clr->setSelected(true);
    plate_sel = clr;
}// select_clr

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


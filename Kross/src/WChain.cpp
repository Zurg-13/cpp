#include <QPropertyAnimation>

#include "_srv/std.h"
#include "_srv/dbg.h"

#include "ui_WChain.h"
#include "WChain.h"


// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern const QString CLR, RED, GRN, BLU;

/* Цепочка элементов. *********************************************************/
/******************************************************************************/

// Добавить элемент. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WChain::add(int val) {
    Plate *plate = new Plate(val, this);

    int x = xBgn + wSze/4;
    int y = yBgn + hSze/4;

    plate->setFont(this->font());
    plate->setFrameShape(QFrame::Box);
    plate->setGeometry(x, y, wSze/2, hSze/2);
    plate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    plate->show();
    plt.append(plate);

    redraw();
}// add

// Изъять элемент. -------------------------------------------------------------
//------------------------------------------------------------------------------
int WChain::grab(void) {
    if(plt.isEmpty()) return 0;

    Plate *plate = plt.last();
    int ret = plate->getVal();
    plt.removeLast(); delete plate;

    return ret;
}// grab

// Перерисовать. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WChain::redraw(void) {

    int cnt(0);
    for(QLabel *label: plt) {
        QPropertyAnimation *anm = new QPropertyAnimation(label, "geometry");

        int x = (this->*calc_x)(cnt);
        int y = (this->*calc_y)(cnt);

        anm->setDuration(1000);
        anm->setEasingCurve(QEasingCurve::OutCubic);
        anm->setEndValue(QRectF(x, y, wSze-2, hSze-2));
        anm->start(QAbstractAnimation::DeleteWhenStopped);

        cnt++;
    }// label

}// redraw

// Инициализация. --------------------------------------------------------------
//------------------------------------------------------------------------------
void WChain::init(int num, WChain::Direct direct) {
    this->num = num;
    this->direct = direct;
    this->calc_x = &WChain::calc_x_top_btm;

    switch(this->direct) {
      case Direct::TOP_TO_BTM:
        calc_y = &WChain::calc_y_top_btm; break;

      case Direct::BTM_TO_TOP:
        calc_y = &WChain::calc_y_btm_top; break;
    }// switch(direct)

}// init

// Перевыставить состояние. ----------------------------------------------------
//------------------------------------------------------------------------------
void WChain::reset() {
    del(); plt.clear();
    for(int i=0; i<this->num; i++) { add(); }
    recalc(); redraw();
}// reset

// Перерасчёт. -----------------------------------------------------------------
//------------------------------------------------------------------------------
void WChain::recalc(void) {
    wSze = wdh / 20.0; hSze = hgt / 5.0;
    xBgn = (this->*calc_x)(num); yBgn = (this->*calc_y)(num);
    xEnd = (this->*calc_x)(0); yEnd = (this->*calc_y)(0);
}// recalc

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WChain::WChain(QWidget *parent) : QWidget(parent), ui(new Ui::WChain) {
    ui->setupUi(this);
    direct = Direct::NONE;
//    init(num, direct);
}// WChain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WChain::~WChain() {
    delete ui;
}// ~WChain

// Изменение размера. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WChain::resizeEvent(QResizeEvent *evt) {
    if(direct == Direct::NONE) return;
    wdh = evt->size().width(); hgt = evt->size().height();
    recalc(); redraw();
}// resizeEvent

//------------------------------------------------------------------------------

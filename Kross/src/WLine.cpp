#include <QPropertyAnimation>

#include "std.h"
#include "dbg.h"

#include "ui_WLine.h"
#include "WLine.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern const QString CLR, RED, GRN, BLU;

// Захватить. ------------------------------------------------------------------
//------------------------------------------------------------------------------
int WLine::grab(QList<int> idx) {
    int ret(0);

    for(int index: idx) {
        QLabel *label = lbl.at(index);

        ret += INT(label->text());  // todo: Расшитить класс QLabel.

        lbl.removeAt(index); delete label;
        add();
    }// index

    return ret;
}// grab

// Задать состояние. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::init(WChain *chain, int num, Direct direct)
    { this->chain = chain; this->num = num; set(direct); reset(); }

// Сбросить состояние. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::reset(void) {
    del(); lbl.clear();
    for(int i=0; i<num; i++) { add(); }
}// reset

// Добавить элемент. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::add() {
    QLabel *label = new QLabel(this);

    int x = xBgn + wSze/4;
    int y = yBgn + hSze/4;
    int val = this->chain->grab();

    label->setText(STR(val));
    label->setStyleSheet(CLR +
        (val == 0 ? BLU
        :val >  0 ? GRN : RED) + ";");

    label->setFont(this->font());
    label->setFrameShape(QFrame::Box);
    label->setGeometry(x, y, wSze/2, hSze/2);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->show();
    lbl.append(label);

    redraw();
}// add

// Перерисовать. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::redraw(void) {

    int cnt(0);
    for(QLabel *label : lbl) {
        QPropertyAnimation *anm = new QPropertyAnimation(label, "geometry");
        connect(anm, SIGNAL(finished()), this, SLOT(finished()));

        int x = (this->*calc_x)(cnt);
        int y = (this->*calc_y)(cnt);

        anm->setDuration(1000);
        anm->setEasingCurve(QEasingCurve::OutCubic);
        anm->setEndValue(QRectF(x, y, wSze-2, hSze-2));
        anm->start(QAbstractAnimation::DeleteWhenStopped);

        cnt++;
    }// label

}// redraw

// Задать направление. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::set(Direct direct) {
    this->direct = direct;

    switch(this->direct) {
      case Direct::LFT_TO_RGT:
        calc_w = &WLine::calc_w_hor; calc_h = &WLine::calc_h_hor;
        calc_x = &WLine::calc_x_lft_rgt; calc_y = &WLine::calc_y_lft_rgt;
        break;

      case Direct::RGT_TO_LFT:
        calc_w = &WLine::calc_w_hor; calc_h = &WLine::calc_h_hor;
        calc_x = &WLine::calc_x_rgt_lft; calc_y = &WLine::calc_y_rgt_lft;
        break;

      case Direct::TOP_TO_BTM:
        calc_w = &WLine::calc_w_vrt; calc_h = &WLine::calc_h_vrt;
        calc_x = &WLine::calc_x_top_btm; calc_y = &WLine::calc_y_top_btm;
        break;

      case Direct::BTM_TO_TOP:
        calc_w = &WLine::calc_w_vrt; calc_h = &WLine::calc_h_vrt;
        calc_x = &WLine::calc_x_btm_top; calc_y = &WLine::calc_y_btm_top;
        break;

    }// switch(this->direct)

    recalc(); redraw();
}// set

// Анимация завершена. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::finished(void) {

}// finished

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLine::WLine(QWidget *parent) : QWidget(parent) {
    ui->setupUi(this);
    direct = Direct::NONE; off = 2;
}// WLine

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WLine::~WLine() {
    del();
    delete ui;
}// ~WLine

// Пересчёт опорных констант. --------------------------------------------------
//------------------------------------------------------------------------------
void WLine::recalc(void){
    wSze = (this->*calc_w)(); hSze = (this->*calc_h)();
    xBgn = (this->*calc_x)(num); yBgn = (this->*calc_y)(num);
    xEnd = (this->*calc_x)(0); yEnd = (this->*calc_y)(0);
}// recalc

// Изменение размера. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::resizeEvent(QResizeEvent *evt) {
    if(direct == Direct::NONE) return;
    wdh = evt->size().width(); hgt = evt->size().height();
    recalc(); redraw();
}// resizeEvent

// Срабатывание таймера. -------------------------------------------------------
//------------------------------------------------------------------------------
void WLine::onTimer() {
    DBG << "onTimer";

//    QTimer::singleShot(2000, this, SLOT(onTimer()));
}// onTimer

//------------------------------------------------------------------------------


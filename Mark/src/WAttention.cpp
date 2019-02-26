// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QTime>
#include <QMouseEvent>
#include <QTimer>

#include "_srv/Srv/dbg.h"
#include "_srv/Srv/std.h"

#include "ui_WAttention.h"
#include "WAttention.h"


// Срабатывание таймера. -------------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::on_timer(void) {
    bool visible = this->geometry().contains(QCursor::pos());
    ui->lbTop->setVisible(visible);
    ui->btCls->setVisible(visible);
    ui->btExt->setVisible(visible);
}// on_timer

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WAttention::WAttention(QWidget *parent)
  : QWidget(parent), ui(new Ui::WAttention)
{
    // Внешний вид.
    ui->setupUi(this);
    this->draw_ext = &WAttention::draw_free;

    this->setParent(nullptr);
    this->setWindowFlags(
        Qt::Widget| Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);

    auto RETAIN = [](QWidget *wgt) -> void {
        QSizePolicy sp = wgt->sizePolicy(); sp.setRetainSizeWhenHidden(true);
        wgt->setSizePolicy(sp); };
    RETAIN(ui->btExt); RETAIN(ui->lbTop); RETAIN(ui->btCls);

    //Подключение слотов.
    connect(&tmr, &QTimer::timeout, this, &WAttention::on_timer);

}// WAttention

// Вызывается при показе формы. ------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::showEvent(QShowEvent * /* evt */) {
    tmr.setInterval(500); tmr.start();

}// showEvent

// Вызывается при скрытии формы. -----------------------------------------------
//------------------------------------------------------------------------------
void WAttention::hideEvent(QHideEvent * /* evt */) {
    tmr.stop();
}// hideEvent

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WAttention::~WAttention() {
    delete ui;
}//~WAttention

// Отрисовка виджета. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::paintEvent(QPaintEvent * /* evt */) {

    QPainter painter(this);

    painter.setPen(QPen(Qt::red, 4, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::NoBrush));

/*
    int x = ui->lyMid->geometry().x(), y = ui->lyMid->geometry().y();
    int p = ui->frRgt->width(), s = ui->lyMid->spacing();
    int w = ui->lyMid->geometry().width(), h = ui->lyMid->geometry().height();
    painter.drawRect(x + p + s, y, w - (2*s + 2*p), h);
*/

//    (this->*draw_ext)(painter);

}// paintEvent

// Дорисовка, если курсор ВНЕ окна. --------------------------------------------
//------------------------------------------------------------------------------
void WAttention::draw_free(QPainter &painter) {
    DBG << "draw_free";

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::NoBrush));
    painter.drawEllipse(50, 50, 100, 100);
}// draw_free

// Дорисовка, если курсор НА окне. ---------------------------------------------
//------------------------------------------------------------------------------
void WAttention::draw_over(QPainter &painter) {
    DBG << "draw_over";

    painter.setPen(QPen(Qt::blue, 5, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::NoBrush));

    QRect rct = this->contentsRect();

    rct.setX(rct.x() + 4); rct.setY(rct.y() + 4);
    rct.setWidth(rct.width() - 4); rct.setHeight(rct.height() - 4);

    painter.drawRect(rct);
}// draw_over

// Изменение размера. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::resizeEvent(QResizeEvent * /* evt */) {

/*
    int x = this->width()/2, y = this->height()/2, r = 50;
    QPainter painter;
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter.drawEllipse(x-r, y-r, x+r, y+r);
*/

//    QRegion maskedRegion(x-r, y-r, x+r, y+r, QRegion::Ellipse);
//    this->setMask(maskedRegion);

}// resizeEvent



// Нажатие кнопки мыши на форме. -----------------------------------------------
//------------------------------------------------------------------------------
void WAttention::mousePressEvent(QMouseEvent *evt) {
    this->mpos = evt->pos();
    this->msze = this->size();
}// mousePressEvent

// Движение мыши на форме. -----------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::mouseMoveEvent(QMouseEvent *evt) {
//    DBG << "mouseMoveEvent" << QTime::currentTime();


    if(evt->buttons() && Qt::LeftButton) {
        int dx = evt->pos().x() - mpos.x(), dy = evt->pos().y() - mpos.y();

        DBG << "dx:" << dx << "dy:" << dy;

        if(ui->lbTop->underMouse()) {        /* POS */
            this->move(this->pos().x() + dx, this->pos().y() + dy);
        } else if(ui->frTop->underMouse()) { /* top */
            this->move(this->pos().x(), this->pos().y() + dy);
            this->resize(this->width(), this->height() - dy);
        } else if(ui->frRgt->underMouse()) { /* rgt */
            this->resize(this->msze.width() + dx, this->height());
        } else if(ui->frBtm->underMouse()) { /* btm */
            this->resize(this->width(), this->msze.height() + dy);
        } else if(ui->frLft->underMouse()) { /* lft */
            this->resize(this->width() - dx, this->height());
            this->move(this->pos().x() + dx, this->pos().y());
        } else if(ui->frBRS->underMouse()) { /* btm rgt */
            this->resize(this->msze.width() + dx, this->msze.height() + dy);
        } else if(ui->frBLS->underMouse()) { /* btm lft */
            this->resize(this->width() - dx, this->height() + dy);
            this->move(this->pos().x() + dx, this->pos().y() + dy);
        } else {                             /* DFT */
            this->move(this->pos().x() + dx, this->pos().y() + dy);
        }

    }// if(evt->buttons() && Qt::LeftButton)

}// mouseMoveEvent

// Курсор мыши попал на форму. -------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::enterEvent(QEvent * /* evt */) {
    DBG << "enterEvent" << QTime::currentTime();

/*
    const static QString VIS =
        "background-color: rgba(255,255,255,255);"
        "color: rgba(0,0,0,255);"
    ;

    ui->lbTop->setStyleSheet(VIS);
    ui->btCls->setStyleSheet(VIS);

    this->draw_ext = &WAttention::draw_over;
    this->repaint();
*/
}// enterEvent

// Курсор мыши покинул форму. --------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::leaveEvent(QEvent * /* evt */) {
    DBG << "leaveEvent" << QTime::currentTime();

/*
    const static QString TRP =
        "background-color: rgba(0,0,0,0);"
        "color: rgba(0,0,0,0);"
    ;

    ui->lbTop->setStyleSheet(TRP);
    ui->btCls->setStyleSheet(TRP);

    this->draw_ext = &WAttention::draw_free;
    this->repaint();
*/
}// leaveEvent


// Скрыть форму. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WAttention::on_btCls_clicked() { this->hide(); }


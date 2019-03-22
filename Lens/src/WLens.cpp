// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QColorDialog>
#include <QPainter>

#include "_srv/Srv/dbg.h"

#include "ui_WLens.h"
#include "WLens.h"
#include "FMain.h"
//#include "


// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern FMain *fmMain;

// Событие отрисовки. ----------------------------------------------------------
//------------------------------------------------------------------------------
/*
void WLens::paintEvent(QPaintEvent* evt) {
    QPainter painter(this);

    if(this->state == State::Pick) {
        int w = this->width(), h = this->height();
        int x = w/2, y = h/2;

        painter.drawLine(0, y, w, y);
        painter.drawLine(x, 0, x, h);
    }// if(this->state == State::Pick)

}// paintEvent
*/
// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLens::WLens(QWidget *parent) : QWidget(parent), ui(new Ui::WLens) {

    // Внешний вид.
    ui->setupUi(this);
    ui->wgTool->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->raise();

    ui->wgPlt->addPlate((new WClr(this, Qt::black))
        ->fix(true)->sel(true) );

    //Инициализация.
    this->w = this->width(); this->h = this->height();
    this->S = 13; // scale

}// WLens

// Выполняется при показе формы. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::showEvent(QShowEvent* /*evt*/) {
    this->raise();
}// showEvent

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WLens::~WLens() {
    delete ui;
}//~WLens

// Задать изображение. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::setImg(const QPoint &pos, const QImage &img) {
    int w = img.width()*S, h = img.height()*S;

    if(this->state == State::Pick) {
        int x = w/2 - 1, y = h/2 - 1;
        QImage big(img.scaled(w, h));
        QPainter painter(&big);

        painter.setPen(Qt::red);
        painter.drawLine(0, y, w, y);
        painter.drawLine(x, 0, x, h);

        ui->lbImg->setPixmap(QPixmap::fromImage(big));
    } else {
        ui->lbImg->setPixmap(QPixmap::fromImage(img.scaled(w, h)));
    }// if(this->state == State::Pick)

    this->setGeometry(pos.x() + shift, pos.y() + shift, w, h);
}// setImg

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::mousePressEvent(QMouseEvent *evt) {

    // Добавить цветовую плашку.
    if(ui->lbImg->geometry().contains(evt->pos())) {
        QPoint pos = ui->lbImg->mapFromParent(evt->pos());

        switch(evt->button()) {

         case Qt::LeftButton: {

            for(int x=(pos.x()/S)*S; x<(1 + pos.x()/S)*S; x++) {
                for(int y=(pos.y()/S)*S; y<(1 + pos.y()/S)*S; y++) {
                    this->img.setPixel(x, y, ui->wgPlt->color().rgb());
                }// y
            }// x

            ui->lbImg->setPixmap(QPixmap::fromImage(img));
         } break;

         case Qt::RightButton: {
            QColor clr = this->img.pixelColor(pos);
            if(ui->wgPlt->contain(clr)) { /*ui->wgPlt->select(clr);*/ }
            else { ui->wgPlt->addPlate((new WClr(this, clr))/*->sel(true)*/); }
         } break;

         default: {
         } break;

        }// switch(evt->button())

    }// if(ui->lbImg->geometry().contains(evt->pos()))

}// mousePressEvent

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::showTool(void) {
    this->img = ui->lbImg->pixmap()->toImage();
    ui->wgTool->show(); this->state = State::Edit;
}// showTool

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::hideTool(void) { ui->wgTool->hide(); this->state = State::Pick; }

// Нажатие кнопки: Пипетка. ----------------------------------------------------
//------------------------------------------------------------------------------
void WLens::on_btPipet_clicked() {
    FNC << "bgn";
}// on_btPipet_clicked

// Нажатие кнопки: Цвет. -------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::on_btColor_clicked() {
    QColor color = QColorDialog::getColor(this->color, this);

    if(color.isValid()) {
        this->color = color;
        ui->wgPlt->addPlate((new WClr(this, this->color))
            ->fix(true)->sel(true) );
    }// if(color.isValid())
}// on_btColor_clicked

//------------------------------------------------------------------------------


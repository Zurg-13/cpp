// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QColorDialog>

#include "_srv/Srv/dbg.h"

#include "ui_WLens.h"
#include "WLens.h"
#include "FMain.h"
//#include "


// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern FMain *fmMain;

// Удалить цветовую плашку. ----------------------------------------------------
//------------------------------------------------------------------------------
void WLens::remove_clr(WClr *clr) {
    QMutableListIterator<WClr*> it(plt);
    while(it.hasNext()) { if(it.next() == clr) { it.remove(); }}
    delete clr;
}// remove_clr

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLens::WLens(QWidget *parent) : QWidget(parent), ui(new Ui::WLens) {

    // Внешний вид.
    ui->setupUi(this);
    ui->wgTool->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->raise();

    //Инициализация.
    this->w = this->width(); this->h = this->height();
    this->scale = 10;

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
void WLens::setPic(const QPoint &pos, const QPixmap &pic) {
    ui->lbImg->setPixmap(pic);
    this->img = pic.toImage();
    this->setGeometry(pos.x() + shift, pos.y() + shift, w, h);
}// setPic

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::mousePressEvent(QMouseEvent *evt) {

    // Добавить цветовую плашку.
    if(ui->lbImg->geometry().contains(evt->pos())) {
        QPoint pos = ui->lbImg->mapFromParent(evt->pos());

        switch(evt->button()) {

         case Qt::LeftButton: {

            for(int x=(pos.x()/10)*10; x<(1 + pos.x()/10)*10; x++) {
                for(int y=(pos.y()/10)*10; y<(1 + pos.y()/10)*10; y++) {
                    this->img.setPixel(x, y, qRgb(0,0,0));
                }
            }// i

            ui->lbImg->setPixmap(QPixmap::fromImage(img));
         } break;

         case Qt::RightButton:
            ui->wgPlt->addPlate(new WClr(
                this, this->img.pixelColor(pos) ));
         break;

         default:
            break;

        }// switch(evt->button())

    }// if(ui->lbImg->geometry().contains(evt->pos()))

}// mousePressEvent

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::showTool(void) { ui->wgTool->show(); }

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::hideTool(void) { ui->wgTool->hide(); }

// Нажатие кнопки: Пипетка. ----------------------------------------------------
//------------------------------------------------------------------------------
void WLens::on_btPipet_clicked() {
    FNC << "bgn";
}// on_btPipet_clicked

// Нажатие кнопки: Цвет. -------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::on_btColor_clicked() {
    this->color = QColorDialog::getColor(this->color, this);
    ui->wgPlt->addPlate((new WClr(
        this, this->color))->fixed(true) );
}// on_btColor_clicked

//------------------------------------------------------------------------------


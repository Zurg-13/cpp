// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "_srv/Srv/dbg.h"

#include "ui_WLens.h"
#include "WLens.h"
#include "FMain.h"


// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern FMain *fmMain;

// Удалить цветовую плашку. ----------------------------------------------------
//------------------------------------------------------------------------------
void WLens::remove_clr(WClr *clr) {
    for(QList<WClr*>::iterator it = plt.begin(); it != plt.end(); it++)
        { if((*it) == clr) { plt.erase(it); }}
    delete clr;

    FNC << "size:" << plt.size();
}// remove_clr

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLens::WLens(QWidget *parent) : QWidget(parent), ui(new Ui::WLens) {

    // Внешний вид.
    ui->setupUi(this);
    ui->wgTool->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->wgTool->setAttribute(Qt::WA_TransparentForMouseEvents, true);

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
    this->setGeometry(pos.x() + shift, pos.y() + shift, w, h);
}// setPic

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::mousePressEvent(QMouseEvent *evt) {

    // Добавить цветовую плашку.
    if(ui->lbImg->geometry().contains(evt->pos())) {
        WClr *plate = new WClr(
            this, ui->lbImg->pixmap()->toImage().pixelColor(
                ui->lbImg->mapFromParent(evt->pos()) ));
        ui->lyPlt->insertWidget(0, plate);
        this->plt.append(plate);
        if(this->plt.size() > this->clr_plate_max)
            {  this->plt.first()->deleteLater(); this->plt.removeFirst(); }
        connect(plate, &WClr::remove, this, &WLens::remove_clr);
    }// if(ui->lbImg->geometry().contains(evt->pos()))

}// mousePressEvent

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::showTool(void) { ui->wgTool->show(); }

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::hideTool(void) {
    FNC << "bgn";
    ui->wgTool->hide();
}// hideTool

// Нажатие кнопки: Пипетка. ----------------------------------------------------
//------------------------------------------------------------------------------
void WLens::on_btPipet_clicked() {
    FNC << "bgn";
}

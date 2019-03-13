// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "_srv/Srv/dbg.h"

#include "ui_WLens.h"
#include "WLens.h"
#include "FMain.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern FMain *fmMain;


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLens::WLens(QWidget *parent) : QWidget(parent), ui(new Ui::WLens) {

    // Внешний вид.
    ui->setupUi(this);
    ui->frTool->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->frTool->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    //Инициализация.
    this->w = this->width(); this->h = this->height();
    this->scale = 10;

}// WLens

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
    this->raise();
}// setPic

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void WLens::mousePressEvent(QMouseEvent* /*evt*/) {
//    fmMain->setState(State::Show);
//    this->hideTool();
}// mousePressEvent

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::showTool(void) { ui->frTool->show(); }

// Показать панель инструментов. -----------------------------------------------
//------------------------------------------------------------------------------
void WLens::hideTool(void) {
    FNC << "bgn";
    ui->frTool->hide();
}// hideTool

//------------------------------------------------------------------------------


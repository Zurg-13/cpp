#include <QTimer>
#include <QPainter>

#include "_srv/Srv/dbg.h"

#include "ui_WClr.h"
#include "WClr.h"

#include "AdvanceEdit.h"

// Является отмеченным. --------------------------------------------------------
//------------------------------------------------------------------------------
bool WClr::isFixed(void) { return ui->cbFix->isChecked(); }

// Выставить цвет. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WClr::setColor(const QColor &clr) {
    static QString CLR("background-color:%1;");
    ui->lbColor->setStyleSheet(CLR.arg(clr.name()));
    ui->edColor->setText(clr.name());
}// setColor

// Зафиксировать цвет. ---------------------------------------------------------
//------------------------------------------------------------------------------
WClr* WClr::fix(bool fix) { ui->cbFix->setChecked(fix); return this; }

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void WClr::mousePressEvent(QMouseEvent* evt) {
    this->setSelected(
        ui->lbColor->geometry().contains(evt->pos()) );
}// mousePressEvent

// Событие отрисовки. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WClr::paintEvent(QPaintEvent */*evt*/) {

    if(this->fl_select) {
        QPen pen(Qt::red); pen.setWidth(10);
        QPainter painter(this); painter.setPen(pen);
        painter.drawRect(this->rect());
    }// if(this->selected)

}// paintEvent

// Установить признак выбора. --------------------------------------------------
//------------------------------------------------------------------------------
void WClr::setSelected(bool selected){
    if(this->fl_select != selected)
        { this->fl_select = selected; this->repaint(); emit select_chg(this); }
}// setSelected

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WClr::WClr(QWidget *parent, QColor color)
    : QWidget(parent), ui(new Ui::WClr), clr(color)
{
    // Внешний вид.
    ui->setupUi(this);
    ui->edColor->clear();
    this->setColor(this->clr);

    // Инициализация.
    connect(ui->edColor, &AdvanceEdit::focused, this, &WClr::on_edColor_focused);

}// WClr

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WClr::~WClr() { delete ui; }

// Удалить плашку. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WClr::on_btClose_clicked() { emit(remove(this)); }

// Выбрать окно кода цвета. ----------------------------------------------------
//------------------------------------------------------------------------------
void WClr::on_edColor_focused()
    { QTimer::singleShot(0, ui->edColor, &AdvanceEdit::selectAll); }

// Изменение состояния фиксации. -----------------------------------------------
//------------------------------------------------------------------------------
void WClr::on_cbFix_stateChanged(int /*arg*/) {
    emit change_fix(this);
}// on_cbFix_stateChanged

//------------------------------------------------------------------------------


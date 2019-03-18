#include <QTimer>

#include "ui_WClr.h"
#include "WClr.h"

#include "AdvanceEdit.h"


// Выставить цвет. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WClr::setColor(const QColor &clr) {
    static QString CLR("background-color:%1;");
    ui->lbColor->setStyleSheet(CLR.arg(clr.name()));
    ui->edColor->setText(clr.name());
}// setColor

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WClr::WClr(QWidget *parent, QColor color)
    : QWidget(parent), ui(new Ui::WClr), color(color)
{
    // Внешний вид.
    ui->setupUi(this);
    ui->edColor->clear();
    this->setColor(this->color);

    // Инициализация.
    connect(ui->edColor, &AdvanceEdit::focused, this, &WClr::on_edColor_focused);

}// WClr

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WClr::~WClr() {
    delete ui;
}//~WClr

// Удалить плашку. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WClr::on_btClose_clicked() { emit(remove(this)); }

#include <QDebug>

// Выбрать окно кода цвета. ----------------------------------------------------
//------------------------------------------------------------------------------
void WClr::on_edColor_focused() {
    QTimer::singleShot(0, ui->edColor, &AdvanceEdit::selectAll);

//    ui->edColor->selectAll();
}

//------------------------------------------------------------------------------


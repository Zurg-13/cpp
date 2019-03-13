#include "ui_WAnimation.h"
#include "WAnimation.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WAnimation::WAnimation(QWidget *parent):QWidget(parent), ui(new Ui::WAnimation){
    ui->setupUi(this);

    btnSize = 50;
    scale = 100; xShift = 100; yShift = 100;

}// WAnimation

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WAnimation::~WAnimation() {
    delete ui;
}// ~WAnimation

// Добавить. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void WAnimation::on_btAdd_clicked() {
    QPushButton *button = new QPushButton(this);

    button->setText(QString::number(btn.size()));
    button->setGeometry(ui->btAdd->geometry());
    button->show();
    btn.append(button);

    reArrangeButtons();
}// on_btAdd_clicked

// Удалить. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void WAnimation::on_btSub_clicked() {
    if(btn.isEmpty()) { return; }

    QPushButton *button = btn.last();

    btn.removeLast();
    delete button;

    reArrangeButtons();
}// on_btSub_clicked

// Перераспределить кнопки по форме. -------------------------------------------
//------------------------------------------------------------------------------
void WAnimation::reArrangeButtons() {
    int btnCnt = btn.size();
    float angleUnit = 6.28 / btnCnt;

    for(int i=0; i < btnCnt; i++) {
        QPushButton *button = btn.at(i);
        QPropertyAnimation *animation =
            new QPropertyAnimation(button, "geometry");

        animation->setDuration(1000);

//        animation->setEasingCurve(QEasingCurve::Linear);
//        animation->setEasingCurve(QEasingCurve::OutElastic);
        animation->setEasingCurve(QEasingCurve::OutCubic);

        animation->setEndValue(QRectF(
            cos(angleUnit * i) * scale + xShift
          , sin(angleUnit * i) * scale + yShift
          , btnSize, btnSize));

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }// i

}// on_btSub_clicked

// Удалить произвольный элемент из списка кнопок. ------------------------------
//------------------------------------------------------------------------------
void WAnimation::on_btDmg_clicked() {
    if(btn.isEmpty()) { return; }

    int num = qrand() % btn.size();
    QPushButton *button = btn[num];

    btn.removeAt(num);
    delete button;

    reArrangeButtons();
}// on_btDmg_clicked

//------------------------------------------------------------------------------


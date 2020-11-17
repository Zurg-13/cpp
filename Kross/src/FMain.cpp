#include <functional>

#include <QMessageBox>
#include <QDateTime>
#include <QRandomGenerator>

#include <qmath.h>

#include "std.h"
#include "tme.h"

#include "ui_FMain.h"
#include "FMain.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern QString BCG;
extern QString CLR;
extern QString RED;
extern QString GRN;
extern QString BLU;

extern int TARGET;

/* Главная форма. *************************************************************/
/******************************************************************************/

// Расчётная функция. ----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::calc(QPushButton *btn, QList<Section*> &sec) {
    int cur = INT(btn->text());
    int cnt = INT(ui->lbCnt->text()), stp = INT(ui->lbStp->text()) + 1;

    ui->lbStp->setText(STR(stp));

    for(Section *section: sec)
        { cur += section->grab(); }

    btn->setStyleSheet(BCG
      + (TARGET == cur ? BLU
        :TARGET <  cur ? GRN : RED) + ";" );

    if(TARGET == cur) {
        cnt++;
        ui->lbCnt->setText(STR(cnt)); btn->setText("+");
    } else {
        btn->setText(STR(cur));
    }// else

    ui->lbReit->setText(QString::number((double)cnt/(double)stp, 'g', 2));
    ui->wgChainBtm->add(cur);
}// calc

// Сброс. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::reset(void) {
    auto LBL = [](QList<Section*> &sec) -> void
        { for(Section *section : sec) { section->reset(); }};

    auto BTN = [](QPushButton* btn) -> void
        { btn->setText("+"); btn->setStyleSheet(BCG + BLU + ";"); };

    LBL(lft_btm); LBL(rgt_top); LBL(center); LBL(lft_btm); LBL(rgt_btm);
    BTN(ui->bt22); BTN(ui->bt24); BTN(ui->bt33); BTN(ui->bt42); BTN(ui->bt44);
    ui->lbStp->setText("Шаги"); ui->lbCnt->setText("Счёт");
    ui->lbReit->setText("Рейт");

    ui->wgChainTop->reset();
    ui->wgChainBtm->clear();
}// reset

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {
    ui->setupUi(this);
    ui->lbReit->setWordWrap(true);



//    QRandomGenerator::global()->seed(SYSDATE.toMSecsSinceEpoch());
    qsrand(SYSDATE.toMSecsSinceEpoch());

    lft.init(ui->wgChainTop, ui->wgLft, {0}, 2, WLine::Direct::LFT_TO_RGT);
    rgt.init(ui->wgChainTop, ui->wgRgt, {0}, 2, WLine::Direct::RGT_TO_LFT);
    top.init(ui->wgChainTop, ui->wgTop, {0}, 2, WLine::Direct::TOP_TO_BTM);
    btm.init(ui->wgChainTop, ui->wgBtm, {0}, 2, WLine::Direct::BTM_TO_TOP);

    lft_top = {&lft, &top}; rgt_top = {&rgt, &top};
    center = {&lft, &rgt, &top, &btm};
    lft_btm = {&lft, &btm}; rgt_btm = {&rgt, &btm};

    ui->wgChainTop->init(100, WChain::Direct::TOP_TO_BTM);
    ui->wgChainBtm->init(100, WChain::Direct::TOP_TO_BTM);
}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}// ~FMain

// Выход. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Нажатие кнопок. -------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_bt24_clicked() { calc((QPushButton*)sender(), lft_top); }
void FMain::on_bt44_clicked() { calc((QPushButton*)sender(), rgt_top); }
void FMain::on_bt33_clicked() { calc((QPushButton*)sender(), center); }
void FMain::on_bt22_clicked() { calc((QPushButton*)sender(), lft_btm); }
void FMain::on_bt42_clicked() { calc((QPushButton*)sender(), rgt_btm); }

// Сброс. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btReset_clicked() { reset(); }

// Сложность. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aEasy_triggered()
    { MAX_NUM = 2; ui->lbReit->setStyleSheet(CLR + "orange" + ";"); reset(); }
void FMain::on_aStandart_triggered()
    { MAX_NUM = 4; ui->lbReit->setStyleSheet(CLR + "green" + ";"); reset(); }
void FMain::on_aHard_triggered()
    { MAX_NUM = 8; ui->lbReit->setStyleSheet(CLR + "red" + ";"); reset(); }

// Проба. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    if(chain == nullptr)
        { chain = new WChain(); chain->init(90, WChain::Direct::BTM_TO_TOP); }

    chain->show();
}// on_aTest_triggered

//------------------------------------------------------------------------------


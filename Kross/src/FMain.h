#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

#include "_srv/std.h"

#include "WLine.h"
#include "WChain.h"

namespace Ui { class FMain; }

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern int MAX_NUM;

/* Секция. ********************************************************************/
/******************************************************************************/
class Section {

public:
    Section(void) {}
    int  grab() { return line->grab(idx); }
    void init(
        WChain *chain, WLine *line, QList<int> idx, int num, WLine::Direct dir)
    {
        this->line = line; this->idx = idx;
        this->line->init(chain, num, dir);
    }// init

    void reset() { this->line->reset(); }

private:
    WLine *line;
    QList<int> idx;

};// Section

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = 0);
            ~FMain();

private slots:
    void on_aExit_triggered();

    void on_bt24_clicked();
    void on_bt44_clicked();
    void on_bt22_clicked();
    void on_bt42_clicked();
    void on_bt33_clicked();

    void on_btReset_clicked();
    void on_aEasy_triggered();
    void on_aStandart_triggered();
    void on_aHard_triggered();

    void on_aTest_triggered();

private:
    Ui::FMain *ui;
    Section top, lft, rgt, btm;
    QList<Section*> lft_top, rgt_top, center, lft_btm, rgt_btm;

    WChain *chain = nullptr;

    void reset(void);
    void calc(QPushButton *btn, QList<Section*> &sec);

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

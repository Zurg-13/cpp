#ifndef WCHAIN_H
#define WCHAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
//#include <QLabel>
#include <QResizeEvent>

#include "Plate.h"

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class WChain; }

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern int MAX_NUM;

/* Цепочка элементов. *********************************************************/
/******************************************************************************/
class WChain : public QWidget {
    Q_OBJECT

public:
    enum class Direct { TOP_TO_BTM, BTM_TO_TOP, NONE };

    explicit WChain(QWidget *parent = 0);
            ~WChain();

    void add(int val);
    void add(void) { add(rnd()); }
    int  grab(void);
    void init(int num, Direct direct);
    void reset(void);
    void clear(void) {  del(); plt.clear(); recalc(); redraw(); }

private:
    Ui::WChain *ui;
    QList<Plate*> plt;

    Direct direct;
    float xBgn, yBgn, xEnd, yEnd, wSze, hSze;
    float wdh, hgt;
    int   num, off; // отступ.
    int   col = 20, row = 5;

    void resizeEvent(QResizeEvent *evt);
    void recalc(void);
    void redraw(void);

    void del(void) { for(QLabel *label: plt) { delete label; }}
    int  rnd(void) { return MAX_NUM - qrand() % (MAX_NUM*2 + 1); }

    int (WChain:: *calc_x)(int cnt);
    int (WChain:: *calc_y)(int cnt);

    int calc_x_top_btm(int cnt)
        { return off + (cnt % col) * wSze; }
    int calc_y_top_btm(int cnt)
        { return (hgt - off) - (hSze + (cnt / col)*hSze); }
    int calc_y_btm_top(int cnt)
        { return off + (cnt / col) * hSze; }

};// WChain

//------------------------------------------------------------------------------
#endif // WCHAIN_H

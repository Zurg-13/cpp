#ifndef WLINE_H
#define WLINE_H

#include <QResizeEvent>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QList>

#include "WChain.h"

namespace Ui { class WLine; }

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern int MAX_NUM;


/* Линия кнопок. **************************************************************/
/******************************************************************************/
class WLine : public QWidget {
    Q_OBJECT

public:
    enum class Direct { LFT_TO_RGT, RGT_TO_LFT, TOP_TO_BTM, BTM_TO_TOP, NONE };

    explicit WLine(QWidget *parent = 0);
            ~WLine();

    int  grab(QList<int> idx);
    void init(WChain *chain, int num, Direct direct);
    void reset(void);

public slots:
    void onTimer(void);

private slots:
    void finished(void);

private:
    Ui::WLine *ui;
    QList<QLabel*> lbl;
    WChain *chain;
    Direct direct;

    int num;
    float xBgn, yBgn, xEnd, yEnd, wSze, hSze;
    float wdh, hgt;
    int  off; // отступ.

    void resizeEvent(QResizeEvent *evt);
    void recalc(void);
    void redraw(void);

    void set(Direct direct);
    void add();
    int  rnd(void) { return MAX_NUM - qrand() % (MAX_NUM*2 + 1); }
    void del(void) { for(QLabel *label: lbl) { delete label; }}

    int (WLine:: *calc_x)(int cnt);
    int (WLine:: *calc_y)(int cnt);
    int (WLine:: *calc_w)(void);
    int (WLine:: *calc_h)(void);

    // Горизонтально.
    int calc_w_hor(void) { return (wdh - off*2) / num; }
    int calc_h_hor(void) { return hgt; }

    int calc_x_rgt_lft(int cnt) { return (cnt * wSze) + off; }
    int calc_y_rgt_lft(int cnt) { return off; }

    int calc_x_lft_rgt(int cnt) { return (wdh - off) - (wSze + cnt*wSze); }
    int calc_y_lft_rgt(int cnt) { return off; }

    // Вертикально.
    int calc_w_vrt(void) { return wdh; }
    int calc_h_vrt(void) { return (hgt - off*2) / num; }

    int calc_x_top_btm(int cnt) { return off; }
    int calc_y_top_btm(int cnt) { return (hgt - off) - (hSze + cnt*hSze); }

    int calc_x_btm_top(int cnt) { return off; }
    int calc_y_btm_top(int cnt) { return (cnt * hSze) + off; }

};// WLine

//------------------------------------------------------------------------------
#endif // WLINE_H

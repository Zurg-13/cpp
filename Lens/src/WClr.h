#ifndef WCLR_H
#define WCLR_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QPaintEvent>

namespace Ui { class WClr; }

/* Класс плашки цвета. ********************************************************/
/******************************************************************************/
class WClr : public QWidget {
    Q_OBJECT

 public:
            WClr(QWidget *parent, QColor color);
           ~WClr();
    WClr*   fix(bool fix);
    WClr*   sel(bool sel) { this->fl_select = sel; return this; }

    QColor  color(void) { return clr; }
    void    setColor(const QColor &clr);
    bool    isFixed(void);
    bool    isSelect(void) { return fl_select; }
    void    setSelected(bool selected);

    void    mousePressEvent(QMouseEvent *evt);
    void    paintEvent(QPaintEvent *evt);

 signals:
    void    remove(WClr*);      // Попытка удаления.
    void    change_fix(WClr*);  // Изменение фиксации.
    void    select_chg(WClr*);  // Изменение статуса выбора.

private slots:
    void on_btClose_clicked();
    void on_edColor_focused();
    void on_cbFix_stateChanged(int);

private:
    Ui::WClr *ui;
    QColor clr;
    bool fl_select = false;

};// WClr

//------------------------------------------------------------------------------
#endif // WCLR_H

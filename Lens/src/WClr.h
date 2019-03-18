#ifndef WCLR_H
#define WCLR_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QColor>

namespace Ui { class WClr; }

/* Класс плашки цвета. ********************************************************/
/******************************************************************************/
class WClr : public QWidget {
    Q_OBJECT

 public:
            WClr(QWidget *parent, QColor color);
           ~WClr();

    void    setColor(const QColor &clr);
    QColor  getColor(void) { return color; }

 signals:
    void    remove(WClr *);    // Попытка удаления.

private slots:
    void on_btClose_clicked();
    void on_edColor_focused();

private:
    Ui::WClr *ui;
    QColor color;

};// WClr

//------------------------------------------------------------------------------
#endif // WCLR_H

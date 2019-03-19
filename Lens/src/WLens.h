#ifndef WLENS_H
#define WLENS_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QMouseEvent>
#include <QImage>

#include "WClr.h"

namespace Ui { class WLens; }

/* Класс отображения увеличенного изображения. ********************************/
/******************************************************************************/
class WLens : public QWidget{
    Q_OBJECT

 public:
    explicit WLens(QWidget *parent = nullptr);
            ~WLens();

    void setPic(const QPoint &pos, const QPixmap &pic);
    void showTool(void);
    void hideTool(void);

 protected:
    void showEvent(QShowEvent *evt);
    void mousePressEvent(QMouseEvent* evt);

 private slots:
    void remove_clr(WClr *clr);
    void on_btPipet_clicked();
    void on_btColor_clicked();

private:
    Ui::WLens *ui;
    int w, h, scale, shift = 10;
    int clr_plate_max = 3;
    QImage img;
    QColor color;

    QList<WClr*> plt;


};// WLens

//------------------------------------------------------------------------------
#endif // WLENS_H

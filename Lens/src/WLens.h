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
class WLens : public QWidget {
    Q_OBJECT

    enum class State { Pick, Edit };

 public:
    explicit WLens(QWidget *parent = nullptr);
            ~WLens();

    void setImg(const QPoint &pos, const QImage &img);

    void showTool(void);
    void hideTool(void);

    int  scale(void) { return S; }

//    void paintEvent(QPaintEvent*);

 protected:
    void showEvent(QShowEvent *evt);
    void mousePressEvent(QMouseEvent* evt);

 private slots:
    void on_btPipet_clicked();
    void on_btColor_clicked();
    void on_btReset_clicked();

private:
    Ui::WLens *ui;
    State state = State::Pick;
    int w, h, S, shift = 10;
    int clr_plate_max = 3;
    QImage img, orig_img;
    QPoint pos;
    QColor color;

};// WLens

//------------------------------------------------------------------------------
#endif // WLENS_H

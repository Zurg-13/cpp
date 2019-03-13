#ifndef WLENS_H
#define WLENS_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

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
    void mousePressEvent(QMouseEvent* evt);

 private:
    Ui::WLens *ui;
    int w, h, scale, shift = 10;

};// WLens

//------------------------------------------------------------------------------
#endif // WLENS_H

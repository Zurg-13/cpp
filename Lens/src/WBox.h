#ifndef WBOX_H
#define WBOX_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include "WClr.h"

//------------------------------------------------------------------------------
namespace Ui { class WBox; }

/* Контейнер для плашек. ******************************************************/
/******************************************************************************/
class WBox : public QWidget {
    Q_OBJECT

 public:
    explicit WBox(QWidget *parent = nullptr);
            ~WBox();

    void     addPlate(WClr *plate);
    QColor   color() { return (plate_sel ? plate_sel->color() : Qt::black); }

 private slots:
   void remove_clr(WClr *clr);
   void chahge_fix(WClr *clr);
   void select_clr(WClr *clr);

 private:
    Ui::WBox *ui;
    int size = 4;

    WClr *plate_sel = nullptr;

};// WBox

//------------------------------------------------------------------------------
#endif // WBOX_H

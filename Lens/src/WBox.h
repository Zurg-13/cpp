#ifndef WBOX_H
#define WBOX_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

#include "WClr.h"

namespace Ui { class WBox; }



/* Контейнер для плашек. ******************************************************/
/******************************************************************************/
class WBox : public QWidget {
    Q_OBJECT

 public:
    explicit WBox(QWidget *parent = nullptr);
            ~WBox();

    void     addPlate(WClr *plate);

 private slots:
   void remove_clr(WClr *clr);
   void chahge_fix(WClr *clr);

 private:
    Ui::WBox *ui;
    int size = 4;

};// WBox

//------------------------------------------------------------------------------
#endif // WBOX_H

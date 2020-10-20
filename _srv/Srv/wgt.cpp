// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QtWidgets/QLayoutItem>
#include <QtWidgets/QLayout>

#include "wgt.h"

// Убрать все элементы из layout. ----------------------------------------------
//------------------------------------------------------------------------------
void clearLayout(QLayout *ly) {
    while(QLayoutItem* itm = ly->layout()->takeAt(0)) {
        if(QWidget *wgt = itm->widget()) { wgt->setParent(nullptr); }
        if(QLayout *chd = itm->layout()) { clearLayout(chd); }
        delete itm;
    }// while(QLayoutItem* itm = ly->layout()->takeAt(0))
}// clearLayout

//------------------------------------------------------------------------------


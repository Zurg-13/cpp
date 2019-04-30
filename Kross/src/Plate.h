#ifndef PLATE_H
#define PLATE_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QLabel>

#include "_srv/std.h"

/* Плашка. ********************************************************************/
/******************************************************************************/
class Plate : public QLabel {
    Q_OBJECT

public:
    Plate(
        int val
      , QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags() );

    int  getVal(void) { return val; }
    void setVal(int val);

private:
    int val;

};// Plate

//------------------------------------------------------------------------------
#endif // PLATE_H

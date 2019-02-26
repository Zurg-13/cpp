#ifndef FASTEN_H
#define FASTEN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QFile>


/* Крепление дополнительной секции данных к файлу. ****************************/
/******************************************************************************/
class Fasten {

public:
    static QByteArray read(QFile &fle);             // Считать.
    static bool write(QFile &fle, QByteArray &arr); // Пристегнуть.

private:
    Fasten(void){}

};

//------------------------------------------------------------------------------
#endif // FASTEN_H
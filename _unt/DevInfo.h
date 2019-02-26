#ifndef DEVINFO_H
#define DEVINFO_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>

/* Считывание серийного номера USB-накопителя. ********************************/
/******************************************************************************/
class DevInfo {

public:
    QString ven, prd, rev, snm;

    static DevInfo inquire(int vol);// Опросить устройство.
    QString toString(void);         // Приведение к строке.
    QString format(QString fmt);    // Форматированный вывод.

private:
    DevInfo(){}
    DevInfo(QString ven, QString prd, QString rev, QString snm)
        : ven(ven), prd(prd), rev(rev), snm(snm) {}
};

//------------------------------------------------------------------------------
#endif // DEVINFO_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "tme.h"


// Приведение миллисекунд. -----------------------------------------------------
//------------------------------------------------------------------------------
unsigned int SS(unsigned int mills) { return mills / SEC; }
unsigned int MI(unsigned int mills) { return mills / MIN; }
unsigned int HH(unsigned int mills) { return mills / HRS; }
unsigned int DD(unsigned int mills) { return mills / DAY; }

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString STR(QDateTime val) { return val.toString(Qt::ISODate); }

//------------------------------------------------------------------------------

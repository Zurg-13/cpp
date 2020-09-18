#ifndef ENV_H
#define ENV_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMimeDatabase>

#include "WWs.h"
#include "DSel.h"
#include "FMain.h"
#include "DHeader.h"

/* Глобальная среда пиложения. ************************************************/
/******************************************************************************/
class E {
 public:
    const static unsigned int PORT = 314;

    static unsigned int port;
    static QMimeDatabase *mime;

    static FMain *Main;
    static DHeader *Header;
    static WWs *Ws;
    static DSel *Sel;

};// E

//------------------------------------------------------------------------------
#endif // ENV_H

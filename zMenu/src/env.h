#ifndef ENV_H
#define ENV_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMimeDatabase>

#include "WBrd.h"
#include "FMain.h"

/* Глобальная среда пиложения. ************************************************/
/******************************************************************************/
class E {
 public:
    const static unsigned int PORT = 314;

    static unsigned int port;
    static QMimeDatabase *mime;

    static WBrd *Log;
    static FMain *Main;
};// E

//------------------------------------------------------------------------------
#endif // ENV_H

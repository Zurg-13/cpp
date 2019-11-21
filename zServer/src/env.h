#ifndef ENV_H
#define ENV_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMimeDatabase>

#include "FMain.h"
#include "DHeader.h"
#include "WLogBoard.h"

/* Глобальная среда пиложения. ************************************************/
/******************************************************************************/
class E {
 public:
    const static unsigned int PORT = 314;

    static unsigned int port;
    static QMimeDatabase *mime;

    static FMain *Main;
    static DHeader *Header;
    static WLogBoard *Log;

};// E


//------------------------------------------------------------------------------
#endif // ENV_H

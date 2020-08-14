#ifndef ENV_H
#define ENV_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMimeDatabase>

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

};// E


//------------------------------------------------------------------------------
#endif // ENV_H

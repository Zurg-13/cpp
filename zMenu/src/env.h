#ifndef ENV_H
#define ENV_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QtSql>
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
    static QSqlDatabase *sldb;

    static WBrd *Log;
    static FMain *Main;

    static void err(const QString &msg)
        { Log->post(msg)->clr(Qt::red); }
    static void imp(const QString &msg)
        { Log->post(msg); }
    static void oth(const QString &msg)
        { Log->post(msg)->clr(Qt::lightGray); }

};// E

//------------------------------------------------------------------------------
#endif // ENV_H

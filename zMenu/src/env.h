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

    // Логирование.
    static void err(const QString &msg)
        { Log->post(msg)->clr(Qt::red); }
    static void err(const QString &msg, const QString &adv)
        { Log->post(msg, adv)->clr(Qt::red); }
    static void imp(const QString &msg)
        { Log->post(msg)->clr(Qt::yellow); }
    static void imp(const QString &msg, const QString &adv)
        { Log->post(msg, adv)->clr(Qt::yellow); }
    static void oth(const QString &msg)
        { Log->post(msg); }
    static void oth(const QString &msg, const QString &adv)
        { Log->post(msg, adv); }

};// E

//------------------------------------------------------------------------------
#endif // ENV_H

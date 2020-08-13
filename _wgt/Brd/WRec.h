#ifndef WREC_H
#define WREC_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include "std.h"

namespace Ui { class WRec; }

/* Запись. ********************************************************************/
/******************************************************************************/
class WRec : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool is_grab READ isGrab)

 public:
    WRec(const QString &msg, QWidget *prn = nullptr)
        : WRec(msg, EMPTY_STR, prn) {}
    WRec(const QString &msg, const QString &adv, QWidget *prn = nullptr);
   ~WRec();

    void tme(const QDateTime &tme);
    void msg(const QString &msg);
    void adv(const QString &adv);

    void clr(const QColor &clr);
    void hgl(const QColor &clr);

    bool  isGrab(void) { return this->is_grab; }
    WRec* grab() { this->is_grab = true; return this;}
    WRec* free() { this->is_grab = false; return this; }

private slots:
    void on_btAdv_clicked();

private:
    Ui::WRec *ui;
    bool is_grab = false;

};// WRec

//------------------------------------------------------------------------------
#endif // WREC_H

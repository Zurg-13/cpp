#ifndef WBRD_H
#define WBRD_H

#include <QWidget>

#include "WRec.h"

namespace Ui { class WBrd; }

/* Панель размещения записей. *************************************************/
/******************************************************************************/
class WBrd : public QWidget {
    Q_OBJECT

 public:
    enum class Place { top, btm, lft, rgt, ovr };

    explicit WBrd(QWidget *parent = nullptr);
            ~WBrd();
    void  clear(void);
    void  place(const QWidget *wgt, Place plc = Place::btm);

    WRec* post(const QString &msg, const QString &adv = EMPTY_STR);
    WRec* post(WRec *rec);
    void  rift(void);

    WRec* grab(void);
    WRec* free(WRec *rec);

 private slots:
    void ctx_menu(const QPoint &pos);
    void on_aClear_triggered();
    void on_aTest_triggered();

    void on_aOver_triggered();

private:
    Ui::WBrd *ui;
    QWidget *box = nullptr;

    int insert_pos = 0;

};// WBrd

//------------------------------------------------------------------------------
#endif // WBRD_H

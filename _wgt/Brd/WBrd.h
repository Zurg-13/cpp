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
    explicit WBrd(QWidget *parent = nullptr);
            ~WBrd();

    WRec* post(const QString &msg, const QColor &clr = QColor());
    WRec* post(const QString &msg, const QString &adv, const QColor &clr = QColor());
    WRec* post(WRec *rec);
    void  rift(void);

    WRec* grab(void);
    WRec* free(WRec *rec);
    void  clear(void);

private:
    Ui::WBrd *ui;
    QWidget *box = nullptr;

    int insert_pos = 0;

};// WBrd

//------------------------------------------------------------------------------
#endif // WBRD_H

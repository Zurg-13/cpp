#ifndef WLOGBOARD_H
#define WLOGBOARD_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

#include "WLogEntry.h"

namespace Ui { class WLogBoard; }

/* Отображение записей лога. **************************************************/
/******************************************************************************/
class WLogBoard : public QWidget {
    Q_OBJECT

 public:
    explicit WLogBoard(QWidget *parent = nullptr);
            ~WLogBoard();

    void        post(const QString &inp, const QString &out);
    void        post(WLogEntry *entry);
    WLogEntry*  grab(void);
    void        free(WLogEntry *entry);

 private:
    Ui::WLogBoard *ui;
    QWidget *box = nullptr;

    int inset_pos = 0;

};// WLogBoard


//------------------------------------------------------------------------------
#endif // WLOGBOARD_H

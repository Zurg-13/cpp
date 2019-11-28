#ifndef WLOGBOARD_H
#define WLOGBOARD_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QColor>

#include "WLogEntry.h"

namespace Ui { class WLogBoard; }

/* Отображение записей лога. **************************************************/
/******************************************************************************/
class WLogBoard : public QWidget {
    Q_OBJECT

 public:
    explicit WLogBoard(QWidget *parent = nullptr);
            ~WLogBoard();

    void       post(const QString &msg, const QColor &clr = QColor());
    void       post(const QString &inp, const QString &out, const QColor &clr);
    void       post(const QString &inp, const QString &out);
    WLogEntry* post(WLogEntry *entry);
    WLogEntry* grab(void);
    WLogEntry* free(WLogEntry *entry);
    void       rift(void);

    void       clear(void);

 private:
    Ui::WLogBoard *ui;
    QWidget *box = nullptr;

    int inset_pos = 0;

};// WLogBoard


//------------------------------------------------------------------------------
#endif // WLOGBOARD_H

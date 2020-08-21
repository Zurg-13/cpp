#ifndef WHDLBOARD_H
#define WHDLBOARD_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QSplitter>

#include "WHdlEntry.h"

namespace Ui { class WHdlBoard; }

/* Отображение обработчиков. **************************************************/
/******************************************************************************/
class WHdlBoard : public QWidget {
    Q_OBJECT

 public:
    explicit WHdlBoard(QWidget *parent = nullptr);
            ~WHdlBoard();


    WHdlEntry* post(WHdlEntry * entry);
    void clear(void);

 private:
    Ui::WHdlBoard *ui;
    QSplitter *box = nullptr;

};// WHdlBoard

//------------------------------------------------------------------------------
#endif // WHDLBOARD_H

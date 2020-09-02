#ifndef WHDLBOARD_H
#define WHDLBOARD_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QSplitter>
#include <QScrollArea>

#include "dbg.h"
#include "std.h"

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

    QScrollArea* scrl(void);
    QSplitter* splt(void) { return this->box; }

 private:
    Ui::WHdlBoard *ui;
    QSplitter *box = nullptr;

};// WHdlBoard

/* Заполнитель. ***************************************************************/
/******************************************************************************/
#include <QResizeEvent>
#include <QLabel>
#include <QMargins>
#include <QScrollBar>
class Spacer : public QLabel {
    Q_OBJECT

 public:
            ~Spacer() {};
    explicit Spacer(WHdlBoard *brd, QWidget *prnt = nullptr)
           : QLabel(prnt), brd(brd)
    {
        this->setMinimumHeight(0); this->setMaximumHeight(100);
        this->setSizePolicy(
            QSizePolicy::Minimum, QSizePolicy::Minimum);
        this->setStyleSheet("border: 3px solid red;");

        connect(
            brd->splt(), &QSplitter::splitterMoved
          , this, &Spacer::spl_on_move );

    }// Spacer

 private slots:
    void spl_on_move(int pos, int idx) {
        QSplitter *splt = brd->splt();
        QList<int> sze = splt->sizes();
        QList<int>::iterator it = sze.begin(), end = sze.end();

        int sum = (this->minimumHeight() + this->maximumHeight()) / 2;
        while(it != end) {
            int val = *it; it++;
            if(it != end) { sum += (val + splt->handleWidth()); }
        }

        splt->setMinimumHeight(sum);
        splt->setMaximumHeight(sum);


        FNC << "pos:" << pos << "idx:" << idx;
    }// spl_on_move

 private:
    WHdlBoard *brd;

};// Spacer

//------------------------------------------------------------------------------
#endif // WHDLBOARD_H

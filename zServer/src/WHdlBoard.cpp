// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QScrollArea>
#include <QVBoxLayout>

#include "WHdlBoard.h"
#include "ui_WHdlBoard.h"


/* WHdlBoard. *****************************************************************/
/******************************************************************************/

// Вернуть зону скроллинга. ----------------------------------------------------
//------------------------------------------------------------------------------
QScrollArea* WHdlBoard::scrl(void) { return ui->sa; }

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QLabel>
WHdlBoard::WHdlBoard(QWidget *parent) : QWidget(parent), ui(new Ui::WHdlBoard) {

    // Внешний вид.
    ui->setupUi(this);

    // Скроллинг.
    ui->sa->adjustSize();

    this->box = new QSplitter(this);
        this->box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        this->box->setOrientation(Qt::Vertical);
        this->box->setChildrenCollapsible(false);
        this->box->setHandleWidth(5);
        this->box->setOpaqueResize(true);
//        this->box->addWidget(spr);
//        this->box->setStretchFactor(0, 1);


    Spacer *spr = new Spacer(this);

        this->box->addWidget(spr);
        this->box->setStretchFactor(0, 1);

/*
    QWidget *wgt = new QWidget;
    QVBoxLayout *lyt = new QVBoxLayout(wgt);
        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->setSpacing(2);
        lyt->addWidget(this->box);
*/

    ui->sa->setWidget(this->box);
}// WHdlBoard

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WHdlBoard::~WHdlBoard() {
    delete ui; FNC << "end";
}//~WHdlBoard

// Добавить запись. ------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QTableWidgetItem>
#include <QListWidget>
#include <QScrollArea>
WHdlEntry* WHdlBoard::post(WHdlEntry *entry) {
    entry->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->box->insertWidget(0, entry); return entry;

}// post

// Очистить. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void  WHdlBoard::clear(void)
    { while(QWidget *wgt = this->box->widget(0)) { delete wgt; } }

//------------------------------------------------------------------------------


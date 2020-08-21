// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------

#include <QScrollArea>
#include <QVBoxLayout>

#include "dbg.h"

#include "WHdlBoard.h"
#include "ui_WHdlBoard.h"


/* WHdlBoard. *****************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QLabel>
WHdlBoard::WHdlBoard(QWidget *parent) : QWidget(parent), ui(new Ui::WHdlBoard) {

    // Внешний вид.
    ui->setupUi(this);

    // Скроллинг.
    QWidget *wgt = new QLabel();
        wgt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
//        wgt->setMaximumHeight(5);
        wgt->setStyleSheet("border: 4px solid red;");

    this->box = new QSplitter(this);
        this->box->setOrientation(Qt::Vertical);
//        this->box->setStretchFactor();
        this->box->setChildrenCollapsible(false);
        this->box->setHandleWidth(3);
        this->box->setOpaqueResize(true);
//        this->box->addWidget(wgt);

    QVBoxLayout *ly = new QVBoxLayout(this->box);
        ly->setContentsMargins(0, 0, 0, 0);
        ly->setSpacing(2);
    ui->sa->setWidget(this->box);

}// WHdlBoard

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WHdlBoard::~WHdlBoard() {
    delete ui; FNC << "end";
}//~WHdlBoard

// Добавить запись. ------------------------------------------------------------
//------------------------------------------------------------------------------
WHdlEntry* WHdlBoard::post(WHdlEntry *entry)
    { this->box->insertWidget(0, entry); return entry; }
//    { this->box->insertWidget(this->box->count()-1, entry); return entry; }
//    { this->ui->listWidget->addItem(QListWidgetItem()) }

// Очистить. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void  WHdlBoard::clear(void)
    { while(QWidget *wgt = this->box->widget(0)) { delete wgt; } }

//------------------------------------------------------------------------------


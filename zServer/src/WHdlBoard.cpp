// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------

#include <QScrollArea>
#include <QVBoxLayout>

#include "WHdlBoard.h"
#include "ui_WHdlBoard.h"


/* WHdlBoard. *****************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WHdlBoard::WHdlBoard(QWidget *parent) : QWidget(parent), ui(new Ui::WHdlBoard) {

    // Внешний вид.
    ui->setupUi(this);

    // Скроллинг.
    this->box = new QWidget(this);
        this->box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    QVBoxLayout *ly = new QVBoxLayout(this->box);
        ly->setContentsMargins(0, 0, 0, 0);
        ly->setSpacing(2);
    ui->sa->setWidget(this->box);

}// WHdlBoard

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WHdlBoard::~WHdlBoard() {
    delete ui;
}//~WHdlBoard

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void  WHdlBoard::clear(void) {
    int pos = 0;
    while(QLayoutItem* itm = this->box->layout()->itemAt(pos)) {
        if(QWidget *wgt = itm->widget())
            { delete wgt; delete this->box->layout()->takeAt(pos); }
    }// while(QLayoutItem* itm = this->box->layout()->itemAt(pos))
}// clear

//------------------------------------------------------------------------------


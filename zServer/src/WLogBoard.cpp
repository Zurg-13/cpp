// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QScrollArea>

#include "dbg.h"
#include "std.h"

#include "WLogBoard.h"
#include "ui_WLogBoard.h"


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLogBoard::WLogBoard(QWidget *parent) : QWidget(parent), ui(new Ui::WLogBoard) {

    // Внешний вмид.
    ui->setupUi(this);

    // Скроллинг.
    this->box = new QWidget(this);
        this->box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    QVBoxLayout *ly = new QVBoxLayout(this->box);
        ly->setContentsMargins(0, 0, 0, 0);
        ly->setSpacing(2);
    ui->sa->setWidget(this->box);

}// WLogBoard

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WLogBoard::~WLogBoard() {
    delete ui;
}//~WLogBoard

// Добавить запись. ------------------------------------------------------------
//------------------------------------------------------------------------------
void WLogBoard::post(const QString &inp, const QString &out)
    { post(new WLogEntry(inp, out)); }

void WLogBoard::post(WLogEntry *entry) {
    static_cast<QBoxLayout*>(this->box->layout())
        ->insertWidget(this->inset_pos, entry);
}// post

// Захватить виджет. -----------------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry* WLogBoard::grab(void) {
    WLogEntry *entry = new WLogEntry(EMPTY_STR, EMPTY_STR);
        entry->highlight(Qt::red);
    post(entry); this->inset_pos++;
    return entry;
}// grab

// Освободить виджет. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WLogBoard::free(WLogEntry *entry) {
    this->inset_pos--;
    entry->highlight(Qt::green);
}// free

//------------------------------------------------------------------------------


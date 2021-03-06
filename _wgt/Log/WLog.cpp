// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include "WLog.h"
#include "ui_WLog.h"


/* Дополнительные функции. ****************************************************/
/******************************************************************************/


/* WLog. **********************************************************************/
/******************************************************************************/

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void  WLog::clr(void) { ui->pt->clear(); }

// Запись об ошибке (ВСЕГДА). --------------------------------------------------
//------------------------------------------------------------------------------
void WLog::err(QString msg) { add(msg, Qt::red); }

// Оповещение (только в активный). ---------------------------------------------
//------------------------------------------------------------------------------
void WLog::inf(QString msg) { if(this->isVisible()) add(msg); }

// Добавить запись в лог. ------------------------------------------------------
//------------------------------------------------------------------------------
#include "dbg.h"
void WLog::add(QString msg, QColor clr) {
    static const QString BR = "<br>";
    auto MSG = [&]() -> QString {
        return "<h style='color:" + clr.name() + ";'>" + msg + "</h>";
    };// MSG

/*
    auto RMV = [&](const QTextBlock &block) -> void {

        if(ui->pt->blockCount() > this->size) {

            FNC << "cnt:" << ui->pt->blockCount();

            QTextCursor cursor(block);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
        }// while(ui->pt->blockCount() >= this->size)
    };// RMV
*/

/*
    if(normal) {
        ui->pt->appendHtml(MSG());
        RMV(ui->pt->document()->firstBlock());
    } else {
        ui->pt->cursorForPosition(QPoint(0, 0)).insertHtml(MSG()+BR);
        RMV(ui->pt->document()->lastBlock());
    }// else // if(normal)
*/

    ui->pt->appendHtml(MSG());
}// add

// Добавить как текст. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WLog::addPlainText(QString msg) {
    static const QString NS = "\n";
    auto MSG = [&]() -> QString { return msg; };

/*
    if(normal) { ui->pt->appendPlainText(MSG());
    } else     { ui->pt->cursorForPosition(QPoint(0, 0)).insertText(MSG()+NS); }
*/

    ui->pt->appendPlainText(MSG());
}// addPlainText

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WLog::WLog(QWidget *parent) : QWidget(parent), ui(new Ui::WLog) {

    // Внешний вид.
    ui->setupUi(this);

    // Контекстное меню.
    ui->pt->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->pt, &QPlainTextEdit::customContextMenuRequested
          , this, &WLog::ctx_menu );
}// WLog

WLog::WLog(int size, QWidget *parent) : WLog(parent)
    { ui->pt->setMaximumBlockCount(size); }

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WLog::~WLog() { delete ui; }

// Показать с привязкой. -------------------------------------------------------
//------------------------------------------------------------------------------
void WLog::place(const QWidget *wgt, Plc plc) {
    QRect loc = this->frameGeometry();
    QRect ext = wgt->frameGeometry();

    switch(plc) {
     case Plc::top:
        this->move(ext.x(), ext.y()-loc.height());
        this->resize(wgt->width(), this->height()); break;

     case Plc::btm:
        this->move(ext.x(), ext.y()+ext.height());
        this->resize(wgt->width(), this->height()); break;

     case Plc::lft:
        this->move(ext.x()-loc.width(), ext.y());
        this->resize(this->width(), wgt->height()); break;

     case Plc::rgt:
        this->move(ext.x()+ext.width(), ext.y());
        this->resize(this->width(), wgt->height()); break;

     case Plc::ovr:
        this->move(ext.topLeft());
        this->resize(wgt->size()); break;

    }// switch(pos)

    this->show();
}// show

// Перехват вызова контекстного меню. ------------------------------------------
//------------------------------------------------------------------------------
void WLog::ctx_menu(const QPoint &pos) {
    QMenu* menu = ui->pt->createStandardContextMenu();

    menu->addSeparator();
    menu->addAction(ui->aClear);
    menu->exec(ui->pt->mapToGlobal(pos));

    delete menu;
}// on_context_menu

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WLog::on_aClear_triggered() { this->clr(); }

//------------------------------------------------------------------------------

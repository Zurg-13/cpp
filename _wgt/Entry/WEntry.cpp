// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>

#include "ui_WEntry.h"
#include "WEntry.h"


// Выполняется при старте. -----------------------------------------------------
//------------------------------------------------------------------------------
WEntry::WEntry(QWidget *parent) : QWidget(parent), ui(new Ui::WEntry) {
    ui->setupUi(this);


    static int cnt = 0; entry_id = cnt++;
}// WEntry

// Выполняется по завершении. --------------------------------------------------
//------------------------------------------------------------------------------
WEntry::~WEntry() { delete ui; }

// Удалить. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void WEntry::on_btDelete_clicked() {
    if(entry_state) return;
    if(confirm_delete) {
        if(QMessageBox::Yes == QMessageBox::question(
            this, "Подтверждение"
          , "Действительно удалить запись."
          , QMessageBox::Yes, QMessageBox::No) )
        { emit(remove(this)); }
    } else {
        { emit(remove(this)); }
    }// else // if(confirm_delete)
}// on_btDelete_clicked

// Вернуть текст. --------------------------------------------------------------
//------------------------------------------------------------------------------
QString WEntry::text(void) { return ui->edVal->text(); }

// Установить фокус на запись. -------------------------------------------------
//------------------------------------------------------------------------------
void WEntry::setFocus() { ui->edVal->setFocus(); }

// Возможность редактирования. -------------------------------------------------
//------------------------------------------------------------------------------
WEntry* WEntry::setReadOnly(bool val) {
    ui->edVal->setReadOnly(val); return this;
}// setReadOnly

// Выставить текст. ------------------------------------------------------------
//------------------------------------------------------------------------------
WEntry* WEntry::setText(QString txt) { ui->edVal->setText(txt); return this; }

// Выставить плейcхолдер. ------------------------------------------------------
//------------------------------------------------------------------------------
WEntry* WEntry::setPlaceholder(QString phld) {
    ui->edVal->setPlaceholderText(phld); return this;
}// setPlaceholder

// Выставить сигнатуру. --------------------------------------------------------
//------------------------------------------------------------------------------
WEntry* WEntry::setSign(QString sgn){ this->entry_sign = sgn; return this; }

// Выставить статичное состояние. ----------------------------------------------
//------------------------------------------------------------------------------
WEntry* WEntry::setState(bool state) {

    if(state) {
        ui->btDelete->setText("!!!");
        ui->btDelete->setStyleSheet("");
    } else {
        ui->btDelete->setText("!!!");
        ui->btDelete->setStyleSheet(
            "border-width 2;border-style:solid;border-color:black;color:red;" );
    }// else // if(state)

    return this;
}// setState

// Выставить подтверждение на удаление. ----------------------------------------
//------------------------------------------------------------------------------
WEntry* WEntry::setConfirmDelete(bool confirm) {
    confirm_delete = confirm; return this;
}// setConfirmDelete

//------------------------------------------------------------------------------

#include "ButtonStopper.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
ButtonStopper::ButtonStopper(QAbstractButton *btn,  const QString &msg) {
    this->btn = btn; this->text = btn->text();
    this->btn->setText(msg.isEmpty() ? QString("Выполняется") : msg);
    this->btn->setEnabled(false);
}// ButtoStopper

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
ButtonStopper::~ButtonStopper() { btn->setText(text); btn->setEnabled(true); }

//------------------------------------------------------------------------------


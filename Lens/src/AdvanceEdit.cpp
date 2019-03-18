// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "AdvanceEdit.h"


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
AdvanceEdit::AdvanceEdit(QWidget *parent) : QLineEdit(parent) {}

// Перехват события фокусировки на контроле. -----------------------------------
//------------------------------------------------------------------------------
void AdvanceEdit::focusInEvent(QFocusEvent *evt) {
    QLineEdit::focusInEvent(evt);
    emit focused();
}// focusInEvent

//------------------------------------------------------------------------------


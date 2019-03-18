#ifndef ADVANCEEDIT_H
#define ADVANCEEDIT_H

#include <QLineEdit>

/* Расширинный класс редактирования строки. ***********************************/
/******************************************************************************/
class AdvanceEdit : public QLineEdit {
    Q_OBJECT

 public:
    explicit AdvanceEdit(QWidget *parent = nullptr);
    virtual void focusInEvent(QFocusEvent *event) override;

 signals:
    void focused(void);

};// AdvanceEdit

//------------------------------------------------------------------------------
#endif // ADVANCEEDIT_H

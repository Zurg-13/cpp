#ifndef WACTIVE_H
#define WACTIVE_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class WActive; }

/* Работа с ActiveX. **********************************************************/
/******************************************************************************/
class WActive : public QWidget {
    Q_OBJECT

 public:
    explicit WActive(QWidget *parent = nullptr);
    ~WActive();

 private:
    Ui::WActive *ui;

};// WActive

//------------------------------------------------------------------------------
#endif // WACTIVE_H

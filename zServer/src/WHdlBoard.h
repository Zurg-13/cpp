#ifndef WHDLBOARD_H
#define WHDLBOARD_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class WHdlBoard; }

/* Отображение обработчиков. **************************************************/
/******************************************************************************/
class WHdlBoard : public QWidget {
    Q_OBJECT

 public:
    explicit WHdlBoard(QWidget *parent = nullptr);
            ~WHdlBoard();


    void       clear(void);

 private:
    Ui::WHdlBoard *ui;
    QWidget *box = nullptr;

};// QWidget

//------------------------------------------------------------------------------
#endif // WHDLBOARD_H

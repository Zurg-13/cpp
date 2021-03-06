#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QPainter>

#include "Part.h"

namespace Ui { class FMain; }

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

 public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();
 protected:
   virtual void paintEvent(QPaintEvent *evt) override;


 private slots:
    void on_aExit_triggered();
    void on_aLog_triggered();
    void on_btGen_clicked();

    void on_btReset_clicked();

    void on_btClr_clicked();

private:
    Ui::FMain *ui;
    Part *tail;

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

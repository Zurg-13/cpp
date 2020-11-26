#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QPainter>

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

 private:
    Ui::FMain *ui;

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

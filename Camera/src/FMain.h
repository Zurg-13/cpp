#ifndef FMAIN_H
#define FMAIN_H

#include <QMainWindow>

#include "Cam.h"

namespace Ui { class FMain; }

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

private slots:
    void on_aExit_triggered();
    void on_aTest_triggered();

    void on_btCpt_clicked();

private:
    Ui::FMain *ui;
    Cam *camera = nullptr;

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QFile>

#include "wgt/WEdit.h"

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class FMain; }

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = 0);
            ~FMain();

private slots:
    void on_aExit_triggered();
    void on_aTest_triggered();
    void on_aLog_triggered();
    void on_aConn_triggered();

private:
    Ui::FMain *ui;
    WEdit *wgEdit = nullptr;

    void save(QString fnme);
    void load(QString fnme);

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

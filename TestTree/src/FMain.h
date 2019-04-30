#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStandardItemModel>
#include <QMainWindow>

#include "unt/DomTreeModel.h"


// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class FMain; }

// Главная форма. --------------------------------------------------------------
//------------------------------------------------------------------------------
class FMain : public QMainWindow {
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = 0);
            ~FMain();

private slots:
    void on_aExit_triggered();
    void on_aTest_triggered();
    void on_aOpenXML_triggered();

private:
    Ui::FMain *ui;
    QStandardItemModel *mdl;
    DomTreeModel *dom_mdl;



};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

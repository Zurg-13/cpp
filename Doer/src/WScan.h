#ifndef WSCAN_H
#define WSCAN_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include "mdl/MScaner.h"

namespace Ui { class WScan; }

/* Настройка прослшивания сканера. ********************************************/
/******************************************************************************/
class WScan : public QWidget {
    Q_OBJECT
    Q_CLASSINFO("notify", "Сканер QR-кодов")

 public:
    MScaner *scan = nullptr;

    explicit WScan(QWidget *parent = nullptr);
            ~WScan();

    void loadConf(const QString& fnme);

private slots:
    void on_btSetConf_clicked();
    void on_btLoadConf_clicked();
    void on_btOpen_clicked();
    void on_btClose_clicked();
    void on_btListDev_clicked();

private:
    Ui::WScan *ui;

};// WScan


//------------------------------------------------------------------------------
#endif // WSCAN_H

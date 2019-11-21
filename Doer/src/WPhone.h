#ifndef WPHONE_H
#define WPHONE_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include "unt/MPhone.h"

namespace Ui { class WPhone; }

/* Настройка телефонии. *******************************************************/
/******************************************************************************/
class WPhone : public QWidget {
    Q_OBJECT
    Q_CLASSINFO("notify", "Телефония")

 public:
    MPhone *phone = nullptr;

    explicit WPhone(QWidget *parent = nullptr);
            ~WPhone();

    void loadConf(const QString& fnme);

 private slots:
    void on_btDevOpen_clicked();
    void on_btDevClose_clicked();
    void on_btConfSet_clicked();
    void on_btConfLoad_clicked();

    void on_btInfo_clicked();
    void on_btStop_clicked();
    void on_btCall_clicked();

private:
    Ui::WPhone *ui;

};// WPhone

//------------------------------------------------------------------------------
#endif // WPHONE_H

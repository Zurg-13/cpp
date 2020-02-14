#ifndef WPINPAD_H
#define WPINPAD_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include "mdl/MPinpad.h"

namespace Ui { class WPinpad; }

/* Настройка пинпада. *********************************************************/
/******************************************************************************/
class WPinpad : public QWidget {
    Q_OBJECT
    Q_CLASSINFO("notify", "Пинпад")

 public:
    MPinpad *pinpad = nullptr;

    explicit WPinpad(QWidget *parent = nullptr);
            ~WPinpad();

    void loadConf(const QString& fnme);

 private slots:
    void on_btCfgLoad_clicked();
    void on_btDevOpen_clicked();
    void on_btDevClose_clicked();
    void on_btCfgSet_clicked();
    void on_btZReport_clicked();

private:
    Ui::WPinpad *ui;

};// WPinpad


//------------------------------------------------------------------------------
#endif // WPINPAD_H

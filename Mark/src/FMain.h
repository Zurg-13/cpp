#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QAbstractNativeEventFilter>

#include "_lib/QGlobalShortcut/qglobalshortcut.h"

namespace Ui { class FMain; }

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

  public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

  protected:
/*
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
*/

  private slots:
    void global_hk(void);

    void on_aExit_triggered();
    void on_aTest_triggered();
    void on_btAdd_clicked();

  private:
    Ui::FMain *ui;
    QGlobalShortcut *hkAct;

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

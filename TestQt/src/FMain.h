#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class FMain; }

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT
    QString test_var_val = QString(3, '#');

  public:
    const QString &test_var = test_var_val;

    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

  private slots:
    void on_aTest_triggered();
    void on_btAnimation_clicked();
    void on_aExit_triggered();
    void on_btSound_clicked();
    void on_btVirtualD_clicked();

    void on_btProc_clicked();
    void on_btTB_clicked();

    void on_btLogger_clicked();
    void on_pushButton_clicked();

    void on_btCast_clicked();
    void on_btActiveX_clicked();

    void on_bt_clicked();

    void on_btListClear_clicked();
    void on_btSharedPtr_clicked();
    void on_btQtConcurrent_clicked();
    void on_btWLog_clicked();
    void on_btLogAdv_clicked();
    void on_btBall_clicked();

    void on_btWS_clicked();

private:
    Ui::FMain *ui;
};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

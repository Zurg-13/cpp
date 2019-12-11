#ifndef WLOG_H
#define WLOG_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QMenu>

//------------------------------------------------------------------------------
namespace Ui { class WLog; }

/* Логирование. ***************************************************************/
/******************************************************************************/
class WLog : public QWidget {
    Q_OBJECT
    const int SIZE_DFT = 1000;

 public:
    enum class Plc { top, btm, lft, rgt, ovr };

    explicit WLog(QWidget *parent = nullptr);
             WLog(int size, QWidget *parent = nullptr);
            ~WLog();

    void addPlainText(QString msg);
    void add(QString msg, QColor clr = Qt::black);
    void err(QString msg);
    void inf(QString msg);
    void spc(void) { add("<br>"); }
    void clr(void);

/*
    void setUpToDown(void) { normal = true; }
    void setDownToUp(void) { normal = false; }
*/

    void place(const QWidget *wgt, Plc loc = Plc::btm);

 private slots:
    void ctx_menu(const QPoint &pos);
    void on_aClear_triggered();

 private:
    Ui::WLog *ui;
//    bool normal = false;
    int size = SIZE_DFT;

};// WLog

//------------------------------------------------------------------------------
#endif // WLOG_H

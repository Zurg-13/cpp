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

 public:
    enum class Plc { top, btm, lft, rgt, ovr };

    explicit WLog(QWidget *parent = nullptr);
            ~WLog();

    void addPlainText(QString msg);
    void add(QString msg, QColor clr = Qt::black);
    void err(QString msg);
    void inf(QString msg);
    void spc(void) { add("<br>"); }
    void clr(void);

    void setUpToDown(void) { normal = true; }
    void setDownToUp(void) { normal = false; }

    void place(const QWidget *wgt, Plc loc = Plc::btm);

 private slots:
    void on_context_menu(const QPoint &pos);

    void on_aClear_triggered();

 private:
    Ui::WLog *ui;
    QMenu *extMenu;
    bool normal = false;

};// WLog

//------------------------------------------------------------------------------
#endif // WLOG_H

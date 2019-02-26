#ifndef WLOG_H
#define WLOG_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class WLog; }

// Логирование. ----------------------------------------------------------------
//------------------------------------------------------------------------------
class WLog : public QWidget {
    Q_OBJECT

public:
    explicit WLog(QWidget *parent = 0);
            ~WLog();

    void addPlainText(QString msg);
    void add(QString msg, QColor clr = Qt::black);
    void err(QString msg);
    void inf(QString msg);
    void spc(void) { add("<br>"); }
    void clr(void);

    void setUpToDown(void) { normal = true; }
    void setDownToUp(void) { normal = false; }

private:
    Ui::WLog *ui;
    bool normal = false;

};

//------------------------------------------------------------------------------
#endif // WLOG_H

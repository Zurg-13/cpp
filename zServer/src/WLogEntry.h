#ifndef WLOGENTRY_H
#define WLOGENTRY_H


#include <QWidget>

namespace Ui { class WLogEntry; }

/* Запись в логе. *************************************************************/
/******************************************************************************/
class WLogEntry : public QWidget {
    Q_OBJECT

 public:
    WLogEntry(const QString &inp, const QString &out, QWidget *prn = nullptr);
   ~WLogEntry();

    void inp(const QString &msg);
    void out(const QString &msg);

    void highlight(const QColor &clr);


 private:
    Ui::WLogEntry *ui;

};// WLogEntry

//------------------------------------------------------------------------------
#endif // WLOGENTRY_H

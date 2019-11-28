#ifndef WLOGENTRY_H
#define WLOGENTRY_H


#include <QWidget>

namespace Ui { class WLogEntry; }

/* Запись в логе. *************************************************************/
/******************************************************************************/
class WLogEntry : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool is_grab READ isGrab)

 public:
    WLogEntry(const QString &inp, const QString &out, QWidget *prn = nullptr);
   ~WLogEntry();

    void inp(const QString &msg);
    void out(const QString &msg);

    void clr(const QColor &clr);
    void hgl(const QColor &clr);

    bool isGrab(void) { return this->is_grab; }
    WLogEntry* grab() { this->is_grab = true; return this;}
    WLogEntry* free() { this->is_grab = false; return this; }

 private:
    Ui::WLogEntry *ui;
    bool is_grab = false;

};// WLogEntry

//------------------------------------------------------------------------------
#endif // WLOGENTRY_H

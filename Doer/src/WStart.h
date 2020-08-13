#ifndef WSTART_H
#define WSTART_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class WStart; }


/* Запуск и слежение за утилитами. ********************************************/
/******************************************************************************/
class WStart : public QWidget {
    Q_OBJECT
    Q_CLASSINFO("notify", "Служба запуска утилит")

 public:
    explicit WStart(QWidget *parent = nullptr);
            ~WStart();

    void setConf(const QString& conf);

 private:
    Ui::WStart *ui;

};// WStart


//------------------------------------------------------------------------------
#endif // WSTART_H

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

    void loadConf(const QString& fnme);

 private:
    Ui::WStart *ui;

};// WStart


//------------------------------------------------------------------------------
#endif // WSTART_H

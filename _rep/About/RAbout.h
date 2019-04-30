#ifndef RABOUT_H
#define RABOUT_H

//INCLUDE.-------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <QDialog>
#include <QDateTime>

//NAMESPACE.-----------------------------------------------------------------
//---------------------------------------------------------------------------
namespace Ui { class RAbout; }

/* Форма отчёта о текущем приложении. *****************************************/
/******************************************************************************/
class RAbout : public QDialog {
    Q_OBJECT

public:
    explicit RAbout(QWidget *parent = nullptr);
             RAbout(QString styleSheet, QWidget *parent = nullptr);
            ~RAbout();

    int      view(QString styleSheet);
    int      view(void);
    void     setStyle(QString styleSheet){ style = styleSheet; }

protected:
    void showEvent(QShowEvent*);

private:
    void startInit(void);    //Начальная инициализация.

    QString style;
    QDateTime start;

    Ui::RAbout *ui;
};// RAbout

//--------------------------------------------------------------------------
#endif // RABOUT_H

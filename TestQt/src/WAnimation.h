#ifndef WANIMATION_H
#define WANIMATION_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <math.h>

#include <QWidget>
#include <QList>
#include <QPropertyAnimation>
#include <QPushButton>

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class WAnimation; }

// Главная форма. --------------------------------------------------------------
//------------------------------------------------------------------------------
class WAnimation : public QWidget {
    Q_OBJECT

public:
    explicit WAnimation(QWidget *parent = 0);
            ~WAnimation();

private slots:
    void on_btAdd_clicked();
    void on_btSub_clicked();

    void on_btDmg_clicked();

private:
    Ui::WAnimation *ui;
    int xShift, yShift, scale, btnSize;
    QList<QPushButton*> btn;

    void reArrangeButtons();


};

//------------------------------------------------------------------------------
#endif // WANIMATION_H

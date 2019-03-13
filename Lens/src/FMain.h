#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QTime>
#include <QShortcut>

namespace Ui { class FMain; }
enum class State { Show, Edit };

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

 public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

    void setState(State state) { this->state = state; }

 protected:
    void showEvent(QShowEvent *evt);
    void closeEvent(QCloseEvent *evt);
    bool eventFilter(QObject *obj, QEvent *evt);
    void mousePressEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);

 private slots:
    void on_key_ESC(void);

    void on_aExit_triggered();
    void on_aTest_triggered();

private:
    Ui::FMain *ui;
    QShortcut *keyESC;

    QTime mouse_press_bgn;
    Qt::MouseButton mouse_press_btn;

    QPoint pos_bgn;
    QRect geom_std;

    State state;
    QPixmap pic, *old_pic = nullptr;
    int scale;

    void on_mouse_click(QMouseEvent *evt);


};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

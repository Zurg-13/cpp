#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QTime>
#include <QShortcut>
#include <QRubberBand>

namespace Ui { class FMain; }
enum class State { Pick, Edit };

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

 public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

    void setState(State state) { this->state = state; }
    void insertImg(const QPoint &pos,  const QImage& img);

 protected:
    void showEvent(QShowEvent *evt);
    void closeEvent(QCloseEvent *evt);

    bool eventFilter(QObject *obj, QEvent *evt);
    void mousePressEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);

    void enterEvent(QEvent *evt);
    void leaveEvent(QEvent *evt);

 private slots:
    void on_key_ESC(void);
    void on_upd_sze(QRect geom);

    void on_aExit_triggered();
    void on_aTest_triggered();

    void on_btSend_clicked();

    void on_aFileDest_triggered();

    void on_aYandexDest_triggered();

    void on_aGoogleDest_triggered();

private:
    Ui::FMain *ui;

    QRubberBand *rubb_band;
    QTime mouse_press_bgn;
    Qt::MouseButton mouse_press_btn;

    QPoint pos_bgn;

    QRect geom_old;
    QPixmap pixmap_old;

    State state = State::Edit;
    int scale = 13;

    void on_mouse_click(QMouseEvent *evt);
    void stdShow(void);
    void sendImg(const QPoint &pos);


};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

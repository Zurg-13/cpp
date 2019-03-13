#ifndef WATTENTION_H
#define WATTENTION_H

#include <QWidget>
#include <QPainter>
#include <QTimer>


namespace Ui { class WAttention; }

/* Выделитьель области. *******************************************************/
/******************************************************************************/
class WAttention : public QWidget {
    Q_OBJECT

public:
    explicit WAttention(QWidget *parent = nullptr);
            ~WAttention();

protected:
    void paintEvent(QPaintEvent *evt);
    void resizeEvent(QResizeEvent *evt);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent * event);

    void enterEvent(QEvent *evt);
    void leaveEvent(QEvent *evt);

    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);

private slots:
    void on_btCls_clicked();
    void on_timer(void);

private:
    Ui::WAttention *ui;
    QPoint mpos;
    QSize msze;
    QTimer tmr;

    void (WAttention::*draw_ext)(QPainter&) = nullptr;
    void draw_free(QPainter &painter);
    void draw_over(QPainter &painter);

};// WAttention

//------------------------------------------------------------------------------
#endif // WATTENTION_H

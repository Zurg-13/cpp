#ifndef WBALL_H
#define WBALL_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QMouseEvent>

namespace Ui { class WBall; }

/* Кружок. ********************************************************************/
/******************************************************************************/
class Ball {

 public:
    qreal x, y, dx, dy, r;

    Ball(qreal x, qreal y, qreal dx, qreal dy, qreal r)
        : x(x), y(y), dx(dx), dy(dy), r(r) {}

    void move(int w, int h) {
        x += dx; if(x > w) { x = 0; } else if ( x < 0) { x = w; }
        y += dy; if(y > h) { y = 0; } else if ( y < 0) { y = h; }
    }// calc

    void draw(QPainter &p) {
        p.setPen(Qt::black);
        p.drawEllipse(x-r, y-r, r*2, r*2);
    }

    void drawCollision(QPainter &p, Ball &ball) {
        qreal phi;

        p.setPen(Qt::green);
        phi = qAtan2(dy, dx);
        p.drawLine(
            this->x              , this->y
          , this->x + r*qCos(phi), this->y + r*qSin(phi));


        if(dist(ball) <= this->r + ball.r) {
            qreal x1 = this->x, y1 = this->y;
            qreal x2 = ball.x, y2 = ball.y;

            p.setPen(Qt::red);
            phi = this->phi(ball);
            p.drawLine(
                this->x               , this->y
              , this->x + 20*qCos(phi), this->y + 20*qSin(phi));

            p.setPen(Qt::blue);
            phi = qAtan((y1-y2)/(x1-x2));
            p.drawLine(
                this->x               , this->y
              , this->x + 20*qCos(phi), this->y + 20*qSin(phi));

        }// if(dist(ball) < this->r + ball.r)

    }// drawCollision

    void calc(Ball &ball) {
        if(dist(ball) < this->r + ball.r) {
            qreal pt;
            qreal p = this->phi(ball), a1 = this->agl(), a2 = ball.agl();
            qreal v1 = this->vel(), v2 = ball.vel();
            qreal m1 = this->mas(), m2 = ball.mas();

            pt =(v1*qCos(a1-p)*(m1-m2) + 2*m2*v2*cos(a2-p))/(m1+m2);
            dx = pt*qCos(p) + v1*qSin(a1-p)*qCos(p+M_PI_2);
            dy = pt*qSin(p) + v1*qSin(a1-p)*qSin(p+M_PI_2);

            pt = (v2*qCos(a2-p)*(m2-m1) + 2*m1*v1*cos(a1-p))/(m2+m1);
            ball.dx = pt*qCos(p) + v2*qSin(a2-p)*qCos(p+M_PI_2);
            ball.dy = pt*qSin(p) + v2*qSin(a2-p)*qSin(p+M_PI_2);
        }// if(dist(ball) <= this->r + ball.r)
    }// kick

    qreal dist(Ball &ball) {
        qreal dx = this->x - ball.x, dy = this->y - ball.y;
        qreal X = dx*dx, Y = dy*dy;
        qreal S = qSqrt(X + Y);
        return S;
    }

    qreal phi(Ball &ball) {
        qreal x1 = this->x, y1 = this->y;
        qreal x2 = ball.x, y2 = ball.y;
        return qAtan((y2-y1)/(x2-x1)) + M_PI_2;
    }

//    qreal agl(void) { return qAtan(dx/dy); }
    qreal agl(void) { return qAtan2(dy, dx); }

    qreal vel(void) { return qSqrt(dx*dx + dy*dy); }
    qreal mas(void) { return r*r; }

};// Ball


class Anomaly;
class Distortion;

/* Позиция. *******************************************************************/
/******************************************************************************/
struct Pos { qreal x, y; };

/* Элемент пачки. *************************************************************/
/******************************************************************************/
class Perturbation {

 public:
    qreal x, y, dx, dy, r, speed;
    QString name;
    QColor color = Qt::black;

    Distortion *parent;
    Perturbation *next = nullptr, *prev = nullptr;

    Perturbation(qreal x, qreal y, qreal dx, qreal dy, qreal r)
    :   x(x), y(y), dx(dx), dy(dy), r(r) {}

    Perturbation* setPrev(Perturbation *prev){ this->prev = prev; return this; }

    bool isMove(void) { return speed; }
    void draw(QPainter &p);
    bool isInside(qreal x, qreal y) { return isInside(x, y, this->r); }
    bool isInside(qreal x, qreal y, qreal r)
        { return (x-this->x)*(x-this->x) + (y-this->y)*(y-this->y) < r*r; }

    void push(void) {
        const double SPEED = 1.0;
        Perturbation *dest;

        if(prev) { if( next) { return; } else { dest = prev; }}
        else     { if(!next) { return; } else { dest = next; }}

        dx = SPEED*sign(dest->x - this->x);
        dy = SPEED*sign(dest->y - this->y);
    }// push
    void push(Perturbation *some) { speed = some->speed; }
    void stop() { speed = 0; }

    void jamp(Perturbation &pbtn) { jamp(pbtn.x, pbtn.y); }
    void jamp(qreal x, qreal y)   { this->x = x, this->y = y; }

    void calc(void) { x += dx; y += dy; }

    qreal dist(Perturbation *pbtn) {
        return (qSqrt(qPow(this->x - pbtn->x, 2)
                   +  qPow(this->y - pbtn->y, 2) ));
    }// dist

    qreal sign(qreal val)
        { if(val > 0) return  1.0; if(val < 0) return -1.0; return 0.0; }

 private:

};// Perturbation

// Траектория движения. *******************************************************/
/******************************************************************************/
class Trajectory {
 public:
    virtual void calc(Perturbation *ptbt) = 0;
    virtual void post(Perturbation *ptbt, const Perturbation *stab) = 0;

}; // Trajectory

/* Прямая. ********************************************************************/
/******************************************************************************/
class Linear : Trajectory {
    qreal dx, dy;

 public:
    Linear(qreal dx, qreal dy): dx(dx), dy(dy) {}
    virtual void calc(Perturbation *pbtn)
        { pbtn->x += dx*pbtn->speed; pbtn->y += dy*pbtn->speed; }
/*
    virtual void post(Perturbation *ptbt, const Perturbation *stab) {

    }// post
*/

};// Linear

/* Пачка. *********************************************************************/
/******************************************************************************/
class Distortion {

 public:
    Anomaly *parent;
    QString name;
    QList<Perturbation*> ptb;

    Distortion(void) {};
   ~Distortion(void) { for(Perturbation *pbtn: ptb) { delete pbtn; }};

    void add(Perturbation *elem) {
        elem->parent = this;
        elem->name = QString::number(count++);
        elem->next = ptb.isEmpty() ? nullptr : ptb.last()->setPrev(elem);
        ptb.append(elem);
    }// add

    Perturbation* get(qreal x, qreal y) {
        for(Perturbation *prtb: this->ptb)
            { if(prtb->isInside(x, y)) { return prtb; }}
        return nullptr;
    }// get

    // Отрисовка. --------------------------------------------------------------
    //--------------------------------------------------------------------------
    void draw(QPainter &p) {
        for(Perturbation *prtb: ptb) { prtb->draw(p); }
    }// draw

 private:
    int count = 0;

};// Distortion

/* Набор пачек. ***************************************************************/
/******************************************************************************/
class Anomaly {

 public:
    Anomaly(void) {};
   ~Anomaly(void) { for(Distortion *dtrn: dtr) { delete dtrn; }}


    void add(Distortion *dtrn) {
        dtrn->parent = this;
        dtrn->name = QString::number(count++);
        dtr.append(dtrn);
    }

    Perturbation* get(qreal X, qreal Y) {
        for(Distortion *dtrn: this->dtr)
            { if(Perturbation* prtb = dtrn->get(X, Y)) { return prtb; }}
        return nullptr;
    }// get

    // Отрисовка. --------------------------------------------------------------
    //--------------------------------------------------------------------------
    void draw(QPainter &p) {
        for(Distortion *dtrn: dtr) { dtrn->draw(p); }
    }// draw

 private:
    int count = 0;
    QList<Distortion*> dtr;

};// Anomaly

/* Рисование кружков. *********************************************************/
/******************************************************************************/
class WBall : public QWidget {
    Q_OBJECT

 public:
    explicit WBall(QWidget *parent = nullptr);
            ~WBall();

    QTimer timer;

 public slots:
    void on_timer(void);

 protected:
    virtual void paintEvent(QPaintEvent *evt) override;
    virtual void mousePressEvent(QMouseEvent *evt) override;

 private slots:
    void on_btRun_clicked();
    void on_btStp_clicked();

    void on_pushButton_clicked();

private:
    Ui::WBall *ui;
    QVector<Ball> bll;
    Anomaly anomaly;

};// WBall

//------------------------------------------------------------------------------
#endif // WBALL_H

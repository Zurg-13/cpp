// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include "dbg.h"

#include "WBall.h"
#include "ui_WBall.h"


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
#define RND ((double)qrand()/RAND_MAX)

WBall::WBall(QWidget *parent) : QWidget(parent), ui(new Ui::WBall) {
    ui->setupUi(this);

    this->timer.setInterval(40);
    connect(&timer, &QTimer::timeout, this, &WBall::on_timer);

    int w = this->width()-50, h = this->height();


/*
    qreal S = 0.05, R = 50;
    for(int i=0; i < 13; i++) {
        Ball ball(w*RND, h*RND, S-RND*2*S, S-RND*2*S, R+R*2*RND);
        while([this](Ball &ball) -> bool {
            for(Ball &b: this->bll)
                { if(b.dist(ball) <= b.r + ball.r) { return true; }}
            return false; }(ball))
            { ball = Ball(w*RND, h*RND, S-RND*2*S, S-RND*2*R, R+R*2*RND); }
        bll.append(ball);
    }// i
*/

    bll.append(Ball(100, 100, 0.5, 0, 50));
    bll.append(Ball(250, 100,-0.5, 0, 50));


}// WBall

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WBall::~WBall() {
    delete ui;
}//~WBall

// Событие отрисовки. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WBall::paintEvent(QPaintEvent */*evt*/) {
    int w = this->width()-50, h = this->height();
    QPainter p(this);

/*
    anomaly.draw(p);
*/

    p.setPen(Qt::black);
    for(int i=0; i<this->bll.size(); i++) {
        Ball &ball = this->bll[i];
        for(int j=i+1; j<this->bll.size(); j++) {
            ball.calc(this->bll[j]);
            ball.drawCollision(p, this->bll[j]);
        }// j
        ball.move(w, h); ball.draw(p);
    }// ball

}// paintEvent



// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void WBall::mousePressEvent(QMouseEvent *evt) {

    if(Perturbation *pbtn = anomaly.get(evt->x(), evt->y())) {

        pbtn->color = Qt::red;
        pbtn->push();

    }

}// mousePressEvent

// Пуск. -----------------------------------------------------------------------
//------------------------------------------------------------------------------
void WBall::on_btRun_clicked() { timer.start(); }

// Стоп. -----------------------------------------------------------------------
//------------------------------------------------------------------------------
void WBall::on_btStp_clicked() { timer.stop(); }

// Срабатывание таймера. -------------------------------------------------------
//------------------------------------------------------------------------------
void WBall::on_timer(void) {
    static int cnt = 0;
    this->setWindowTitle(QString::number(cnt++));
    this->repaint();
}// on_timer

// Создать аномалию. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WBall::on_pushButton_clicked() {

/*
    Distortion *one = new Distortion();
        one->add(new Perturbation(100, 100, 50));
        one->add(new Perturbation(100, 200, 50));
        one->add(new Perturbation(100, 300, 50));
    this->anomaly.add(one);

    Distortion *two = new Distortion();
        two->add(new Perturbation(200, 200, 50));
        two->add(new Perturbation(300, 200, 50));
        two->add(new Perturbation(400, 200, 50));
    this->anomaly.add(two);
*/

}// on_pushButton_clicked


// Отрисовка. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void Perturbation::draw(QPainter &p) {
    p.setPen(this->color);
    p.drawEllipse(x-r, y-r, r*2, r*2);
    p.drawText(x, y, this->name);
    p.drawText(x, y+10, "x:" + QString::number(x));
    p.drawText(x, y+20, "y:" + QString::number(y));


    if(isMove()) {

        calc();

        if((prev && prev->dist(this) >= r*2)
        || (next && next->dist(this) >= r*2))
        {
            this->color = Qt::darkYellow;
            stop();
        } else {

/*
            if(dest && dest->isInside(x, y, 10)) {
                dest->push(this);
                this->jamp(dest->x, dest->y);
                this->stop();

                this->color = Qt::green;
                dest->color = Qt::red;
            }// pbtn
*/

        }// else // if(prev && prev->dist(this) >= r)
    }// isMove()


}// draw

//------------------------------------------------------------------------------

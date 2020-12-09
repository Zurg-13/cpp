// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QTimer>
#include <QtMath>
#include <QRandomGenerator>

#include "env.h" // Глобальная среда приложения.
#include "std.h"
#include "dbg.h"

#include "FMain.h"
#include "ui_FMain.h"


// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define ERR(m) E::err(STR("MAIN: ") % m)
#define IMP(m) E::imp(STR("MAIN: ") % m)
#define OTH(m) E::oth(STR("MAIN: ") % m)

// Дополнительные функции. ****************************************************/
/******************************************************************************/

/* FMain **********************************************************************/
/******************************************************************************/

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete tail;
    delete ui;
}//~FMain

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/ico.ico"));
//    QTimer::singleShot(0, [this] { E::Log->place(this); E::Log->show(); });

    this->tail = new Part(
        "Начало", {}, [](int &x, int &y) { Q_UNUSED(x); Q_UNUSED(y); }
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            Q_UNUSED(dna);

            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(Qt::gray, Qt::NoBrush));
            pntr.drawEllipse(x-50, y-50, 100, 100);
            pntr.drawText(x, y, "Сброс - начать заново.");
            pntr.drawText(x, y+30, "Генерация - внести изменения.");
        });

}// FMain

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No )) { QApplication::quit(); }
}// on_aExit_triggered

// Событие отрисовки. ----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::paintEvent(QPaintEvent */*evt*/) {
    QPainter pntr(this);

    this->tail->draw(pntr, 150, 150);


}// paintEvent

// Показать лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aLog_triggered() { E::Log->place(this); E::Log->show(); }

// Генерация. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void morf(Part *part) {
    for(Form &form: part->dna) {
        qreal val = form.val;

//        qreal rnd = QRandomGenerator::global()->generateDouble() - 0.5;
        qreal rnd = 0.5 - (qreal)qrand()/RAND_MAX;
        form.val = val + (val/10)*rnd;
    }// form

    for(Part *chld: part->dsc) { morf(chld); }
}// morf

void FMain::on_btGen_clicked() {
    morf(this->tail); repaint();
}// on_btGen_clicked

// Сбросить. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btReset_clicked() {
    // Сборка существа.
    const std::function<void(int &x, int &y)> STAB = [](int &x, int &y)
        { Q_UNUSED(x); Q_UNUSED(y); };

    this->tail = new Part(
        "Хвост", {
            Form(99, 0.5) // длина
          , Form(10, 0.5) // ширина
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0xd77d31), Qt::SolidPattern));

            int L = dna[0].val, W = dna[1].val;
            pntr.drawEllipse(x, y, L, W);

        }, {

    new Part(
        "Тело", {
            Form(99, 0.5) // жирнота
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0xd77d31), Qt::SolidPattern));

            int W = dna[0].val, H = dna[0].std;
            pntr.drawEllipse(x-W/2, y-H/2, W, H);

        }, {

    new Part(
        "Голова", {}
      , [](int &x, int &y){ Q_UNUSED(x); y = y-50; }
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            Q_UNUSED(pntr); Q_UNUSED(dna); Q_UNUSED(x); Q_UNUSED(y)

            pntr.drawRect(x-2, y-2, 4, 4);

        }, {

    new Part(
        "Уши", {
            Form(5*M_PI/4, 0.5) // наклон
          , Form(40, 0.5) // длина
          , Form(15, 0.5) // ширина
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(Qt::gray, Qt::SolidPattern));

            qreal a = dna[0].val;
            qreal xT = dna[1].val*qCos(a), yT = dna[1].val*qSin(a);
            qreal xM = dna[2].val*qCos(a+M_PI/2);
            qreal yM = dna[2].val*qSin(a+M_PI/2);
            qreal xB = dna[2].val*qCos(a-M_PI/2);
            qreal yB = dna[2].val*qSin(a-M_PI/2);

/*
            int sX = x-10, sY = y-10;
            pntr.drawText(sX+xB, sY+yB, "B");
            pntr.drawText(sX+xT, sY+yT, "T");
            pntr.drawText(sX+xM, sY+yM, "M");
*/
            QPointF lft[3] = {
                {x-10+xB, y-10+yB}, {x-10+xT, y-10+yT}, {x-10+xM, y-10+yM}
            };

/*
            QPointF lft[3] = {
                {(qreal)x, (qreal)y - 20}
              , {(qreal)x-30, (qreal)y-40}
              , {(qreal)x-25, (qreal)y}
            };
*/

            pntr.drawPolygon(lft, 3);

            QPointF rgt[3] = {
                {x+10-xB, y-10+yB}, {x+10-xT, y-10+yT}, {x+10-xM, y-10+yM}
            };

/*
            QPointF rgt[3] = {
                {(qreal)x, (qreal)y-20}
              , {(qreal)x+30, (qreal)y-40}
              , {(qreal)x+25, (qreal)y}};
*/
            pntr.drawPolygon(rgt, 3);
          })

  , new Part(
        "Морда", {
            Form(60, 0.5)
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            qreal w = dna[0].val;

            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0xd77d31), Qt::SolidPattern));
            pntr.drawEllipse(x-w/2, y-30, w, 60);
        })

  , new Part(
        "Глаза", {
            Form(10, 0.5) // размер
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            qreal S = dna[0].val;

            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0x75c1ff), Qt::SolidPattern));
            pntr.drawEllipse(x-13-S/2, y-13-S/2, S, S);
            pntr.drawEllipse(x+13-S/2, y-13-S/2, S, S);


            pntr.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            pntr.drawEllipse(x-13-2, y-13-2, 4, 4);
            pntr.drawEllipse(x+13-2, y-13-2, 4, 4);
        })

  , new Part(
        "Усы", {
            Form(M_PI/6, 0.5) // сектор
          , Form(65, 0.5)   // длина
          , Form(7, 0.5)    // количество
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);

            int count = dna[2].val;
            qreal S = dna[0].val,  D = S/count;
            qreal L = dna[1].val, P = 15;

            pntr.drawEllipse(x-7, y+5-7, 14, 14);
            for(int i=0; i<count; i++) {
                pntr.drawLine(
                    x+P*qCos(i*D - S/2), y+5+P*qSin(i*D - S/2)
                  , x+L*qCos(i*D - S/2), y+5+L*qSin(i*D - S/2));
                pntr.drawLine(
                    x-P*qCos(i*D - S/2), y+5+P*qSin(i*D - S/2)
                  , x-L*qCos(i*D - S/2), y+5+L*qSin(i*D - S/2));
              }// i
        })

    })

  , new Part(
        "Ноги", {
            Form(40, 0.5) // длина
          , Form(10, 0.5) // ширина
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0xd77d31), Qt::SolidPattern));

            qreal L = dna[0].val, W =  dna[1].val;

            pntr.drawEllipse(x-20-W/2, y, W, L);
            pntr.drawEllipse(x+20-W/2, y, W, L);
        })

  , new Part(
        "Грудь", { Form(0, 1), Form(0, 1) }, STAB
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            int size = 120;

            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0xd77d31), Qt::NoBrush));

            pntr.drawRect(x-size, y-size, size*2, size*2);
        })

        })

        }
    );

    repaint();
}// on_btReset_clicked

// Окрас. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "two_dim_rd.h"
#include "reaction_fitzhugh_nagumo.h"
#include "reaction_barkley.h"
#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QThread>
#include <QPainter>
#include <QBrush>

TwoDimRD *tdrd = nullptr;

void FMain::on_btClr_clicked() {
    static bool first = true;
    int w = 48, h = 48, step = 13;

/*
//    ../build/turing --Da 1 --Db 100 --dx 1.0 --dt 0.001 --width 128 --height 128 \
//    --steps 20 --tsteps 1000 --outfile "data.bin" --reaction fitzhugh-nagumo \
//    --parameters "alpha=-0.005;beta=10.0" --pbc
*/


//    if(first) {
        delete tdrd;
        tdrd = new TwoDimRD(1, 100, w, h, 1.0, 0.001, step, 1000);
        tdrd->set_reaction(
            dynamic_cast<ReactionSystem*>(new ReactionFitzhughNagumo()) );
        tdrd->set_parameters("alpha=-0.005;beta=10.0");
        tdrd->set_pbc(true);
        tdrd->time_integrate();
        first = false;
//    }// if(first)



/*
//    ../build/turing --Da 5.0 --Db 0.0 --dx 1.0 --dt 0.001 --width 128 --height 128 \
//    --steps 20 --tsteps 1000 --outfile "data.bin" --reaction barkley \
//    --parameters "alpha=0.75;beta=0.06;epsilon=50.0"
*/


/*
//    if(first) {
        delete tdrd;
        tdrd = new TwoDimRD(5.0, 0.0, w, h, 1.0, 0.001, step, 1000);
        tdrd->set_reaction(
            dynamic_cast<ReactionSystem*>(new ReactionBarkley()) );
        tdrd->set_parameters("alpha=0.75;beta=0.06;epsilon=50.0");
        tdrd->set_pbc(false);
        tdrd->time_integrate();
        first = false;
//    }// if(first)
*/


    QImage img(w*2, h*2, QImage::Format_RGB32);
    double *data = new double[w*h];
    int hue = QRandomGenerator::global()->generateDouble() * 359;
    int val = QRandomGenerator::global()->generateDouble() * 255;

    for(int s=0; s<step; s++) {
        tdrd->write_state_to_array(data, s);
        double min = data[0], max = data[0];
        for(int i=0; i<w; i++) {
            for(int j=0; j<h; j++) {
                int idx = i*w + j;
                int sat = 127 + data[idx]*127;
                QColor clr = QColor::fromHsv(hue, sat, val);

                img.setPixelColor(i+w/2, j+h/2, clr);
                if(i < w/2 && j < h/2) {
                    img.setPixelColor(w/2-i-1, h/2+j, clr);
                    img.setPixelColor(w/2+i, h/2-j-1, clr);
                    img.setPixelColor(w/2-i-1, h/2-j-1, clr);
                } else if(i >= w/2 && j < h/2) {
                    img.setPixelColor((w-i)+3*w/2-1, h/2+j, clr);
                    img.setPixelColor(w/2+i, h/2-j-1, clr);
                    img.setPixelColor((w-i)+3*w/2-1, h/2-j-1, clr);
                } else if(i < w/2 && j >= h/2) {
                    img.setPixelColor(w/2-i-1, h/2+j, clr);
                    img.setPixelColor(w/2+i, (h-j)+3*h/2-1, clr);
                    img.setPixelColor(w/2-i-1, (h-j)+3*h/2-1, clr);
                } else if(i >= w/2 && j >= h/2) {
                    img.setPixelColor((w-i)+3*w/2-1, h/2+j, clr);
                    img.setPixelColor(w/2+i, (h-j)+3*h/2-1, clr);
                    img.setPixelColor((w-i)+3*w/2-1, (h-j)+3*h/2-1, clr);
                }

                if(min > data[idx]) { min = data[idx]; }
                if(max < data[idx]) { max = data[idx]; }
            }// j
        }// i

        ui->lbImg->setPixmap(QPixmap::fromImage(img));
        this->repaint();
        QThread::msleep(200);

    }// s
    delete[] data;


    const int W = 315, H = 315, P = 10;


    QPixmap pic(W+P*2, H+P*2);
    QPainter pnr(&pic);
        pnr.setPen(Qt::lightGray);
        pnr.setBrush(QBrush(Qt::lightGray));
        pnr.drawRect(0, 0, W+P*2, H+P*2);
        pnr.setBrush(QBrush(img));
        pnr.drawEllipse(P, P, W, H);
    ui->lbImg->setPixmap(pic);

    FNC << "end";
}// on_btClr_clicked

//------------------------------------------------------------------------------


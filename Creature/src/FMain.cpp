// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QTimer>
#include <QtMath>
//#include <QRandomGenerator>

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
        "Морда", { Form(0, 1), Form(0, 1) }, STAB
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(QColor(0xd77d31), Qt::SolidPattern));
            pntr.drawEllipse(x-30, y-30, 60, 60);
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

//------------------------------------------------------------------------------


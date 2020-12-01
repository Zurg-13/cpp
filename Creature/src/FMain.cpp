// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QTimer>
#include <QtMath>

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
    QTimer::singleShot(0, [this] { E::Log->place(this); E::Log->show(); });

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
            pntr.setBrush(QBrush(Qt::gray, Qt::SolidPattern));

            int L = dna[0].val, W = dna[1].val;
            pntr.drawEllipse(x, y, L, W);

        }, {

    new Part(
        "Тело", {
            Form(99, 0.5) // жирнота
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(Qt::gray, Qt::SolidPattern));

            int W = dna[0].val, H = dna[0].std;
            pntr.drawEllipse(x-W/2, y-H/2, W, H);

        }, {

    new Part(
        "Голова", {}
      , [](int &x, int &y){ Q_UNUSED(x); y = y-50; }
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            Q_UNUSED(pntr); Q_UNUSED(dna); Q_UNUSED(x); Q_UNUSED(y)
        }, {

    new Part(
        "Уши", {
            Form(M_PI/2, 0.5) // наклон
          , Form(30, 0.5) // длина
          , Form(30, 0.5) // ширина
        }
      , STAB, [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
              pntr.setPen(Qt::black);
              pntr.setBrush(QBrush(Qt::gray, Qt::SolidPattern));

              qreal a = dna[0].val;
              int xB = dna[2].val*qCos(a+M_PI/2);
              int yB = dna[2].val*qSin(a+M_PI/2);
              int xT = dna[1].val*qCos(a), yT = dna[1].val*qSin(a);


              QPointF lft[3] = {
                  {(qreal)x, (qreal)y - 20}
                , {(qreal)x-30, (qreal)y-40}
                , {(qreal)x-25, (qreal)y}};
              pntr.drawPolygon(lft, 3);

              QPointF rgt[3] = {
                  {(qreal)x, (qreal)y-20}
                , {(qreal)x+30, (qreal)y-40}
                , {(qreal)x+25, (qreal)y}};
              pntr.drawPolygon(rgt, 3);
          })

  , new Part(
          "Морда", { Form(0, 1), Form(0, 1) }, STAB
        , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
              pntr.setPen(Qt::black);
              pntr.setBrush(QBrush(Qt::gray, Qt::SolidPattern));

              pntr.drawEllipse(x-30, y-30, 60, 60);
          })

  , new Part(
          "Глаза", { Form(0, 1), Form(0, 1) }, STAB
        , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
              pntr.setPen(Qt::black);
              pntr.setBrush(QBrush(Qt::gray, Qt::NoBrush));

              pntr.drawEllipse(x-13-5, y-13-5, 10, 10);
              pntr.drawEllipse(x+13-5, y-13-5, 10, 10);
          })

  , new Part(
          "Усы", { Form(0, 1), Form(0, 1) }, STAB
        , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
              pntr.setPen(Qt::black);
              pntr.setBrush(QBrush(Qt::gray, Qt::NoBrush));

              int count = 7;
              qreal S = M_PI/6,  D = S/count;
              pntr.drawEllipse(x-7, y+5-7, 14, 14);
              for(int i=0; i<count; i++) {
                    pntr.drawLine(
                        x+15*qCos(i*D - S/2), y+5+15*qSin(i*D - S/2)
                      , x+65*qCos(i*D - S/2), y+5+65*qSin(i*D - S/2));
                    pntr.drawLine(
                        x-15*qCos(i*D - S/2), y+5+15*qSin(i*D - S/2)
                      , x-65*qCos(i*D - S/2), y+5+65*qSin(i*D - S/2));
              }// i

          })

    })

  , new Part(
        "Ноги", { Form(0, 1), Form(0, 1) }, STAB
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(Qt::gray, Qt::NoBrush));

            pntr.drawEllipse(x-20-5, y, 10, 40);
            pntr.drawEllipse(x+20-5, y, 10, 40);
        })

  , new Part(
        "Грудь", { Form(0, 1), Form(0, 1) }, STAB
      , [](QPainter &pntr, const QList<Form> &dna, int x, int y) {
            int size = 120;

            pntr.setPen(Qt::black);
            pntr.setBrush(QBrush(Qt::gray, Qt::NoBrush));

            pntr.drawRect(x-size, y-size, size*2, size*2);
        })

        })

        }
    );

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

//------------------------------------------------------------------------------


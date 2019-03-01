// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
#include <QTime>

#include "_srv/Srv/dbg.h"
#include "_srv/Srv/std.h"

#include "ZLogger.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "WAnimation.h"
#include "WSound.h"
#include "FProc.h"
#include "WTextBrowzer.h"
#include "WAttention.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WAnimation *wgAnimation;
extern WSound *wgSound;
extern FProc *fmProc;
extern WTextBrowzer *wgTextBrowzer;
extern WAttention *wgAttention;



// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);

}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}// ~FMain

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Отладка -> проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    FNC << R"(/ bgn)";


    FNC << R"(\ end)";
}// on_aTest_triggered

// WAnimation. -----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btAnimation_clicked() {
    wgAnimation->show();
}// on_btAnimation_clicked

// WSound. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btSound_clicked() {
    wgSound->show();
}// on_btSound_clicked

// Проверка работы виртуальных деструкторов. -----------------------------------
//------------------------------------------------------------------------------
struct One {
    One() { FNC << "ok"; }
    virtual ~One() { FNC << "ok"; }
    virtual int run();
};// One
int One::run() { return 1; }

struct Two : public One {
    Two() : One() { FNC << "ok"; }
   ~Two() { FNC << "ok"; }
    int run();
};// Two
int Two::run() { return 2; }

struct Thr : public One {
    Thr() : One() { FNC << "ok"; }
    virtual ~Thr() { FNC << "ok"; }
    virtual int run();
};// Thr
int Thr::run() { return 3; }

void FMain::on_btVirtualD_clicked() {
    FNC << R"(/ bgn)";
    FNC << endl;

    FNC << "new two"; One *two = new Two;
    FNC << "two.run" << two->run();
    FNC << "del two"; delete two;

    FNC << endl;

    FNC << "new thr"; One *thr = new Thr;
    FNC << "thr.run" << thr->run();
    FNC << "del thr"; delete thr;

    FNC << endl;
    FNC << R"(\ end)";
}// on_btVirtualD_clicked

// Запуск процессов через консоль. ---------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btProc_clicked() {
    fmProc->show();
}// on_btProc_clicked

// Логирование. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btLogger_clicked() {
    static int cnt = 0;
    static ZLogger log(QDir("d:/Temp/22"), QString("test"), 100, 3);

    log.add("TST", STR("%1: Сообщение в лог файл.").arg(cnt++));
}// on_btLogger_clicked

// Работа с QTextBrowzer. ------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btTB_clicked() {
    wgTextBrowzer->show();
}// on_btTB_clicked

// Работа с WAttention. --------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_pushButton_clicked() {
    if(wgAttention->isVisible()) { wgAttention->hide(); }
    else                         { wgAttention->show(); }
}// on_pushButton_clicked

// Приведение типов. -----------------------------------------------------------
//------------------------------------------------------------------------------
struct A {
    A():a(0), aa(0) {}
    int a, aa;
};
struct B : A {
    B():b(0) {}
    int b;
};
struct D {
    D():d(0) {}
    float d;
};
struct C : A, D {
    C():c(0){}
    double c;
};

void FMain::on_btCast_clicked() {



  {
    C* pC = new C;
    A* pA = static_cast<A*>(pC);
    D* pD = static_cast<D*>(pC);
    B* pB = static_cast<B*>(pA);

    DBG << "STD:" << pC << pD << pA;

    DBG << "STD:" << &(pB->b) << pD << pA;
    pB->b = 100; /* записю в pB->b перекрываем переменную pC->d */
    DBG << "ERR:" << pC->a << pC->aa << pC->d;
  }


  {
    C* pC = new C;
    A* pA = dynamic_cast<A*>(pC);
    D* pD = dynamic_cast<D*>(pC);

/* Не компилируется, требуется виртуальность.
    B* pB = dynamic_cast<B*>(pA);
*/
  }

}// on_btCast_clicked

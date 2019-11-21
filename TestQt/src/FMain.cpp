// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
#include <QTime>

#include "_srv/Srv/dbg.h"
#include "_srv/Srv/std.h"
#include "_srv/Srv/lst.h"

#include "ZLogger.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "WAnimation.h"
#include "WSound.h"
#include "FProc.h"
#include "WTextBrowzer.h"
#include "WAttention.h"
#include "WActive.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WAnimation *wgAnimation;
extern WSound *wgSound;
extern FProc *fmProc;
extern WTextBrowzer *wgTextBrowzer;
extern WAttention *wgAttention;
extern WActive *wgActive;


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


//template <typename T>
//QString TST(QLiast<T>)

// Отладка -> проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
typedef QPair<QString, QString> tst_pair;
void FMain::on_aTest_triggered() {
    FNC << R"(/ bgn)";


    QMap<QString, QString> map = {{"1", "one"}, {"2", "two"}, {"3", "thr"}};
    QList<QString> lst = {"one", "two", "thr"};

    FNC << "map:" << map;
    FNC << "lst:" << LST(map);

    FNC << "str:" << BLD(lst);
    FNC << "bld:" << BLD(LST(map));

    FNC << "l_s:" << BLD(
        lst, [](const QString &val){ return "{LBD:" + val + ":LBD}"; }
      , QString("{SEP}") );

    FNC << "tst:" << BLD(
        lst, [](const QString &val){ return "{LBD:" + val + ":LBD}"; }
      , QString("{SEP}") );


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


//#include "windows.h"

// Разное. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_bt_clicked() {
    DBG << R"(/bgn)";

/*
    APPBARDATA pabd;
    pabd.hWnd = reinterpret_cast<HWND>(this->winId());
    pabd.uCallbackMessage = 40032;
    pabd.cbSize = sizeof(pabd);

    if(SHAppBarMessage(ABM_NEW, &pabd) == FALSE) {
        DBG << "err abm_new";
    }

    pabd.uEdge = ABE_TOP;
    pabd.rc.top = 0;
    pabd.rc.left = 0;
    pabd.rc.right = 800;
    pabd.rc.bottom = 50;
    pabd.hWnd = reinterpret_cast<HWND>(this->winId());
    pabd.cbSize = sizeof(pabd);

    if (SHAppBarMessage(ABM_SETPOS, &pabd) == FALSE) {
        DBG << ("err abm_setpos");
    }

    this->setGeometry(0, 0, 800, 50);
*/

    DBG << R"(\end)";
}// on_bt_clicked

// ActiveX. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btActiveX_clicked() {
    wgActive->show();
}// on_btActiveX_clicked

// Очистка списка указателей. --------------------------------------------------
//------------------------------------------------------------------------------
#include <QPointer>
void FMain::on_btListClear_clicked() {
    FNC << R"(/bgn)";

    QList<One*> lst;
    for(int i=0; i<3; i++) {
        lst.append(new One());
    }// i
    lst.clear();

    FNC << R"(\end)";
}// on_btListClear_clicked

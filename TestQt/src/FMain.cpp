// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QMessageBox>
#include <QTime>

#include "_srv/Srv/dbg.h"
#include "_srv/Srv/std.h"
#include "_srv/Srv/lst.h"
#include "_srv/Srv/xml.h"

#include "ZLogger.h"

#include "WAnimation.h"
#include "WSound.h"
#include "FProc.h"
#include "WTextBrowzer.h"
#include "WAttention.h"
#include "WActive.h"
#include "WBall.h"

#include "FMain.h"
#include "ui_FMain.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WAnimation *wgAnimation;
extern WSound *wgSound;
extern FProc *fmProc;
extern WTextBrowzer *wgTextBrowzer;
extern WAttention *wgAttention;
extern WActive *wgActive;
extern WBall *wgBall;

extern FMain *fmMain;

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

    QString xml =
        "<root>"
            "<val_one>1</val_one>"
            "<val_two>2</val_two>"
            "<val_thr>3</val_thr>"
        "</root>";

    FNC << "val:";
    FNC << "root:" << VAL("root", xml) << "val_two:" << VAL("val_two", xml);

    FNC << "cut:";
    FNC << "root:" << CUT("root", xml) << "val_two:" << CUT("val_two", xml);

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

    this->test_var_val = "123";
    DBG << "val:" << this->test_var;

    this->test_var_val = "456";
    DBG << "val:" << this->test_var;


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

// WLog. -----------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "WLog.h"
void FMain::on_btWLog_clicked() {
//    static QSharedPointer<WLog> tst_log(new WLog(5));
    static WLog* tst_log(new WLog(5));
    static int cnt = 0;

    if(!tst_log->isVisible()) { tst_log->show(); }

    tst_log->add(QString("%1: msg").arg(cnt++));
}// on_btWLog_clicked

// QSharedPointer. -------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QSharedPointer>
QSharedPointer<One> tst_shared_func(void)
    { return QSharedPointer<One>(new One); }
void FMain::on_btSharedPtr_clicked() {
    static bool first = true;
    static QMap<bool, QSharedPointer<One>> map;

    FNC << "first:" << first;

    map[first] = tst_shared_func();

    first = !first;
}// on_btSharedPtr_clicked

// QtConcurrent. ---------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QtConcurrent/QtConcurrent>
#include <QThread>
void FMain::on_btQtConcurrent_clicked() {
    FNC << R"(/ bgn)";


    unsigned long sec = 5;
    QFuture<unsigned long> ftr = QtConcurrent::run([sec]() -> unsigned long {
        QThread::sleep(sec); FNC << "run:" << sec; return sec; });



/*
    QList<unsigned long> lst({1, 2, 3});
    std::function<void(unsigned long&)> fn = [](unsigned long &elm) {
        QThread::sleep(elm); FNC << "map:" << elm; };
    QtConcurrent::map(lst, fn);
*/

/*
    QVector<QString> words = {"one", "two", "three", "four"};
    std::function<QString(const QString &word)> func = [](const QString &word) {
        return word + word; };
    QFuture<QString> result = QtConcurrent::mapped(words, func);
*/

    FNC << R"(\ end)";
}// on_btQtConcurrent_clicked

// WLogAdv. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "WBrd.h"
#include <QList>
#include <QPair>
void FMain::on_btLogAdv_clicked() {
    FNC << R"(/ bgn)";


    static WBrd board(nullptr); board.show();

    QList<QPair<QString, QString>> lst {
        {"one", "111"}
      , {"two", ""}
      , {"thr", "333"}};

    board.rift();
    for(const QPair<QString, QString> &pair: lst) {
        board.post(pair.first, pair.second);
    }// pair
    board.rift();


    FNC << R"(\ end)";
}// on_btLogAdv_clicked

// WBall. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btBall_clicked(){
    wgBall->show();
}

//------------------------------------------------------------------------------


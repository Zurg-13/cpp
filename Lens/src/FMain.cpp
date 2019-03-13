// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
#include <QBuffer>
#include <QScreen>
#include <QThread>
#include <QMouseEvent>
#include <QCursor>
#include <QTimer>


#include "_srv/Srv/dbg.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "WLens.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WLens *wgLens;

// Отмасштабировать прямоугольник. ---------------------------------------------
//------------------------------------------------------------------------------
template <typename T>
T SCL(T val, double scl) { return static_cast<T>(val * scl); }

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);

    // Инициализация.
//    this->keyESC = new QShortcut(this); this->keyESC->setKey(Qt::Key_Escape);
    qApp->installEventFilter(this);

    // CONNECT.
//    connect(this->keyESC, &QShortcut::activated, this, &FMain::on_key_ESC);

}// FMain

// Выполняется при показе формы. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::showEvent(QShowEvent* /*evt*/) {

    // Инициализация.
    this->scale = 10;
    this->state = State::Edit;
//    this->geom_std = this->geometry();

    ui->lbImg->setFocus();
}// showEvent

// Выпрлняется при закрытии главного окна. -------------------------------------
//------------------------------------------------------------------------------
void FMain::closeEvent(QCloseEvent* /*evt*/) { QApplication::quit(); }

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
/*
    delete old_pic;
*/

    delete ui;
}//~FMain

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Перехват событий. -----------------------------------------------------------
//------------------------------------------------------------------------------
bool FMain::eventFilter(QObject */*obj*/, QEvent *evt) {

    // События клавиатуры.
    if(evt->type() == QEvent::KeyPress) {
        QKeyEvent *kEvt = static_cast<QKeyEvent*>(evt);
        if(kEvt->key() == Qt::Key_Escape)
            { this->on_key_ESC(); }
    }// if(evt->type() == QEvent::KeyPress)

    // События мыши.
    if(evt->type() == QEvent::MouseMove) {
       QMouseEvent *mEvt = static_cast<QMouseEvent*>(evt);

        if(this->state == State::Pick) {

            int lw = 200, lh = 200;
            int x = mEvt->pos().x() - (lw/this->scale)/2;
            int y = mEvt->pos().y() - (lh/this->scale)/2;

            wgLens->setPic(QCursor::pos(), ui->lbImg->pixmap()
                ->copy(x, y, lw/this->scale, lh/this->scale));

       }// if(!this->pic.isNull())

    }// if (evt->type() == QEvent::MouseMove)

    return false;
}// eventFilter

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::mousePressEvent(QMouseEvent *evt) {
    this->mouse_press_bgn = QTime::currentTime();
    this->mouse_press_btn = evt->button();

    this->pos_bgn = QCursor::pos();
}// mousePressEvent

// Отпускание кнопки мыши. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::mouseReleaseEvent(QMouseEvent* evt) {

    // Разовый клик.
    if(this->mouse_press_btn == evt->button()
    && this->mouse_press_bgn.msecsTo(QTime::currentTime()) < 500)
    {
        this->on_mouse_click(evt); this->mouse_press_btn = Qt::NoButton;
    } else {
        DBG << "bgn:" << this->pos_bgn << "cur:" << QCursor::pos();
    }// else // if(this->mouse_press_btn == evt->button()

}// mouseReleaseEvent

// Клик мыши. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_mouse_click(QMouseEvent* /*evt*/) {
    DBG << "on_mouse_click";

    if(this->state == State::Pick) {
        this->state = State::Edit;
        wgLens->showTool();
    } else {
        this->state = State::Pick;
        wgLens->hideTool();
    }// if(this->state == State::Show)

}// on_mouse_click

// Отладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
//    int w = ui->lbImg->width(), h = ui->lbImg->height();
    QList<QScreen*> scr = QGuiApplication::screens();
    QScreen *screen = scr[0];

    if(screen) {

        FNC << "g:" << this->geometry() << "fg:" << this->frameGeometry();

        ui->mm->hide();
        this->geom_old = this->geometry();
        this->setWindowFlags(
            Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
/*
        this->setGeometry(screen->virtualGeometry());
*/
        this->show();


/*
        int x = screen->virtualGeometry().x()
          , y = screen->virtualGeometry().y()
          , w = screen->virtualGeometry().width()
          , h = screen->virtualGeometry().height();
*/

        int x = 0
          , y = 0
          , w = screen->geometry().width()/2
          , h = screen->geometry().height()/2;

//        this->pic = screen->grabWindow(0, x, y, w, h);
        ui->lbImg->setPixmap(screen->grabWindow(0, x, y, w, h));

        wgLens->show();
    }// if(screen)

}// on_aTest_triggered

// Нажатие ESC. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_key_ESC(void) {
    DBG << "on_key_ESC";

    if(this->geom_old.isEmpty()) { return; }

    ui->lbImg->setPixmap(QPixmap());
    this->setWindowFlags(Qt::Window);
    ui->mm->show();
    this->show();

    QApplication::processEvents();

    this->setGeometry(this->geom_old);
}// on_key_ESC


//------------------------------------------------------------------------------
void FMain::on_upd_sze(QRect geom) {
    FNC << "ok";

    this->setGeometry(geom);
}

//------------------------------------------------------------------------------


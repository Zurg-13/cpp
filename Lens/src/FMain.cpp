// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
#include <QBuffer>
#include <QScreen>
#include <QThread>
#include <QMouseEvent>
#include <QCursor>


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
    this->keyESC = new QShortcut(this); this->keyESC->setKey(Qt::Key_Escape);
    qApp->installEventFilter(this);

    // CONNECT.
    connect(this->keyESC, &QShortcut::activated, this, &FMain::on_key_ESC);

}// FMain

// Выполняется при показе формы. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::showEvent(QShowEvent* /*evt*/) {

    // Инициализация.
    this->scale = 10;
    this->state = State::Show;
//    this->geom_std = this->geometry();

    this->setFocus();
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

    if (evt->type() == QEvent::MouseMove) {
       QMouseEvent *mEvt = static_cast<QMouseEvent*>(evt);

        if(!this->pic.isNull() && this->state == State::Show) {
            int lw = 200, lh = 200;
            int x = mEvt->pos().x() - (lw/this->scale)/2;
            int y = mEvt->pos().y() - (lh/this->scale)/2;

            wgLens->setPic(QCursor::pos(), this->pic
                .copy(x, y, lw/this->scale, lh/this->scale)
                .scaled(lw, lh, Qt::KeepAspectRatio, Qt::FastTransformation) );

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

    if(this->state == State::Show) {
        this->state = State::Edit;
        wgLens->showTool();
    } else {
        this->state = State::Show;
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
        ui->mm->hide();
        this->geom_std = this->frameGeometry();
        this->setWindowFlags(
            Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
        this->setGeometry(screen->virtualGeometry());
        this->show();

/*
        delete this->old_pic;
        this->old_pic = new QPixmap(*ui->lbImg->pixmap());
*/

        int x = screen->virtualGeometry().x()
          , y = screen->virtualGeometry().y()
          , w = screen->virtualGeometry().width()
          , h = screen->virtualGeometry().height();
        this->pic = screen->grabWindow(0, x, y, w, h);

        ui->lbImg->setPixmap(this->pic
            .copy()
            .scaled(
                w, h, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation) );

        wgLens->show();
    }// if(screen)

}// on_aTest_triggered

// Нажатие ESC. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_key_ESC(void) {
    DBG << "on_key_ESC";

    if(!this->geom_std.isEmpty()) {

/*
        ui->lbImg->setPixmap(*this->old_pic);
*/

        this->setGeometry(this->geom_std);
    }// if(!this->geom_std.isEmpty())
    this->setWindowFlags(Qt::Widget);

    ui->mm->show(); this->show();
}// on_key_ESC

//------------------------------------------------------------------------------


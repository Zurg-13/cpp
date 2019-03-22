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
    qApp->installEventFilter(this);
    this->rubb_band = new QRubberBand(QRubberBand::Rectangle, this);

    // CONNECT.

}// FMain

// Выполняется при показе формы. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::showEvent(QShowEvent* /*evt*/) {
    this->lower();
}// showEvent

// Выпрлняется при закрытии главного окна. -------------------------------------
//------------------------------------------------------------------------------
void FMain::closeEvent(QCloseEvent* /*evt*/) { QApplication::quit(); }

// Передать изображение в линзу. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::sendImg(const QPoint &pos) {
    int lw = 247/wgLens->scale(), lh = 195/wgLens->scale();
    int x = pos.x() - lw/2, y = pos.y() - lh/2;

    wgLens->setImg(
        QCursor::pos()
      , ui->lbImg->pixmap()->toImage().copy(x, y, lw, lh));
}// sendImg

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
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

            // Отрисовка линзы.
            this->sendImg(mEvt->pos());

            // Отрисовка выделения.
            if(mEvt->buttons() == Qt::MouseButton::LeftButton) {
                this->rubb_band->setGeometry(
                    QRect(this->pos_bgn, mEvt->pos()).normalized() );
            }// if(mEvt->button() == Qt::MouseButton::LeftButton)

        }// if(!this->pic.isNull())

    }// if (evt->type() == QEvent::MouseMove)

    return false;
}// eventFilter

// Нажатие кнопки мыши. --------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::mousePressEvent(QMouseEvent *evt) {
    this->mouse_press_bgn = QTime::currentTime();
    this->mouse_press_btn = evt->button();
    this->pos_bgn = evt->pos();

    // Отрисовка выделения.
    if(this->state == State::Pick) {
        this->rubb_band->setGeometry(QRect(this->pos_bgn, QSize()));
        this->rubb_band->show();
    }// if(this->state == State::Pick)

}// mousePressEvent

// Отпускание кнопки мыши. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::mouseReleaseEvent(QMouseEvent* evt) {
    // Разовый клик.
    if(this->mouse_press_btn == evt->button()
    && this->mouse_press_bgn.msecsTo(QTime::currentTime()) < 200 )
    {
        this->on_mouse_click(evt); this->mouse_press_btn = Qt::NoButton;
    } else if(ui->lbImg->pixmap() != nullptr) {
        int x = this->pos_bgn.x(), y = this->pos_bgn.y();
        int w = abs(x - evt->pos().x()), h = abs(y - evt->pos().y());

        this->pixmap_old = ui->lbImg->pixmap()->copy(x, y, w, h);
        this->rubb_band->hide();
        this->stdShow();
    }// else // if(this->mouse_press_btn == evt->button()

}// mouseReleaseEvent

// Клик мыши. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_mouse_click(QMouseEvent *evt) {
    if(ui->lbImg->pixmap() == nullptr) { return; }

    if(this->state == State::Pick) {
        FNC << "wgLens->showTool";

        this->state = State::Edit;
        wgLens->showTool();
    } else {
        FNC << "wgLens->hideTool";

        this->state = State::Pick;
        wgLens->hideTool();
    }// if(this->state == State::Show)

    //todo: разобраться со смещением.
//    this->sendImg(evt->pos());
}// on_mouse_click

// Отладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    QList<QScreen*> scr = QGuiApplication::screens();
    QScreen *screen = scr[0];

    if(screen) {
        QRect geom = screen->geometry() /*virtualGeometry()*/;

        ui->mm->hide();
        this->geom_old = this->geometry();
        this->pixmap_old = (ui->lbImg->pixmap() == nullptr
            ? QPixmap() : *ui->lbImg->pixmap());
        this->setWindowFlags(
            Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
        this->setGeometry(geom); this->show();

        int x = geom.x(), y = geom.y()
          , w = geom.width() , h = geom.height();
        ui->lbImg->setPixmap(screen->grabWindow(0, x, y, w, h));

        this->setState(State::Pick);
        wgLens->hideTool();
        this->eventFilter(nullptr, new QMouseEvent(
            QEvent::MouseMove, QCursor::pos()
          , Qt::NoButton, Qt::NoButton, Qt::NoModifier ));
    }// if(screen)

}// on_aTest_triggered

// Нажатие ESC. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_key_ESC(void) {
    FNC << "bgn";

    if(this->geom_old.isEmpty()) { return; }

    this->stdShow();
}// on_key_ESC

// Показ формы в стандартном виде. ---------------------------------------------
//------------------------------------------------------------------------------
void FMain::stdShow(void) {
    ui->lbImg->setPixmap(this->pixmap_old);

    this->setWindowFlags(Qt::Window);
    ui->mm->show(); this->show();

    QApplication::processEvents();

    this->setGeometry(this->geom_old);
    this->adjustSize();
}// stdShow

//------------------------------------------------------------------------------
void FMain::enterEvent(QEvent */*evt*/) {
    if(ui->lbImg->pixmap() == nullptr) { wgLens->hide(); }
    else                               { wgLens->show(); }
}// enterEvent

//------------------------------------------------------------------------------
void FMain::leaveEvent(QEvent */*evt*/) {
    if(this->state == State::Pick) { wgLens->hide(); }
}// leaveEvent

//------------------------------------------------------------------------------
void FMain::on_upd_sze(QRect geom) {
    this->setGeometry(geom);
}// on_upd_sze

//------------------------------------------------------------------------------


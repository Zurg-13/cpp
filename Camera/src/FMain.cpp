#include <QMessageBox>
#include <QClipboard>
#include <QPicture>
#include <QCameraInfo>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"

#include "ui_FMain.h"
#include "FMain.h"


/* FMain **********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {
    ui->setupUi(this);
}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
    delete camera;
}// ~FMain

// Выход. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Отладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
#include <QTimer>

void FMain::on_aTest_triggered() {
    FNC << R"(/bgn)";

    camera->reactive();

    FNC << R"(\end)";
}// on_aTest_triggered

// Захват. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btCpt_clicked() {
    FNC << R"(/bgn)";

    if(camera == nullptr) { camera = new Cam(QCameraInfo::defaultCamera()); }
    ui->lbImg->setPixmap(camera->captPix());

    FNC << R"(\end)";
}// on_btCpt_clicke

//------------------------------------------------------------------------------



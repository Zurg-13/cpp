#include "FMain.h"
#include "ui_FMain.h"

FMain::FMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FMain)
{
    ui->setupUi(this);
}

FMain::~FMain()
{
    delete ui;
}

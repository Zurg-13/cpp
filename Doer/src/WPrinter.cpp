#include "WPrinter.h"
#include "ui_WPrinter.h"

WPrinter::WPrinter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WPrinter)
{
    ui->setupUi(this);
}

WPrinter::~WPrinter()
{
    delete ui;
}

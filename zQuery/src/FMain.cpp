// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "dbg.h"
#include "std.h"
#include "xml.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "wgt/WOut.h"
#include "wgt/WCnn.h"

#include "WLog.h"
#include "DSel.h"

// DEFINE. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#define STATE_FLE QString("state.xml")

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WLog *wgLog;
extern DSel *dgSel;
extern WCnn *wgCnn;

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Инициализация.
    wgCnn = new WCnn(this); //todo: это вообще-то не нормально...
    this->wgEdit = new WEdit();
    this->load(FSN(QApplication::applicationDirPath(), STATE_FLE));

    // Внешний вид;
    ui->setupUi(this);
    this->setWindowFlags(
        Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );
    ui->lyLft->addWidget(this->wgEdit);
    ui->lyRgt->addWidget(wgCnn); wgCnn->hide();

}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    DBG << "~FMain";

    delete ui;
}// ~FMain

// Выход. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    {
        this->save(FSN(QApplication::applicationDirPath(), STATE_FLE));
        QApplication::quit();
    }// if(QMessageBox::Yes == QMessageBox::question ...
}// on_aExit_triggered

// Отладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    this->save(FSN(QApplication::applicationDirPath(), STATE_FLE));
}// on_aTest_triggered

// Показать лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aLog_triggered() { wgLog->show(); }

// Показать поддключения. ------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aConn_triggered() { wgCnn->show(); }

// Сохранить состояние в файл. -------------------------------------------------
//------------------------------------------------------------------------------
void FMain::save(QString fnme) {
    QFile file(fnme);

    if(!file.open(QIODevice::WriteOnly))
        { QMessageBox::warning(this, "Ошибка", "Нельзя открыть файл"); return; }

    QXmlStreamWriter doc(&file);
    doc.setAutoFormatting(true);

    doc.writeStartDocument();
        wgCnn->write(doc);  //cnn
        wgEdit->write(doc); //edit
    doc.writeEndDocument();
}// save

// Загрузить состояние из файла. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::load(QString fnme) {
    QFile file(fnme);

    if(!file.exists()) { return; }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        { QMessageBox::warning(this, "Ошибка", "Нельзя открыть файл"); return; }

    wgCnn->state(VAL("cnn", file.readAll()));
}// load

//------------------------------------------------------------------------------

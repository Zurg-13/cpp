// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QMenu>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QSqlDatabase>
#include <QActionGroup>

#include "ui_WConnect.h"
#include "WConnect.h"

#include "std.h"
#include "dbg.h"
#include "xml.h"


// Инициализация. --------------------------------------------------------------
//------------------------------------------------------------------------------
int WConnect::global_count = 0;

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WConnect::WConnect(QWidget *parent) : QWidget(parent), ui(new Ui::WConnect) {

    // Внешний вид.
    ui->setupUi(this);
    ui->edNme->setText(QString("conn-") + STR(count));

    // Типы подключений.
    QActionGroup *group = new QActionGroup(this);
    QMenu *type = new QMenu(this);
    auto ADD = [type, group](QAction *action) {
        group->addAction(action);
        type->addAction(action); action->setParent(type); };

    ADD(ui->aConnOCI); ADD(ui->aConnODBC); ADD(ui->aConnPSQL);
    ADD(ui->aConnMYSQL); ADD(ui->aConnSQLITE);
    ui->aConnOCI->triggered();  // По умолчанию.

    // Меню.
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->aTest);
    menu->addSeparator(); //---------------------
    menu->addMenu(type);  // >>>
    menu->addSeparator(); //---------------------
    menu->addAction(ui->aDelete);
    ui->btMenu->setMenu(menu);

    // Инициализация.

}// WConnect

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WConnect::~WConnect() {
    FNC << "~WConnect";

    delete ui;
}// ~WConnect

// Удалить. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void WConnect::on_aDelete_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение", "Действительно закрыть ?") )
    { emit remove(this); }
}// on_aDelete_triggered

// Проверить подключение. ------------------------------------------------------
//------------------------------------------------------------------------------
void WConnect::on_aTest_triggered() {
    QSqlDatabase db = QSqlDatabase::addDatabase(this->conn_type);
    db.setDatabaseName(url()); db.setUserName(usr()); db.setPassword(pwd());

    QMessageBox::information(
        this, "Проверка подключения"
      ,(db.open() ? QString("Успешно") : db.lastError().text()) );
}// on_aTest_triggered

// Название подключения. -------------------------------------------------------
//------------------------------------------------------------------------------
QString WConnect::nme(void) { return ui->edNme->text().trimmed(); }

// Пользователь. ---------------------------------------------------------------
//------------------------------------------------------------------------------
QString WConnect::usr(void) { return ui->edUsr->text().trimmed(); }

// Пароль. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
QString WConnect::pwd(void) { return ui->edPwd->text().trimmed(); }

// Строка подключения. ---------------------------------------------------------
//------------------------------------------------------------------------------
QString WConnect::url(void) { return ui->edCnn->toPlainText().trimmed(); }

// Запись состояния в XML-документ. --------------------------------------------
//------------------------------------------------------------------------------
void WConnect::write(QXmlStreamWriter &doc) {

    auto TAG = [&doc](QString nme, QString val) -> void {
        doc.writeStartElement(nme);
        doc.writeCharacters(val);
        doc.writeEndElement();
    };// TAG

    doc.writeStartElement("connect");

        TAG("nme", ui->edNme->text().trimmed());
        TAG("usr", ui->edUsr->text().trimmed());
        TAG("pwd", ui->edPwd->text().trimmed());
        TAG("url", ui->edCnn->toPlainText().trimmed());

    doc.writeEndElement();  //connect

}// WConnectwrite

// Восстановить состояние из XML-документа. ------------------------------------
//------------------------------------------------------------------------------
void WConnect::state(const QString &xml) {
    ui->edNme->setText(VAL("nme", xml));
    ui->edUsr->setText(VAL("usr", xml));
    ui->edPwd->setText(VAL("pwd", xml));
    ui->edCnn->setPlainText(VAL("url", xml));
}// parse


// Назначить тип БД. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WConnect::SET(QObject *sender, const QString &type) {
    this->conn_type = type;
    qobject_cast<QMenu*>(sender->parent())->setTitle(
        STR("Тип БД: %1").arg(qobject_cast<QAction*>(sender)->text()) );
}// SET
void WConnect::on_aConnOCI_triggered()   { SET(sender(), "QOCI"); }
void WConnect::on_aConnODBC_triggered()  { SET(sender(), "QODBC"); }
void WConnect::on_aConnMYSQL_triggered() { SET(sender(), "QMYSQL"); }
void WConnect::on_aConnPSQL_triggered()  { SET(sender(), "QPSQL"); }
void WConnect::on_aConnSQLITE_triggered(){ SET(sender(), "QSQLITE"); }

//------------------------------------------------------------------------------

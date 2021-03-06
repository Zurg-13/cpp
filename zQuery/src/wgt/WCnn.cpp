// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "ui_WCnn.h"
#include "WCnn.h"

#include "dbg.h"
#include "xml.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WCnn::WCnn(QWidget *parent) : QWidget(parent), ui(new Ui::WCnn) {
    ui->setupUi(this);
}// WCnn

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WCnn::~WCnn() {
    DBG << "~WCnn";

    delete ui;
}// ~WCnn

// Добавить подключение (действие). --------------------------------------------
//------------------------------------------------------------------------------
void WCnn::on_btAdd_clicked() { addConn(); }

// Добавить подключение. -------------------------------------------------------
//------------------------------------------------------------------------------
WConnect* WCnn::addConn(void) {
    WConnect *conn = new WConnect(this);

    connect(conn, &WConnect::remove, this, &WCnn::remove_conn);
    cnn.push_back(conn);
    ui->lyCnn->addWidget(conn);

    return conn;
}// addConn

// Скрыть форму. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WCnn::on_btHide_clicked() { this->hide(); }

// Удалить подключение. --------------------------------------------------------
//------------------------------------------------------------------------------
void WCnn::remove_conn(WConnect *conn) {
    for(QList<WConnect*>::iterator it = cnn.begin(); it != cnn.end(); it++)
        { if((*it) == conn) { cnn.erase(it); conn->deleteLater(); return; }}
}// remove_conn

// Запись состояния в XML-документ. --------------------------------------------
//------------------------------------------------------------------------------
void WCnn::write(QXmlStreamWriter &doc) {

    doc.writeStartElement("cnn");

        for(WConnect *connect: this->cnn) { connect->write(doc); }

    doc.writeEndElement();  //cnn

}// write

// Восстановить состояние из XML-документа. ------------------------------------
//------------------------------------------------------------------------------
void WCnn::state(const QString &xml) {
    for(QString &conn: LST("connect", xml)) { addConn()->state(conn); }
}// state

//------------------------------------------------------------------------------



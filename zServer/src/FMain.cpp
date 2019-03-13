#include <functional>

#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QTime>
#include <QNetworkProxyFactory>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QtConcurrentMap>

#include "ui_FMain.h"
#include "FMain.h"

// Дополнительные функции. ****************************************************/
/******************************************************************************/

// Подмена символов: & = &amp, < = &lt, > = &gt. ---------------------------
//--------------------------------------------------------------------------
QString ESCPG(QString val) {
    return val
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;");
}// ESCPG

/* FMain **********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/ico.ico"));

    //Инициализация.
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    tcp = new QTcpServer(this);
    connect(tcp, &QTcpServer::newConnection, this, &FMain::new_connect);

    //Меню серверов.
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->aClearHdl);
    ui->btMenuHdl->setMenu(menu);

}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    tcp->close();
    delete ui;
}// ~FMain

// Добавить. -------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btAdd_clicked() {
    WHandler *handler = new WHandler(this);

    connect(handler, &WHandler::remove, this, &FMain::remove_handler);
    hdl.push_back(handler);

    ui->lyHdl->addWidget(handler);
}// on_btAdd_clicked

// Удалить прослушиватель. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::remove_handler(WHandler *handler) {

    for(QList<WHandler*>::iterator it = hdl.begin(); it != hdl.end(); it++){
        if((*it) ==  handler) { hdl.erase(it); handler->deleteLater(); return; }
    }// it

}// on_btAdd_clicked

// Пуск/Стоп. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btStart_clicked() {
    auto SET_BTN = [&](const QString &ttl, const QString &sht) -> void {
        ui->btStart->setText(ttl); ui->btStart->setStyleSheet(sht); };

    if(tcp->isListening()) {
        tcp->close();
        log(tr("Прослушиватель остановлен"), Qt::lightGray);
        SET_BTN("Пуск", "color: limegreen;");
    } else {

        bool ok;
        unsigned short port = ui->edPort->text().toUShort(&ok);
        if(!ok) { log(tr("Введите корректный порт"), Qt::red); return; }

        if(tcp->listen(QHostAddress::Any, port)) {
            log(tr("Прослушиватель запущен"), Qt::lightGray);
            SET_BTN("Стоп", "color: crimson;");
        } else {
            log(tr("Ошибка прослушивателя: ") + tcp->errorString(), Qt::red);
        }// else // if(!tcp->listen(QHostAddress::Any, ui->edPort->text.toInt()))
    }// else // if(tcp->isListening())

}// on_btStart_clicked

// Новое подключение. ----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::new_connect(void) {
    QTcpSocket *socket = tcp->nextPendingConnection();

    spc();
    log(tr("Новое подключение: ")
       + QString::number(reinterpret_cast<ulong>(socket))
       + " (" + socket->peerAddress().toString() + ")", Qt::lightGray);

    connect(socket, &QTcpSocket::readyRead, this, &FMain::read_socket);
    connect(socket, &QTcpSocket::disconnected, this, &FMain::close_socket);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error_socket(QAbstractSocket::SocketError)) );

//    sct.push_back(socket);
}// new_connect

// Чтение сокета. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::read_socket(void) {
    static QByteArray OU(" <<< "), IN(" >>> "), OK("<ok> DEFAULT </ok>");
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream out(socket);

    QString read = QString::fromUtf8(socket->readAll());
    int pos = read.indexOf('/');

    QString type = read.left(pos).trimmed();
    QString addr = read.mid(pos, read.indexOf("HTTP/") - pos).trimmed();
    QString path = addr.left(addr.indexOf('?')).mid(1).trimmed();

    auto SND = [&](const QString &log, const QByteArray &ans, const QColor &clr)
    -> void {
        this->log(IN + "type:" + type + ", path:" + path, clr);
        this->log(OU + log, clr);
        out << ans; socket->disconnectFromHost();
    };// SEND

    for(WHandler *handler: hdl){
        if(0 == QString::compare(path, handler->path, Qt::CaseInsensitive)) {
            SND(handler->plane_text(), handler->answer(), handler->color);
            return;
        }// if(0 == QString::compare(path, handler->path, Qt::CaseInsensitive))
    }// handler

    SND(OK, OK, Qt::darkRed);
}// read_socket

// Закрытие сокета. ------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::close_socket(void) {
    QTcpSocket *socket = (qobject_cast<QTcpSocket*>(sender()));

    log(tr("Сокет закрыт.")
      + QString::number(reinterpret_cast<ulong>(socket))
      + " (" + socket->peerAddress().toString() + ")", Qt::lightGray);

}// close_socket

// Ошибка сокета. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::error_socket(QAbstractSocket::SocketError) {
    log(tr("Ошибка сокета: ")
      +(qobject_cast<QTcpSocket*>(sender()))->errorString()
      , Qt::red );
}// error_socket

// Логирование. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::log(const QString &msg, const QColor &clr) {
    if(!ui) { return; }
    ui->tbLog->cursorForPosition(QPoint(0, 0)).insertHtml(
        "<h style='color:" + clr.name() + ";'>"
      + "[" + QTime::currentTime().toString() + "] "
      + ESCPG(msg) + "</h>" + "<br>" );
}// log

// Разделитель. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::spc(void) {
    if(!ui) { return; }
    ui->tbLog->cursorForPosition(QPoint(0, 0)).insertHtml("<br>");
}// spc

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btClearLog_clicked() { ui->tbLog->clear(); }

// Сохранить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btSave_clicked() {
    QString fnme = QFileDialog::getSaveFileName(
        this, tr("Выбор когфигурации"), qApp->applicationDirPath(), "*.zconf" );
    if(fnme.isEmpty()) { return; }

    QFile file(fnme);
    QDataStream out(&file);

    if(file.open(QIODevice::WriteOnly)) {
        out << hdl.length();
        for(WHandler *handler: hdl) {
            out << handler->answer_type_name() << handler->color
                << handler->path << handler->plane_text();
        }// handler
        file.close();
    }// if(file.open(QIODevice::ReadWrite))

}// on_btSave_clicked

// Загрузить конфигурвцию. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btLoad_clicked() {
    QString fnme = QFileDialog::getOpenFileName(
        this, tr("Выбор когфигурации"), qApp->applicationDirPath(), "*.zconf" );
    if(fnme.isEmpty()) { return; }

    QFile file(fnme);
    QDataStream inp(&file);

    clearHdl();

    int len;
    if(file.open(QIODevice::ReadOnly)) {
        inp >> len;
        for(int i=0; i<len; i++) {
            QString type, path, plane_text;
            QColor color;
            inp >> type >> color >> path >> plane_text;
            addHandler(type, path, color, plane_text);
        }// i
        file.close();
    }// if(file.open(QIODevice::ReadWrite))

}// on_btLoad_clicked

// Добавить сервер. ------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::addHandler(QString type, QString path, QColor color, QString text) {
    WHandler *handler = new WHandler(type, path, color, text, this);

    connect(handler, &WHandler::remove, this, &FMain::remove_handler);
    hdl.push_back(handler);

    ui->lyHdl->addWidget(handler);
}// addHandler

// Очистить список серверов. ---------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aClearHdl_triggered() { clearHdl(); }
void FMain::clearHdl(void) {
    for(WHandler *handler : hdl){ delete handler;  }
    hdl.clear();
}// clearHdl

// Поверх всех окон. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btUp_clicked() {
   if(windowFlags() & Qt::WindowStaysOnTopHint) { ui->btUp->setText("Наверх"); }
   else                                         { ui->btUp->setText("Вниз"); }

   this->setWindowFlags(windowFlags() ^ Qt::WindowStaysOnTopHint);
   this->show();
}// on_btUp_clicked

//------------------------------------------------------------------------------


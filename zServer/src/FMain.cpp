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

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "xml.h"
#include "tme.h"

#include "ui_FMain.h"
#include "FMain.h"

// Дополнительные функции. ****************************************************/
/******************************************************************************/


/* FMain **********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);
    this->setWindowFlags(
        Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );
    this->setWindowIcon(QIcon(":/img/ico.ico"));

    // Инициализация.
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    tcp = new QTcpServer(this);
    connect(tcp, &QTcpServer::newConnection, this, &FMain::new_connect);
    E::port = E::PORT; ui->edPort->setText(QString::number(E::port));

    // Меню серверов.
    QMenu *mServer = new QMenu(this);
    mServer->addAction(ui->aClearHdl);
    ui->btMenuHdl->setMenu(mServer);

    // Меню (файл).
    QMenu *mFile = new QMenu(this);
    mFile->addAction(ui->aConfLoad);
    mFile->addAction(ui->aConfSave);
    mFile->addSeparator(); //----------------
    mFile->addAction(ui->aExit);
    ui->btFile->setMenu(mFile);

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
            spc();
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

/*
    spc();
    log(tr("Новое подключение: ")
       + QString::number(reinterpret_cast<ulong>(socket))
       + " (" + socket->peerAddress().toString() + ")", Qt::lightGray);
*/
    connect(socket, &QTcpSocket::readyRead, this, &FMain::read_socket);
    connect(socket, &QTcpSocket::disconnected, this, &FMain::close_socket);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error_socket(QAbstractSocket::SocketError)) );

//    sct.push_back(socket);
}// new_connect

// Чтение сокета. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::read_socket(void) {
//    static QByteArray OU(" <<< "), TO(" >>> "), OK("<ok> DEFAULT </ok>");
    static QByteArray OU(R"( / )"), TO(R"( \ )"), OK("<ok> DEFAULT </ok>");

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

//    QDataStream out(socket);
/**/
    QTextStream out(socket); out.setAutoDetectUnicode(true);
/**/

    QString read = QString::fromUtf8(socket->readAll());
    int pos = read.indexOf('/');

    QString type = read.left(pos).trimmed();
    QString addr = read.mid(pos, read.indexOf("HTTP/") - pos).trimmed();
    QString path = addr.left(addr.indexOf('?')).mid(1).trimmed();

    auto SND = [&](const QString &log, const QByteArray &ans, const QColor &clr)
    -> void {
/*
        this->log(TO + type + ", path:" + path, clr);
        this->log(OU + log, clr);
        out << ans; socket->waitForBytesWritten(); socket->disconnectFromHost();
*/

/*
QString("<div style='margin-bottom:5px'>")
      + "  <span>" + QString::number(cnt+0) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+0) + msg + "</span>"
      + "</div>"

*/


/**/
        QString tme = "[" + QTime::currentTime().toString() + "]";
        QString msg =
            "<div style='margin-bottom:0px; color:" + clr.name() + "'>"
            "  <span>" + tme + OU + log + "</span>" + "<br>"
            "  <span>" + tme + TO + type + ", path:" + path + "</span>" + "<br>"
            "</div>";
        this->log_html(msg);
        out << ans; socket->close();
/**/

    };// SEND

    for(WHandler *handler: hdl) {
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
/*
    QTcpSocket *socket = (qobject_cast<QTcpSocket*>(sender()));

    log(tr("Сокет закрыт.")
      + QString::number(reinterpret_cast<ulong>(socket))
      + " (" + socket->peerAddress().toString() + ")", Qt::lightGray);
*/
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
void FMain::log(const QString &msg, const QColor &clr)
    { this->log(msg, "color:" + clr.name() + ";"); }
void FMain::log(const QString &msg, const QString &stl) {
/*
    if(!ui) { return; }

    ui->tbLog->cursorForPosition(QPoint(0, 0)).insertHtml(
        "<h style='" + stl + "'>"
      + "[" + QTime::currentTime().toString() + "] "
      + ESCPG(msg) + "</h>" + "<br>" );
*/
    this->log_html(
        "<h style='" + stl + "'>"
        "[" + QTime::currentTime().toString() + "] " + ESCPG(msg)
      + "</h>" + "<br>" );

}// FMain::log
void FMain::log_html(const QString &html)
    { if(ui){ ui->tbLog->cursorForPosition(QPoint(0, 0)).insertHtml(html); }}

// Разделитель. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::spc(void) {
    if(!ui) { return; }
    ui->tbLog->cursorForPosition(QPoint(0, 0)).insertHtml("<br>");
}// spc

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btClearLog_clicked() { ui->tbLog->clear(); }

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

// Сохранить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aConfSave_triggered() {
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
}// on_aConfSave_triggered

// Загрузить конфигурацию. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aConfLoad_triggered() {
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
}// on_aConfLoad_triggered

// Завершение работы. ----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    {
        QApplication::quit();
    }// if(QMessageBox::Yes == QMessageBox::question ...
}// FMain::on_aExit_triggered()

// Отладка. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btDebug_clicked() {
    const QString msg = ": НЕКОЕ ДОСТАТОЧНО ДЛИННОЕ СООБЩЕНИЕ";
    static int cnt = 0; cnt++;

/**/
    QTextBrowser *browser = ui->tbLog;
    QString text;

/*
    text.append("<html><body>");
    text.append(
        "<style type='text/css'>"
//        "div { margin-bottom:" + QString::number(cnt) + "px; }"
        "hr { "
        "margin-bottom:" + QString::number(cnt) + "px; "
        "margin-top:" + QString::number(cnt) + "px; "
        "size:" + QString::number(cnt) + "px; "
        "}"

        "</style>" );
*/

    text.append(
QString("<div style='margin-bottom:0px'>")
      + "  <span>" + QString::number(cnt+0) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+0) + msg + "</span>"
      + "</div>"
    );

//    text.append("<div style='border-top: 1px solid black; width: 100%; margin: 0px;'>&nbsp;</div>");
//    text.append(R"(<hr size="1" style='margin-top:1; margin-bottom:1'>)");
//    text.append("<hr>");

    text.append(
QString("<div style='margin-bottom:0px'>")
      + "  <span>" + QString::number(cnt+1) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+1) + msg + "</span>"
      + "</div>"
    );

//    text.append("<div style='border-top: 1px solid black; width: 100%; margin: 0px;'>&nbsp;</div>");
//    text.append(R"(<hr size="1" style='margin-top:1; margin-bottom:1'>)");
//    text.append("<hr>");

    text.append(
QString("<div style='margin-bottom:0px'>")
      + "  <span>" + QString::number(cnt+2) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+2) + msg + "</span>"
      + "</div>"
    );

    text.append("</body></html>");
    browser->setHtml(text);
/**/


// style='padding-top:10px;'

/*
    if(FIRST_RUN) {
        ui->tbLog->append(
            "<style type='text/css'>p { margin-bottom:0px; }</style>"
        );
    }

    ui->tbLog->append(
QString("<p>") //+ QString::number(cnt) + msg
      + "  <span>" + QString::number(cnt) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt) + msg + "</span>" + "<br>"
      + "</p>"
    );
*/

}

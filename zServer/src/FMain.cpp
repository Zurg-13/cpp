// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
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
#include <QTimer>
#include <QStringBuilder>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "xml.h"
#include "tme.h"

#include "FMain.h"
#include "ui_FMain.h"

// Дополнительные функции. ****************************************************/
/******************************************************************************/


/* FMain **********************************************************************/
/******************************************************************************/

// Обработка запроса. ----------------------------------------------------------
//------------------------------------------------------------------------------
QHttpServerResponse FMain::proc(
    const QString &path, const QHttpServerRequest &req )
{
    static int cnt = 0; cnt++;
    static QString  DFT("<ok> DEFAULT: %1 </ok>");

    qDebug() << "path:" << req.url().path();

    QString type = QVariant::fromValue(req.method()).toString();
    QString body = req.body(); // req вызывать ДО входа в ожидание.

    for(WHandler *handler: this->hdl) {

        if(path == handler->path) {
            WLogEntry *entry = ui->wgLog->grab();
                entry->inp(type % ", path:" % path % ", cont:" % body);
                entry->clr(handler->color);
            QHttpServerResponse rsp(handler->answer()); // WAIT
                entry->out(handler->plane_text());
            ui->wgLog->free(entry);
            return rsp;
        }// if(path == handler->path)

    }// handler

    ui->wgLog->post(
        type % ", path:" % path + ", body:" % body
      , DFT.arg(cnt), Qt::darkYellow );
    return QHttpServerResponse(DFT.arg(cnt));
}// proc

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
    E::port = E::PORT; ui->edPort->setText(QString::number(E::port));

    this->tcp = new QTcpServer(this);
    srv.bind(this->tcp);
    srv.route("/<arg>",
    [this](
        const QUrl &url,
        const QHttpServerRequest &req, QHttpServerResponder &&rpdr )
    {
        QTimer::singleShot(
            0, [this, rpdr = std::move(rpdr), path = url.path(), &req = req]()
        mutable {
            QHttpServerResponse rsp = this->proc(path, req);
            if(rpdr.socket() && rpdr.socket()->isValid())
                { rsp.write(std::move(rpdr)); }
        });// singleShot
    });// route

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

// Добавить обработчик. --------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btAdd_clicked() {
    addHandler(
        "TEXT", EMPTY_STR, this->palette().color(QPalette::Window), EMPTY_STR );
}// on_btAdd_clicked

// Удалить обработчик. ---------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::remove_handler(WHandler *handler) {

    for(QList<WHandler*>::iterator it = hdl.begin(); it != hdl.end(); it++) {
        if((*it) == handler) { hdl.erase(it); handler->deleteLater(); return; }
    }// it

}// on_btAdd_clicked

// Пуск/Стоп. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btStart_clicked() {
    auto SET_BTN = [&](const QString &ttl, const QString &sht) -> void {
        ui->btStart->setText(ttl); ui->btStart->setStyleSheet(sht); };

    if(tcp->isListening()) {
        tcp->close();
        for(WHandler *handler: this->hdl)
            { handler->interrupt(); }
        for(QTcpSocket *socket: this->tcp->findChildren<QTcpSocket*>())
            { socket->disconnectFromHost(); }

        post(tr("Прослушивание остановлено"), Qt::lightGray);
        SET_BTN("Пуск", "color: limegreen;");

    } else {
        bool ok;
        unsigned short port = ui->edPort->text().toUShort(&ok);
        if(!ok)
            { post(tr("Введите корректный порт"), Qt::red); return; }

        if(tcp->listen(QHostAddress::Any, port)) {
            rift();
            post(tr("Прослушивание запущено"), Qt::lightGray);
            SET_BTN("Стоп", "color: crimson;");
        } else {
            post(tr("Ошибка прослушивателя: ") + tcp->errorString(), Qt::red);
        }// else // if(tcp->listen(QHostAddress::Any, port))
    }// else // if(tcp->isListening())

}// on_btStart_clicked

// Опубликовать запись в логе. -------------------------------------------------
//------------------------------------------------------------------------------
void FMain::post(const QString &msg, const QColor &clr)
    { ui->wgLog->post(msg, clr); }
void FMain::post(const QString &inp, const QString &out, const QColor &clr)
    { ui->wgLog->post(new WLogEntry(inp, out))->clr(clr); }

// Захватить записб в логе. ----------------------------------------------------
//------------------------------------------------------------------------------
WLogEntry* FMain::grab(void) { return ui->wgLog->grab(); }

// Освободить запись в логе. ---------------------------------------------------
//------------------------------------------------------------------------------
void FMain::free(WLogEntry *entry) { ui->wgLog->free(entry); }

// Вставить промежуток в лог. --------------------------------------------------
//------------------------------------------------------------------------------
void FMain::rift(void) { ui->wgLog->rift(); }

// Очистить лог. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btClearLog_clicked() { ui->wgLog->clear(); }

// Добавить обработчик. --------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::addHandler(QString type, QString path, QColor color, QString text) {
    WHandler *handler = new WHandler(type, path, color, text, this);

    connect(handler, &WHandler::remove, this, &FMain::remove_handler);
    hdl.push_back(handler);

    ui->lyHdl->addWidget(handler);
}// addHandler

// Очистить список обработчиков. -----------------------------------------------
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
      , QMessageBox::Yes | QMessageBox::No )) { QApplication::quit(); }
}// FMain::on_aExit_triggered()

// Отладка. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btDebug_clicked() {
    FNC << R"(/ bgn)";

/*
    static int cnt(0);
    const QString msg = ": НЕКОЕ ДОСТАТОЧНО ДЛИННОЕ СООБЩЕНИЕ :";
    ui->wgLog->post("INP" + msg + STR(cnt++) , "OUT" + msg);
    ui->wgLog->show();
*/

/*
    for(const QTcpSocket *socket: this->tcp->findChildren<QTcpSocket*>()) {
        FNC << "| obj:" << socket->metaObject()->className();
    }// socket
*/

    FNC << R"(\ end)";
}// on_btDebug_clicked

//------------------------------------------------------------------------------

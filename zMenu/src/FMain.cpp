// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>


#include <QListWidget>
#include <QListWidgetItem>
#include <QStringBuilder>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "xml.h"
#include "tme.h"
#include "lst.h"

#include "Item.h"
#include "FMain.h"
#include "ui_FMain.h"

// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define ERR(m) E::err(STR("MAIN: ") % m)
#define IMP(m) E::imp(STR("MAIN: ") % m)
#define OTH(m) E::oth(STR("MAIN: ") % m)

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
    QString type = QVariant::fromValue(req.method()).toString();
    QString body = req.body(); // req вызывать ДО входа в ожидание.

    E::Log->post(type % ", path:" % path + ", body: [++]", body);

    FNC << req.query().queryItems();

    // Вернуть страницу.
    if(path == "page") {
        QHttpServerResponse rsp(FLE(FSN(this->path, "page.html")).toUtf8());
            rsp.addHeader("Content-Type", "text/html; charset=utf-8");
        return rsp;

    // Вернуть список элементов.
    } else if(path == "list") {
/*
        QStringList itm;
        for(int i=0; i< ui->lwMenu->count(); i++) {
            itm.append(
                static_cast<Item*>(
                    ui->lwMenu->itemWidget(ui->lwMenu->item(i)))->jsn() );
        }// i

        QHttpServerResponse rsp(QString("[" % BLD(itm) % "]").toUtf8());
            rsp.addHeader("Content-Type", "application/json; charset=utf-8");
        return rsp;
*/

    // Задать состояние элемента.
    } else if(path == "item") {
        QMap<QString, QString> prm(MAP(req.query().queryItems()));

    }// else if(path == "item")

    return QHttpServerResponse(DFT.arg(cnt));
}// proc

// Связывание обработчиков. ----------------------------------------------------
//------------------------------------------------------------------------------
typedef QHttpServerRequest QHttpReq;
typedef QHttpServerResponse QHttpRsp;
void FMain::ROUTING(void) {

    // Файлы.
    this->srv.route("/file/<arg>", [this](const QUrl &url) {
        OTH("file: " % url.path());
        return QHttpRsp::fromFile(FSN(this->path, url.path()));
    });

    // Конфиг.
    this->srv.route("/conf/<arg>", [this](const QUrl &url) {
        OTH("conf: " % url.path());
        return QJsonObject {{
            { "wsport", this->wss->serverPort() }
          , { "wsaddr", this->wss->serverAddress().asSTR }
        }};
    });

}// ROUTING

// Получено текстовое сообщение. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_txt_msg(QString msg) {
    FNC << "txt:" << msg;

    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    QJsonObject json = QJsonValue(msg).toObject();


}// on_ws_txt_msg

// Получено двоичное сообщение. ------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_bin_msg(QByteArray msg) {
    FNC << "bin:" << msg;
}// on_ws_bin_msg

// Ws: потеря соединения. ------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_disconnect() {
    FNC << "bgn";

    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if(socket) { socket->deleteLater(); }
}// on_ws_disconnect

// Ws: новое подключение. ------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_connect() {
    QWebSocket *socket = this->wss->nextPendingConnection();

    connect(
        socket, &QWebSocket::textMessageReceived
      , this, &FMain::on_ws_txt_msg );
    connect(
        socket, &QWebSocket::binaryMessageReceived
      , this, &FMain::on_ws_bin_msg );
    connect(
        socket, &QWebSocket::disconnected
      , this, &FMain::on_ws_disconnect );

}// on_ws_connect

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete wss;
    delete ui;
}//~FMain

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringLiteral>
FMain::FMain(QWidget *prnt)
:   QMainWindow(prnt), ui(new Ui::FMain)
  , wss(new QWebSocketServer("Menu wss", QWebSocketServer::NonSecureMode, this))

{

    // Внешний вид.
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/ico.ico"));
    QTimer::singleShot(0, [this] { E::Log->place(this); E::Log->show(); });

    // Инициализация.
    SET_PRM(QCoreApplication::arguments());
    SET_SQL();

    // Сервер.
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    this->tcp = new QTcpServer(this);
    this->srv.bind(this->tcp); ROUTING();
    connect(
        this->wss, &QWebSocketServer::newConnection
      , this, &FMain::on_ws_connect );


    // Пункты меню.
/*
    auto LW_ADD = [this](QWidget *wgt){
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(wgt->sizeHint());
        this->ui->lwMenu->addItem(item);
        this->ui->lwMenu->setItemWidget(item, wgt);
    };// LW_ADD

    LW_ADD(new Item("Раз", "Раз: подробное описание", 20));
    LW_ADD(new Item("Два", "Два: подробное описание", 35));
    LW_ADD(new Item("Три", "Три: подробное описание", 50));
    LW_ADD(new Item("Четыре", "Один: подробное описание", 65));
    LW_ADD(new Item("Пять", "Пять: подробное описание", 80));
    LW_ADD(new Item("Шесть", "Шесть: подробное описание", 95));
    LW_ADD(new Item("Семь", "Семь: подробное описание", 110));
    LW_ADD(new Item("Восемь", "Восемь: подробное описание", 125));
    LW_ADD(new Item("Девять", "Девять: подробное описание", 140));
    LW_ADD(new Item("Десять", "Десять: подробное описание", 155));
*/

}// FMain

// Обработать аргументы командной строки. --------------------------------------
//------------------------------------------------------------------------------
#include <QCommandLineParser>
void FMain::SET_PRM(const QStringList &args) {
    QCommandLineParser prsr;

    auto HVE = [&prsr](QString nme) -> bool { return prsr.isSet(nme); };
    auto VAL = [&prsr](QString nme)
        -> QString { return prsr.value(nme).trimmed(); };

    prsr.setApplicationDescription("Онлайн меню.");
    prsr.addVersionOption();
    prsr.addHelpOption();
    prsr.addOptions({
        {{"q", "quit"}, tr("Завершить приложение."), "quit" }
      , {{"f", "file"}, tr("Задать путь к файловому хранилищу."), "file" }
      , {{"l", "sqlt"}, tr("Задать путь к файловому хранилищу."), "file" }
    });
    prsr.process(args);

    // quit.
    if(HVE("quit")) { QTimer::singleShot(0, qApp, SLOT(quit())); }

    // path
    if(HVE("path")) { this->path = VAL("path"); }
    else            { this->path = FSN(APP_DIR, "web"); }

}// SET_PRM

// Активация БД. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::SET_SQL(void) {
    if(NOT(E::sldb->open())) { ERR(E::sldb->lastError().text()); }

    static const QString ALREDY_EXIST("2");
    static const QSqlError::ErrorType
        OK(QSqlError::NoError), STMT(QSqlError::StatementError);

    QStringList tbl {
        "CREATE TABLE item ("
        "  id   INTEGER PRIMARY KEY NOT NULL"
        ", name VARCHAR(255)        NOT NULL"
        ", note VARCHAR(255)        NOT NULL"
        ", cost INTEGER             NOT NULL"
        ", room INTEGER             NOT NULL"
        ", type INTEGER             NOT NULL )"

      , "CREATE TABLE type ("
        "  id   INTEGER PRIMARY KEY NOT NULL"
        ", name VARCHAR(255)        NOT NULL"
        ", note VARCHAR(255)        NOT NULL )"

      , "CREATE TABLE room ("
        "  id   INTEGER PRIMARY KEY NOT NULL"
        ", name VARCHAR(255)        NOT NULL"
        ", note VARCHAR(255)        NOT NULL )"
    };

    for(const QString &query: tbl) {
        QSqlError error = E::sldb->exec(query).lastError();
        QSqlError::ErrorType etype = error.type();

        if(etype != OK
        &&(etype != STMT || error.nativeErrorCode() != ALREDY_EXIST) )
            { ERR(error.text()); }
    }// query

}// SET_SQL

/*
// Страница. -------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QDesktopServices>
void FMain::on_btTest_clicked() {
    QString URL("http://{ADR}:{PRT}/{PTH}");

    const QHostAddress &local = QHostAddress(QHostAddress::LocalHost);
    for(const QHostAddress &addr: QNetworkInterface::allAddresses()) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr != local) {
            QDesktopServices::openUrl(QUrl(URL
                .replace("{ADR}", addr.toString())
                .replace("{PRT}", QString::number(E::port))
                .replace("{PTH}", "page") ));
            break;
        }//if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr != local)
    }// addr
}// on_btTest_clicked
*/

// Http: Пуск / Стоп. ----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btRunHttp_clicked() {
    auto SET_BTN = [&](const QString &ttl, const QString &sht) -> void {
        ui->btRunHttp->setText(ttl); ui->btRunHttp->setStyleSheet(sht); };

    if(this->tcp->isListening()) {
        this->tcp->close();
        for(QTcpSocket *socket: this->tcp->findChildren<QTcpSocket*>())
            { socket->disconnectFromHost(); }
        E::Log->post("Http: Прослушивание остановлено")->clr(Qt::lightGray);
        SET_BTN("Http: Пуск", "color: limegreen;");

    } else {
        if(this->tcp->listen(QHostAddress::Any, E::port)) {
            E::Log->rift();
            E::Log->post("Http: Прослушивание запущено")->clr(Qt::lightGray);
            SET_BTN("Http: Стоп", "color: crimson;");
        } else {
            E::Log->post("Http: Ошибка прослушивателя: " + tcp->errorString())
                ->clr(Qt::red);
        }// else // if(tcp->listen(QHostAddress::Any, port))
    }// else // if(tcp->isListening())

}// on_btRunHttp_clicked

// Ws: Пуск / Стоп. ------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btRunWs_clicked() {
    auto SET_BTN = [&](const QString &ttl, const QString &sht) -> void {
        ui->btRunWs->setText(ttl); ui->btRunWs->setStyleSheet(sht); };

    if(this->wss->isListening()) {
        this->wss->close();
        for(QWebSocket *socket: this->wss->findChildren<QWebSocket*>())
            { socket->disconnect(); }

        E::Log->post("Ws: Прослушивание остановлено")->clr(Qt::lightGray);
        SET_BTN("Ws: Пуск", "color: limegreen;");

    } else {
        if(this->wss->listen(QHostAddress::Any)) {
            E::Log->rift();
            E::Log->post("Ws: Прослушивание запущено")->clr(Qt::lightGray);
            SET_BTN("Ws: Стоп", "color: crimson;");
        } else {
            E::Log->post("Ws: Ошибка прослушивателя: " + tcp->errorString())
                ->clr(Qt::red);
        }// else // if(tcp->listen(QHostAddress::Any, port))
    }// else // if(tcp->isListening())
}// on_btRunWs_clicked

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMessageBox>
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No )) { QApplication::quit(); }
}// on_aExit_triggered

// Отладка -> Лог. -------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aLog_triggered() { E::Log->place(this); E::Log->show(); }

//------------------------------------------------------------------------------

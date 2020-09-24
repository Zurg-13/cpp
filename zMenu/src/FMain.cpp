// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>


#include <QListWidget>
#include <QListWidgetItem>
#include <QStringBuilder>
#include <QList>
#include <QPair>

#include "env.h" // Глобальная среда приложения.

#include "std.h"
#include "dbg.h"
#include "xml.h"
#include "tme.h"
#include "lst.h"
#include "sql.h"

#include "FMain.h"
#include "ui_FMain.h"

// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define ERR(m) E::err(STR("MAIN: ") % m)
#define IMP(m) E::imp(STR("MAIN: ") % m)
#define OTH(m) E::oth(STR("MAIN: ") % m)

#define ARR(j) j.toJson()

// Дополнительные функции. ****************************************************/
/******************************************************************************/


// Преобразование в JSON-объект. -----------------------------------------------
//------------------------------------------------------------------------------
QJsonDocument JSON(std::initializer_list<QPair<QString, QJsonValue>> list)
    { return QJsonDocument(QJsonObject(list)); }
QJsonArray JSON(QSqlQuery &query) {
    QJsonArray arr;

    while(query.next()) {
        QJsonObject rec;
        for(int i=0; i<query.record().count(); i++) {
            rec.insert(
                query.record().fieldName(i)
              , QJsonValue::fromVariant(query.value(i)) );
        }// i
        arr.push_back(rec);
    }// while(query.next())

    return arr;
}// JSON

// Приведение к виду ключ-значене. ---------------------------------------------
//------------------------------------------------------------------------------
ZPrm PRM(const QJsonValue &json) {
    ZPrm prmt;
    QJsonObject::const_iterator etty = json.toObject().constBegin();
    for(; etty != json.toObject().constEnd(); etty++) {
        prmt[':' + etty.key()] = etty->isDouble() ? QVariant(etty->toDouble())
                                                  : QVariant(etty->toString());
    }// etty
    return prmt;
}// prm

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

    auto RSP = [](const QJsonObject &json) -> QHttpRsp {
        QHttpRsp rsp(json);
            rsp.addHeader("Access-Control-Allow-Origin", "*");
            rsp.addHeader("Access-Control-Allow-Method", "GET");
        return rsp;
    };// RSP

    // Файлы.
    this->srv.route("/file/<arg>", [this](const QUrl &url) {
        OTH("file: " % url.path());
        return QHttpRsp::fromFile(FSN(this->path, url.path()));
    });

    // Конфиг.
    this->srv.route("/conf/<arg>", [this, RSP](const QUrl &url) {
        OTH("conf: " % url.path());
        return RSP(QJsonObject {{
            { "host", addr().asSTR }
          , { "port", STR(E::port) }
        }});
    });

    // По умолчанию.
    this->srv.route("/<arg>", [RSP](const QUrl &url) {
        OTH("dflt: " % url.path());
        return RSP(QJsonObject {{
            { "appname", QApplication::applicationName() }
          , { "version", QApplication::applicationVersion() }
          , { "sysdate", SYSDATE.asSTR }
        }});
    });

}// ROUTING

// Получено текстовое сообщение. -----------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_txt_msg(QString msg) {
    static QMap<QString, int> route = {
        {"item_list",1}, {"item_save",2}, {"item_post",3}
      , {"type_list",4}, {"type_save",5}, {"type_post",6}
      , {"room_list",7}, {"room_save",8}, {"room_post",9}
    };

    QWebSocket *rsp = qobject_cast<QWebSocket*>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(msg.asUTF);
    QJsonObject obj = doc.isNull()
        ? QJsonObject()
        : doc.isObject() ? doc.object() : QJsonObject();

    switch(route[obj["cmnd"].asSTR]) {
     case  1: item_list(rsp, obj); break;
     case  2: item_save(rsp, obj); break;
     default:
        rsp->sendTextMessage(ARR(JSON({
            PR("cmnd", obj["cmnd"])
          , PR("err", "Команда не обрабатывается")
        })));
    }// switch(route[obj["cmnd"].asSTR])

    E::Log->post("Текстовое сообщение", msg);
}// on_ws_txt_msg

// Выполнение запросов. --------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::EXEC(
    QWebSocket *rsp, const QJsonObject &obj, QSqlQuery &sql
  , std::function<void(QSqlQuery&)> done )
{
    if(sql.exec()) {
        done(sql);
    } else {
        E::Log->post(
            "Ошибка выполнения запроса: " % sql.lastError().asTXT
          , sql.executedQuery() )->clr(Qt::darkYellow);
        rsp->sendTextMessage(ARR(JSON({
            PR("id", (obj["data"])["id"].asINT)
          , PR("cmnd", obj["cmnd"])
          , PR("err", sql.lastError().asTXT) })));
    }// else // if(sql.lastError().type() == QSqlError::NoError)
}// EXEC

// ОБРАБОТЧИК: Создать новый элемент меню. -------------------------------------
//------------------------------------------------------------------------------
void FMain::item_post(QWebSocket *rsp, const QJsonObject &obj) {
    static ZSqlQuery iItem = ZSqlQuery(
        "\n INSERT INTO item "
                  "(name, note, cost, stat, hide, type, room)"
        "\n VALUES(:name,:note,:cost,:stat,:hide,:type,:room)", *E::sldb );
    static ZSqlQuery sItemID = ZSqlQuery(
        "\n SELECT seq FROM sqlite_sequence WHERE name='item'", *E::sldb );

    EXEC(
        rsp, obj, iItem(PRM(obj["data"]))
      ,[rsp, obj](QSqlQuery&) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("item",sItemID.exe().fst()["seq"].asINT) }))); });
}// item_post

// ОБРАБОТЧИК: Вернуть список элементов меню. ----------------------------------
//------------------------------------------------------------------------------
void FMain::item_list(QWebSocket *rsp, const QJsonObject &obj) {
    static QSqlQuery sItem = ZSqlQuery(
        "SELECT * FROM item", *E::sldb);
    EXEC(
        rsp, obj, sItem
      ,[rsp, obj](QSqlQuery &sql) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("list", JSON(sql)) }))); });
}// item_list

// ОБРАБОТЧИК: Сохранить элемент меню. -----------------------------------------
//------------------------------------------------------------------------------
void FMain::item_save(QWebSocket *rsp, const QJsonObject &obj) {
    static ZSqlQuery uItem = ZSqlQuery(
        "\n UPDATE item"
        "\n SET name = :name, note = :note, cost = :cost"
        "\n   , type = :type, room = :room"
        "\n WHERE id = :id", *E::sldb );
    EXEC(
        rsp, obj, uItem(PRM(obj["data"]))
      ,[rsp, obj](QSqlQuery&) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("item",(obj["data"])["id"].asINT) }))); });
}// item_save

// ОБРАБОТЧИК: Вернуть список типов. -------------------------------------------
//------------------------------------------------------------------------------
void FMain::type_list(QWebSocket *rsp, const QJsonObject &obj) {
    static QSqlQuery sType = ZSqlQuery(
        "SELECT * FROM type", *E::sldb);
    EXEC(
        rsp, obj, sType
      ,[rsp, obj](QSqlQuery &sql) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("list", JSON(sql)) }))); });
}// type_list

// ОБРАБОТЧИК: Сохраить тип. ---------------------------------------------------
//------------------------------------------------------------------------------
void FMain::type_save(QWebSocket *rsp, const QJsonObject &obj) {
    static ZSqlQuery uType = ZSqlQuery(
        "\n UPDATE type"
        "\n SET name = :name, note = :note"
        "\n WHERE id = :id", *E::sldb );
    EXEC(
        rsp, obj, uType(PRM(obj["data"]))
      ,[rsp, obj](QSqlQuery&) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("type",(obj["data"])["id"].asINT) }))); });
}// type_save

// ОБРАБОТЧИК: Вернуть список размещений. --------------------------------------
//------------------------------------------------------------------------------
void FMain::room_list(QWebSocket *rsp, const QJsonObject &obj) {
    static QSqlQuery sRoom = ZSqlQuery(
        "SELECT * FROM room", *E::sldb);
    EXEC(
        rsp, obj, sRoom
      ,[rsp, obj](QSqlQuery &sql) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("list", JSON(sql))
            })));
        });
}// room_list

// ОБРАБОТЧИК: Сохранить размещение. -------------------------------------------
//------------------------------------------------------------------------------
void FMain::room_save(QWebSocket *rsp, const QJsonObject &obj) {
    static ZSqlQuery uRoom = ZSqlQuery(
        "\n UPDATE room"
        "\n SET name = :name, note = :note"
        "\n WHERE id = :id", *E::sldb );
    EXEC(
        rsp, obj, uRoom(PRM(obj["data"]))
      ,[rsp, obj](QSqlQuery&) {
            rsp->sendTextMessage(ARR(JSON({
                PR("cmnd", obj["cmnd"])
              , PR("room",(obj["data"])["id"].asINT) }))); });
}// room_save

// Получено двоичное сообщение. ------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_bin_msg(QByteArray msg) {
    E::Log->post("Двоичное сообщение", msg);
}// on_ws_bin_msg

// Ws: потеря соединения. ------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_disconnect() {
    OTH("Ws: Потеря соединения");

    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if(this->last == socket) { this->last = nullptr; }
    if(socket) { socket->deleteLater(); }
}// on_ws_disconnect

// Ws: новое подключение. ------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_ws_connect() {
    OTH("Ws: Новое подключение");
    QWebSocket *socket = this->srv.nextPendingWebSocketConnection();

    connect(
        socket, &QWebSocket::textMessageReceived
      , this, &FMain::on_ws_txt_msg );
    connect(
        socket, &QWebSocket::binaryMessageReceived
      , this, &FMain::on_ws_bin_msg );
    connect(
        socket, &QWebSocket::disconnected
      , this, &FMain::on_ws_disconnect );

    this->last = socket;
}// on_ws_connect

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}//~FMain

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringLiteral>
FMain::FMain(QWidget *prnt) : QMainWindow(prnt), ui(new Ui::FMain) {

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
       &this->srv, &QHttpServer::newWebSocketConnection
      , this, &FMain::on_ws_connect );

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
      , {{"l", "sqlt"}, tr("Задать путь к хранилищу БД."), "sqlt" }
    });
    prsr.process(args);

    // quit.
    if(HVE("quit")) { QTimer::singleShot(0, qApp, SLOT(quit())); }

    // file
    if(HVE("file")) { this->path = VAL("file"); }
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
        "  id   INTEGER PRIMARY KEY AUTOINCREMENT"
        ", name TEXT    NOT NULL"
        ", note TEXT    NOT NULL"
        ", cost INTEGER NOT NULL"
        ", stat INTEGER NOT NULL"
        ", hide INTEGER NOT NULL"
        ", room INTEGER NOT NULL"
        ", type INTEGER NOT NULL )"

      , "CREATE TABLE type ("
        "  id   INTEGER PRIMARY KEY AUTOINCREMENT"
        ", name TEXT    NOT NULL"
        ", note TEXT    NOT NULL )"

      , "CREATE TABLE room ("
        "  id   INTEGER PRIMARY KEY AUTOINCREMENT"
        ", name TEXT    NOT NULL"
        ", note TEXT    NOT NULL )"
    };

    for(const QString &query: tbl) {
        QSqlError error = E::sldb->exec(query).lastError();
        QSqlError::ErrorType etype = error.type();

        if(etype != OK
        &&(etype != STMT || error.nativeErrorCode() != ALREDY_EXIST) )
            { ERR(error.text()); }
    }// query

}// SET_SQL

// Перемещение окна. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::moveEvent(QMoveEvent */*evt*/) { E::Log->place(this); }

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

// Локальный IP-адрес. ---------------------------------------------------------
//------------------------------------------------------------------------------
QHostAddress FMain::addr(void) {
    const QHostAddress &local = QHostAddress(QHostAddress::LocalHost);
    for(const QHostAddress &addr: QNetworkInterface::allAddresses()) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr != local)
            { return addr; }
    }// addr
    return QHostAddress(QHostAddress::LocalHost);
}// addr

// Пуск / Стоп. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btRun_clicked() {
    auto SET_BTN = [&](const QString &ttl, const QString &sht) -> void {
        ui->btRun->setText(ttl); ui->btRun->setStyleSheet(sht); };

    if(this->tcp->isListening()) {
        this->tcp->close();
        for(QTcpSocket *socket: this->tcp->findChildren<QTcpSocket*>())
            { socket->disconnectFromHost(); }
        IMP("Прослушивание остановлено");
        SET_BTN("Пуск", "color: limegreen;");

    } else {
        if(this->tcp->listen(QHostAddress::Any, E::port)) {
            IMP("Прослушивание запущено");
            SET_BTN("Стоп", "color: crimson;");
        } else {
            ERR("Ошибка прослушивателя: " + tcp->errorString());
        }// else // if(tcp->listen(QHostAddress::Any, port))
    }// else // if(tcp->isListening())

}// on_btRunHttp_clicked

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

// Ws: Отправить. --------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QInputDialog>
void FMain::on_aWsSend_triggered() {
    static QString prev;

    if(this->last) {
        prev = QInputDialog::getMultiLineText(this, "Ws: Передать", "", prev);
        this->last->sendTextMessage(prev);
    } else {
        ERR("Нет подключений");
    }// else // if(sock)

}// on_aWsSend_triggered

// Отладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    QString msg(R"({"cmnd" : "list"})");

    QJsonDocument doc = QJsonDocument::fromJson(msg.asUTF);
    QJsonObject obj;

    if(doc.isNull()) {
        FNC << "json err";
    } else if(doc.isObject()) {
        FNC << "json ok";
        obj = doc.object();
    } else {
        FNC << "json not obj";
    }// else // if(doc.isNull())

    FNC << "method:" << obj["method"];
}// on_aTest_triggered

// Тестовое заполнение БД. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTestFillDB_triggered() {
    static const QString ALREDY_EXIST("2");
    static const QSqlError::ErrorType
        OK(QSqlError::NoError), STMT(QSqlError::StatementError);

    QStringList tbl {
        "INSERT INTO item (name, note, cost, stat, hide, room, type)"
        "VALUES('NAME-1', 'NOTE-1', 0, 0, 0, 0, 0)"
      , "INSERT INTO item (name, note, cost, stat, hide, room, type)"
        "VALUES('NAME-2', 'NOTE-2', 0, 0, 0, 0, 0)"
      , "INSERT INTO item (name, note, cost, stat, hide, room, type)"
        "VALUES('NAME-3', 'NOTE-3', 0, 0, 0, 0, 0)"
      , "INSERT INTO item (name, note, cost, stat, hide, room, type)"
        "VALUES('NAME-4', 'NOTE-4', 0, 0, 0, 0, 0)"

      , "INSERT INTO type (name, note)"
        "VALUES('Нет', 'Тип не задан')"
      , "INSERT INTO type (name, note)"
        "VALUES('Меню', 'Меню')"
      , "INSERT INTO type (name, note)"
        "VALUES('Пицца', 'Пицца')"

      , "INSERT INTO room (name, note)"
        "VALUES('Нет', 'Размещение не задано')"
      , "INSERT INTO room (name, note)"
        "VALUES('Салат', 'Салаты')"
      , "INSERT INTO room (name, note)"
        "VALUES('Первое', 'Первые блюда')"
      , "INSERT INTO room (name, note)"
        "VALUES('Второе', 'Вторые блюда')"
      , "INSERT INTO room (name, note)"
        "VALUES('Десерт', 'Десерты')"
    };

    for(const QString &query: tbl) {
        QSqlError error = E::sldb->exec(query).lastError();
        QSqlError::ErrorType etype = error.type();

        if(etype != OK
        &&(etype != STMT || error.nativeErrorCode() != ALREDY_EXIST) )
            { ERR(error.text()); }
    }// query

}// on_aTestFillDB_triggered

//------------------------------------------------------------------------------


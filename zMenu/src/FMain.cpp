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
#define ERR(m) E::err("MAIN " % STR(SYSDATE) % ": " % m)
#define IMP(m) E::imp("MAIN " % STR(SYSDATE) % ": " % m)
#define OTH(m) E::oth("MAIN " % STR(SYSDATE) % ": " % m)

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
        QStringList itm;
        for(int i=0; i< ui->lwMenu->count(); i++) {
            itm.append(
                static_cast<Item*>(
                    ui->lwMenu->itemWidget(ui->lwMenu->item(i)))->jsn() );
        }// i

        QHttpServerResponse rsp(QString("[" % BLD(itm) % "]").toUtf8());
            rsp.addHeader("Content-Type", "application/json; charset=utf-8");
        return rsp;

    // Задать состояние элемента.
    } else if(path == "item") {
        QMap<QString, QString> prm(MAP(req.query().queryItems()));

    }// else if(path == "item")

    return QHttpServerResponse(DFT.arg(cnt));
}// proc

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}//~FMain

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *prnt) : QMainWindow(prnt), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/ico.ico"));

    // Инициализация.
    SET_PRM(QCoreApplication::arguments());
    SET_SQL();
    QNetworkProxyFactory::setUseSystemConfiguration(false);

    // Связывание обработчиков.
    this->tcp = new QTcpServer(this);
    this->srv.bind(this->tcp);

    this->srv.route("/file/<arg>", [this](const QUrl &url) {
        return QHttpServerResponse::fromFile(FSN(this->path, url.path())); });

    this->srv.route("/<arg>",
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
    });

    // Пункты меню.
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
      , {{"p", "path"}, tr("Задать путь к файловому хранилищу."), "path" }
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
    FNC << R"(/ bgn)";

    if(NOT(E::sldb->open())) { ERR(E::sldb->lastError().text()); }

/**/
    QString item(
        "CREATE TABLE item ("
        "  id   INTEGER PRIMARY KEY NOT NULL"
        ", name VARCHAR(255)"
        ", desc VARCHAR(255)"
        ", cost INTEGER )" );

    QSqlQuery query = E::sldb->exec(item);
    FNC << "| err:" << query.lastError().text() // таблица уже существует
        << ", cde:" << query.lastError().type() // 2
        << ", ntv:" << query.lastError().nativeErrorCode(); // 1

/**/

    FNC << R"(\ end)";
}// SET_SQL

// Проба. ----------------------------------------------------------------------
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

// Пуск / Стоп. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btRun_clicked() {
    auto SET_BTN = [&](const QString &ttl, const QString &sht) -> void {
        ui->btRun->setText(ttl); ui->btRun->setStyleSheet(sht); };

    if(tcp->isListening()) {
        tcp->close();
        for(QTcpSocket *socket: this->tcp->findChildren<QTcpSocket*>())
            { socket->disconnectFromHost(); }

        E::Log->post(tr("Прослушивание остановлено"))->clr(Qt::lightGray);
        SET_BTN("Пуск", "color: limegreen;");

    } else {
        if(tcp->listen(QHostAddress::Any, E::port)) {
            E::Log->rift();
            E::Log->post(tr("Прослушивание запущено"))->clr(Qt::lightGray);
            SET_BTN("Стоп", "color: crimson;");
        } else {
            E::Log->post(tr("Ошибка прослушивателя: ") + tcp->errorString())
                ->clr(Qt::red);
        }// else // if(tcp->listen(QHostAddress::Any, port))
    }// else // if(tcp->isListening())

}// on_btRun_clicked

//------------------------------------------------------------------------------


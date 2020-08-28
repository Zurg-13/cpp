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

    // Вернуть список элементов.
    } else if(path == "list") {
        QStringList itm;
        for(int i=0; i< ui->lw->count(); i++) {
            itm.append(
                static_cast<Item*>(ui->lw->itemWidget(ui->lw->item(i)))->jsn());
        }// i

        QHttpServerResponse rsp(QString("[" % BLD(itm) % "]").toUtf8());
            rsp.addHeader("Content-Type", "text/html; charset=utf-8");
        return rsp;
    // Задать состояние элемента.
    } else if(path == "item") {

        QMap<QString, QString> prm(MAP(req.query().queryItems()));

    }// else if(path == "item")

    return QHttpServerResponse(DFT.arg(cnt));
}// proc

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *prnt) : QMainWindow(prnt), ui(new Ui::FMain) {

    // Внешний вид.
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/ico.ico"));

    // Инициализация.
    QNetworkProxyFactory::setUseSystemConfiguration(false);

    this->tcp = new QTcpServer(this);
    this->srv.bind(this->tcp);
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
        this->ui->lw->addItem(item);
        this->ui->lw->setItemWidget(item, new Item);
    };// LW_ADD

    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);
    LW_ADD(new Item);

}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}//~FMain

// Проба. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btTest_clicked() {

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


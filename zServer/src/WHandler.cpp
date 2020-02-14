#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QFile>
#include <QMimeDatabase>
#include <QDesktopServices>
#include <QNetworkInterface>

#include "env.h" // Глобальная среда приложения.
#include "dbg.h"
#include "drw.h"

#include "ui_WHandler.h"
#include "WHandler.h"
#include "DHeader.h"

// Задать режим ожидания ответа. -----------------------------------------------
//------------------------------------------------------------------------------
void WHandler::setWait(WAIT_TYPE type) {
    this->wait_type = type;

    switch(this->wait_type) {
     case WAIT_TYPE::NOT : ui->btRsp->setVisible(false); break;
     case WAIT_TYPE::BTN : ui->btRsp->setVisible(true); break;
    }// switch(type)
}// setWait

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WHandler::WHandler(QWidget *parent) : QWidget(parent), ui(new Ui::WHandler) {

    // Внешний вид.
    ui->setupUi(this);
    ui->btRsp->setIcon(QIcon(":/img/run.png")); ui->btRsp->setVisible(false);

    // Меню (заголовки).
    header = new QMenu("Заголовки", this);
    header->addAction(ui->aAddHeader);

    // Меню (тип ожидания).
    QMenu *wait = new QMenu("Ожидание", this);
    wait->addAction(ui->aWaitNot);
    wait->addAction(ui->aWaitBtn);

    // Меню (основное).
    QActionGroup *grTypeAnswer = new QActionGroup(this);
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("color: black;");
    menu->addAction(ui->aRequest);
    menu->addMenu(wait);  // >>>
    menu->addSeparator(); //----------------
    menu->addMenu(header);// >>>
    menu->addSeparator(); //----------------
    menu->addAction(ui->aAsText); grTypeAnswer->addAction(ui->aAsText);
    menu->addAction(ui->aAsFile); grTypeAnswer->addAction(ui->aAsFile);
    menu->addAction(ui->aAsQuery); grTypeAnswer->addAction(ui->aAsQuery);
    menu->addSeparator(); //----------------
    menu->addAction(ui->aSetColor);
    menu->addSeparator(); //----------------
    menu->addAction(ui->aQuit);
    ui->btMenu->setMenu(menu);

    // Инициализация.
    this->color = Qt::black;
    ui->aAsText->triggered();

}// WHandler

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WHandler::WHandler(
    QString type, QString path, QColor color, QString text, QWidget *parent )
  : WHandler(parent)
{
    setColor(color);
    setType(type);
    ui->edPath->setText(path);
    ui->teAnswer->setText(text);
}// WHandler

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WHandler::~WHandler() {
    delete ui;

    FNC << "end";
}// ~WHandler

// Остановка. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aQuit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение", "Действительно закрыть ?" ))
    { emit remove(this); }
}// terminate

// Задать цвет. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aSetColor_triggered() {
    setColor(QColorDialog::getColor(this->color));
}// on_aSetColor_triggered

// Отвечать текстом. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aAsText_triggered() {
    ui->aAsText->setChecked(true);
    answer_type = ANSWER_TYPE::TEXT;
}// answerAsText

// Отвечать файлом. ------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aAsFile_triggered(){
    QString fnme = QFileDialog::getOpenFileName(nullptr, tr("Выбор файла"));
    if(!fnme.isEmpty()) {
        ui->teAnswer->setText(fnme);
        ui->aAsFile->setChecked(true);
        answer_type = ANSWER_TYPE::FILE;
    }// if(!fnme.isEmpty())
    else { ui->aAsText->triggered(); }
}// answerAsFile

// Отвечать запросом. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aAsQuery_triggered() {
    QMessageBox::warning(this, "Сообщение", "Не реализовано.");

//    ui->aAsQuery->setChecked(true);
//    answer_type = ANSWER_TYPE::QUERY;
}// answerAsQuery

// Изменить путь. --------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_edPath_textChanged(const QString &text) { this->path = text; }

// Получить ответ. -------------------------------------------------------------
//------------------------------------------------------------------------------
QHttpServerResponse WHandler::answer(void) {

    if(this->wait_type == WAIT_TYPE::BTN) {
        this->is_interrupted = false;

        ui->btRsp->setEnabled(true);
        QEventLoop loop;
        connect(ui->btRsp, &QPushButton::clicked, &loop, &QEventLoop::quit);
        loop.exec();
        ui->btRsp->setEnabled(false);

        if(this->is_interrupted) {
            return QHttpServerResponse(
                QHttpServerResponse::StatusCode::InternalServerError );
        }// if(this->is_interrupted)

    }// if(this->wait_type == WAIT_TYPE::BTN)

    switch(answer_type) {
      case ANSWER_TYPE::TEXT : return answer_text();
      case ANSWER_TYPE::FILE : return answer_file();
      case ANSWER_TYPE::QUERY: return answer_query();
    }// switch (answer_type)

    return answer_text();
}// answer

// Получить ответ в виде текста. -----------------------------------------------
//------------------------------------------------------------------------------
QHttpServerResponse WHandler::answer_text(void) {
    QHttpServerResponse rsp(ui->teAnswer->toPlainText().toUtf8());

    if(this->hdr.isEmpty()) {
        rsp.setHeader("Content-Type", "text/html; charset=utf-8");
    } else {
        rsp.clearHeaders();
        for(const Header &header : this->hdr)
            { rsp.addHeader(header.first.toUtf8(), header.second.toUtf8()); }
    }// else // if(this->hdr.isEmpty())

    return rsp;
}// answer_text

// Получить ответ в виде файла. ------------------------------------------------
//------------------------------------------------------------------------------
QHttpServerResponse WHandler::answer_file(void)
    { return QHttpServerResponse::fromFile(ui->teAnswer->toPlainText()); }

// Получить ответ в виде запроса. ----------------------------------------------
//------------------------------------------------------------------------------
QHttpServerResponse WHandler::answer_query(void) {
    qDebug() << "Не реализовано [answer_query]";
    return QHttpServerResponse("error answer query");
}// answer_query

// Текст в окне ответа. --------------------------------------------------------
//------------------------------------------------------------------------------
QString WHandler::plane_text(void) { return ui->teAnswer->toPlainText(); }

// Название типа ответа. -------------------------------------------------------
//------------------------------------------------------------------------------
QString WHandler::answer_type_name(void) {
    return answer_type == ANSWER_TYPE::TEXT  ? "TEXT"
         : answer_type == ANSWER_TYPE::FILE  ? "FILE"
         : answer_type == ANSWER_TYPE::QUERY ? "QUERY" : "UNDEF";
}// answer_type_name

// Прервать обработку запроса. -------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::interrupt()
    { this->is_interrupted = true; ui->btRsp->click(); }

// Выставить тип запроса по имени типа. ----------------------------------------
//------------------------------------------------------------------------------
void WHandler::setType(QString type_name) {

    if      (type_name == "TEXT")  { ui->aAsText->trigger(); }
    else if (type_name == "FILE")  {
        ui->aAsFile->setChecked(true);
        answer_type = ANSWER_TYPE::FILE;
    }else if(type_name == "QUERY") { ui->aAsQuery->trigger(); }

}// set_answer_type

// Выставить цвет. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::setColor(QColor color) {
    static QString NOT = "", CLR =
        "QLineEdit { background-color: %1; color: %2 }"
        "QTextEdit { background-color: %1; color: %2 }";
    this->color = color;
    this->setStyleSheet(CLR.arg(color.name()).arg(contrast_bw(color).name()));
}// answer_type_name

// Добавить заголовок. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aAddHeader_triggered() {
    if(E::Header->exec() == QDialog::Rejected) return;

    static QAction *separator = header->insertSeparator(ui->aAddHeader);
    QMenu *menu = new QMenu(E::Header->title(), this);
    QAction *action = new QAction("Удалить", menu);

    connect(action, &QAction::triggered, this, &WHandler::on_DelHeader );
    menu->addAction(action); hdr[menu] =
        qMakePair(E::Header->type(), E::Header->value());

    header->insertMenu(separator, menu);
}// on_aAddHeader_triggered

// Удалить заголовок. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_DelHeader() {
    QMenu *menu = qobject_cast<QMenu*>(sender()->parent());

    hdr.remove(menu);
    menu->deleteLater();
}// on_aDelHeader_triggered

// Вызвать в браузере. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aRequest_triggered() {
    QString URL("http://{ADR}:{PRT}/{PTH}");

    const QHostAddress &local = QHostAddress(QHostAddress::LocalHost);
    for(const QHostAddress &addr: QNetworkInterface::allAddresses()) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr != local) {
            QDesktopServices::openUrl(QUrl(URL
                .replace("{ADR}", addr.toString())
                .replace("{PRT}", QString::number(E::port))
                .replace("{PTH}", this->path) ));

            break;
        }//if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr != local)
    }// addr
}// on_aRequest_triggered

// Включить режим мгновенного ответа на запрос. --------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aWaitNot_triggered() { this->setWait(WAIT_TYPE::NOT); }

// Включить режим ответа на запрос по нажатию кнопки. --------------------------
//------------------------------------------------------------------------------
void WHandler::on_aWaitBtn_triggered() { this->setWait(WAIT_TYPE::BTN); }

//------------------------------------------------------------------------------


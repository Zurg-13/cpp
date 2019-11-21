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

#include "ui_WHandler.h"
#include "WHandler.h"
#include "DHeader.h"

// Задать режим ожидания ответа. -----------------------------------------------
//------------------------------------------------------------------------------
void WHandler::setWait(WAIT_TYPE type) {
    this->wait_type = type;

    switch(this->wait_type) {
     case WAIT_TYPE::NOT : ui->btRsp->setEnabled(false); break;
     case WAIT_TYPE::BTN : ui->btRsp->setEnabled(true); break;
    }// switch(type)
}// setWait

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WHandler::WHandler(QWidget *parent) : QWidget(parent), ui(new Ui::WHandler) {

    // Внешний вид.
    ui->setupUi(this);

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
}// ~WHandler

// Остановка. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aQuit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение", "Действительно закрыть ?") )
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
    QMessageBox::warning(this, "Сообщение", "Не реализоапно.");

//    ui->aAsQuery->setChecked(true);
//    answer_type = ANSWER_TYPE::QUERY;
}// answerAsQuery

// Изменить путь. --------------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_edPath_textChanged(const QString &text) { this->path = text; }

// Получить ответ. -------------------------------------------------------------
//------------------------------------------------------------------------------
QByteArray WHandler::answer(void) {
    FNC << "bgn";

    if(this->wait_type == WAIT_TYPE::BTN) {

        FNC << "entr loop";

        QEventLoop loop;
        connect(ui->btRsp, &QPushButton::clicked, &loop, &QEventLoop::quit);
        loop.exec();

        FNC << "quit loop";

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
QByteArray WHandler::answer_text(void) {
    const QString NS("\r\n");
    QString HEADER(
        "HTTP/1.0 200 Ok\r\n"
        "{HDR}"
        "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n" );

    return (
        HEADER.replace("{HDR}", hdr.size() ? header_lst().join(NS): "")
      + ui->teAnswer->toPlainText() ).toUtf8();

}// answer_text

// Получить ответ в виде файла. ------------------------------------------------
//------------------------------------------------------------------------------
QByteArray WHandler::answer_file(void) {
    QByteArray line, buff(
        "HTTP/1.0 200 Ok\r\n"
        "Content-Type: {MIME};\r\n\r\n" );
    QFile file(ui->teAnswer->toPlainText());

    if(!file.open(QIODevice::ReadOnly)) { return QByteArray(); }

    buff.replace("{MIME}", E::mime->mimeTypeForFile(file).name().toUtf8());
    while(!file.atEnd()) {
        line = file.read(1024);
        buff.append(line);
    }// while(!file.atEnd())

    return buff;
}// answer_file

// Получить ответ в виде запроса. ----------------------------------------------
//------------------------------------------------------------------------------
QByteArray WHandler::answer_query(void) {
    qDebug() << "[answer_query]";
    return QString("error answer query").toUtf8();
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
    static QString BKG_C = "background-color: %1;", EMPTY = "";
    this->color = color;
    this->setStyleSheet(color == Qt::black ? EMPTY : BKG_C.arg(color.name()));
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

// Сформировать список заголовков. ---------------------------------------------
//------------------------------------------------------------------------------
QList<QString> WHandler::header_lst(void) {
    QList<QString> ret;
    for(Header header: hdr.values())
        { ret.push_back(header.first + ": " + header.second); }
    return ret;
}// header_lst

// Вызвать в браузере. ---------------------------------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aRequest_triggered() {
    static QString URL("http://{ADR}:{PRT}/{PTH}");
    QDesktopServices::openUrl(QUrl(URL
        .replace("{ADR}", QNetworkInterface::allAddresses().first().toString())
        .replace("{PRT}", QString::number(E::port))
        .replace("{PTH}", this->path) ));
}// on_aRequest_triggered

// Включить режим мгновенного ответа на запрос. --------------------------------
//------------------------------------------------------------------------------
void WHandler::on_aWaitNot_triggered() { this->setWait(WAIT_TYPE::NOT); }

// Включить режим ответа на запрос по нажатию кнопки. --------------------------
//------------------------------------------------------------------------------
void WHandler::on_aWaitBtn_triggered() { this->setWait(WAIT_TYPE::BTN); }

//------------------------------------------------------------------------------


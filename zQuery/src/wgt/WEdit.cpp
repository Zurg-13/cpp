// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QtSql>
#include <QSqlDatabase>
#include <QMap>
#include <QRegExp>

#include "dbg.h"
#include "std.h"

#include "ui_WEdit.h"
#include "WEdit.h"
#include "WLog.h"
#include "DSel.h"

#include "wgt/WCnn.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WLog *wgLog;
extern WCnn *wgCnn;
extern DSel *dgSel;


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WEdit::WEdit(QWidget *parent) : QWidget(parent), ui(new Ui::WEdit) {

    // Внешний вид.
    ui->setupUi(this);
    highlighter = new SqlHighlighter(ui->edSql->document());
    out = new WOut();

    selection.format.setBackground(QColor(Qt::lightGray).lighter(115));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = ui->edSql->textCursor();

    ui->edSql->setFocus(); ui->edSql->cursorPositionChanged();

    // Инициализация.
    ui->edSql->installEventFilter(this);

}// WEdit

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WEdit::~WEdit() {
    DBG << "~WEdit";

    delete out;
    delete ui;
}// ~WEdit

// Открыть файл. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::on_aOpen_triggered() {
    QFile file(QFileDialog::getOpenFileName(
        nullptr, "Открыть", "d:/temp/12", "*.sql") );
    if(file.open(QFile::ReadOnly | QFile::Text))
        { ui->edSql->setPlainText(file.readAll()); }
}// on_aOpen_triggered()

// Колесо мыши. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::wheelEvent(QWheelEvent *evt) {
    static QFont fnt = ui->edSql->font();

    if(evt->modifiers() & Qt::ControlModifier) {
        fnt.setPointSizeF(
            fnt.pointSizeF() + (evt->angleDelta().y() > 0 ? +0.5 : -0.5 ) );
        ui->edSql->setFont(fnt);

        evt->ignore();
    }// if(evt->modifiers() & Qt::ControlModifier)

    QWidget::wheelEvent(evt);
}// wheelEvent

// Горячие кнопки. -------------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::keyPressEvent(QKeyEvent *evt) {
    Qt::Key key = static_cast<Qt::Key>(evt->key());
    Qt::KeyboardModifiers mod = evt->modifiers();

    // Ctrl + Enter
    if(mod.testFlag(Qt::ControlModifier) && key == Qt::Key_Return)
        { ui->aExec->trigger(); evt->accept(); }

    QWidget::keyPressEvent(evt);
}// keyPressEvent

// Проба. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::on_btTest_clicked() {
    FNC << R"(/bgn)";

    this->db.setDatabaseName("db.sqlite");
    if(NOT(this->db.open())) { FNC << this->db.lastError(); }

    FNC << R"(\end)";
}// on_btTest_clicked

// Выполнить запрос. -----------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::on_aExec_triggered() {
    QSqlQuery qry(this->db); // todo: !!!

    // Выполнение запроса.
    wgLog->add(qry.exec(curSql())
        ? QString("query ok")
        : qry.lastError().text() );

    while(qry.next()) {
        QList<QString> res;
        for(int i=0; i < qry.record().count(); i++)
            { res << qry.value(i).asSTR; }
    }// while(qry.next())

    this->out->setQuery(qry);
    this->out->show();
}// on_aExec_triggered

// Извлечь и подготовить запрос. -----------------------------------------------
//------------------------------------------------------------------------------
QString WEdit::curSql(void) {
    static QRegExp QUO(R"(/\*(.*)\*/)");
    static QRegExp NME(R"(.*nme:(.*)(rem:|prm:|$))");
    static QRegExp REM(R"(.*rem:(.*)(nme:|prm:|$))");
    static QRegExp PRM(R"(.*prm:(.*)(nme:|rem:|$))");
    QString sql_txt = ui->edSql->toPlainText().mid(this->txt_bgn, this->txt_end);
    QString ntf_txt, nme_txt, rem_txt, prm_txt;
    QMap<QString, QString> prm;
    auto Q = [](QString str) -> QString
        { return QString("<<") + str + QString(">>"); };

    QUO.setMinimal(true); NME.setMinimal(true);
    REM.setMinimal(true); PRM.setMinimal(true);

    // Разбор заголовка, извлечение параметров.
    QUO.indexIn(sql_txt); ntf_txt = QUO.cap(1).trimmed();
    NME.indexIn(ntf_txt); nme_txt = NME.cap(1).trimmed();
    REM.indexIn(ntf_txt); rem_txt = REM.cap(1).trimmed();
    PRM.indexIn(ntf_txt); prm_txt = PRM.cap(1).trimmed();
    for(QString entry : prm_txt.split(',')) {
        QStringList pair = entry.split('=');
        prm[pair.first().trimmed()] = pair.last().trimmed();
    }// entry

    // Генерация исполняемого запроса.
    for(QString key : prm.keys())
        { sql_txt = sql_txt.replace(Q(key), prm[key]); }

    return sql_txt;
}// curSql

// Тестовое действие. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::on_aTest_triggered() {
    DBG << "on_aTest_triggered";
}// on_aTest_triggered

// Выбор подключения. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::on_btCnn_clicked() {
    static int cnt = 0; cnt++;

    QMap<QVariant, QString> map;
    for(WConnect *conn : wgCnn->lst()) {
        map.insert(
            QVariant::fromValue(conn)
          , FSN(conn->nme(), FSN(conn->usr(), conn->url(), " -> "), ": "));
    }// conn

    if(dgSel->sel(&map)) {
        WConnect* conn = dgSel->ret().value<WConnect*>();
        this->db.setDatabaseName(conn->url());
        this->db.setUserName(conn->usr());
        this->db.setPassword(conn->pwd());

        if(this->db.open()) {
            wgLog->add(QString("connect to: ") + conn->nme() + QString(" ok"));
            ui->btCnn->setText(conn->nme());
        } else {
            wgLog->add(db.lastError().text());
        }// if(this->db.open())

        FNC << "ok" << conn->nme();
    } else {
        FNC << "no";
    }// else // if(wgSel->sel(lst))

}// on_btCnn_clicked

// Запись состояния в XML-документ. --------------------------------------------
//------------------------------------------------------------------------------
void WEdit::write(QXmlStreamWriter &doc) {

    doc.writeStartElement("edit");

        doc.writeStartElement("file");
        doc.writeCharacters("не реализовано");
        doc.writeEndElement();

    doc.writeEndElement();  //edit

}// write

// Изменение позиции курсора. --------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::on_edSql_cursorPositionChanged() {
    static QRegExp sep(R"(;$)");
    QList<QTextEdit::ExtraSelection> extraSelections;
    int pos = ui->edSql->textCursor().position();

    QTextDocument *doc = ui->edSql->document();
    QTextCursor crs(doc); crs.movePosition(QTextCursor::End);
    txt_bgn = 0; txt_end = -1;
    while((txt_end = doc->find(sep, txt_bgn).position()-1) > 0) {

/*
        FNC << "txt_end_chr:" << doc->characterAt(txt_end)
            << "txt_bgn:" << txt_bgn << "txt_end:" << txt_end;
*/

        if(txt_bgn <= pos && pos <= txt_end) { break; }
        txt_bgn = txt_end + 2;
    }// while((txt_end = doc->find(sep, txt_bgn).position()) > 0)
    if(txt_end < 0) { txt_end = crs.position(); }

/*
    FNC << "bgn:" << txt_bgn << "end:" << txt_end
        << "pos:" << pos << "chr:" << doc->characterAt(pos);
*/

    selection.cursor.clearSelection();
    selection.cursor.setPosition(txt_bgn, QTextCursor::MoveAnchor);
    selection.cursor.setPosition(txt_end, QTextCursor::KeepAnchor);

    extraSelections.append(selection);
    ui->edSql->setExtraSelections(extraSelections);
}// on_edSql_cursorPositionChanged

//------------------------------------------------------------------------------


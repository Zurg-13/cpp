// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QHeaderView>

#include "sql.h"


/* Служебные функции. *********************************************************/
/******************************************************************************/
const QString SQL_ERR_MSG("Ошибка SQL-запроса (%1)");
const QString SQL_NME_MSG("Ошибка именования");

// Найти положение символа конца строки "\0". ----------------------------------
//------------------------------------------------------------------------------
int FIND_END(const QString &str){
    int ret(0);
    QChar end('\0');
    while(str[ret] != end && ret < str.length()) { ret++; }
    return ret;
}// FIND_END

// Расширенное сообщение (содержит скрываемые детели).--------------------------
//------------------------------------------------------------------------------
void ExtMsg(const QString &txt, const QString &inf, const QString &dtl) {
    QMessageBox mb;
    mb.setStandardButtons(QMessageBox::NoButton);

    mb.setWindowTitle("ERR SQL");
    mb.setStyleSheet("QLabel{min-width: 700px;}");
    mb.setText(txt);
    mb.setInformativeText(inf);
    mb.setDetailedText(dtl);

    mb.exec();
}// ExtMsg

// Список объявленных параметров со значениями.---------------------------------
//------------------------------------------------------------------------------
QString BoundParams(const QSqlQuery &q){
    QString ret("\n\n ПАРАМЕТРЫ ==============\n");
    QMap<QString, QVariant> prm = q.boundValues();
    QList<QString> key = prm.keys();

    QList<QString>::iterator it = key.begin(), end = key.end();
    while(it != end){
        ret += (*it) + " = " + prm[(*it)].toString() + "\n";
        it++;
    }// while(bgn != end)

    return ret;
}// BindParams

// Выполненть SQL запрос.-------------------------------------------------------
//------------------------------------------------------------------------------
bool ExecSQL(QSqlQuery *q) { return ExecSQL(*q); }
bool ExecSQL(QSqlQuery &q) {
    if(!q.exec()){
        QString msg(q.lastError().databaseText());
        ExtMsg(
            SQL_ERR_MSG.arg(q.lastError().nativeErrorCode())
          , msg.left(FIND_END(msg)) + "\n" + q.lastError().driverText()
          , q.lastQuery() + BoundParams(q));
        return false;
    }// if(!q.exec())
    return true;
}// ExecSQL


/* ZSqlQuery ******************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
ZSqlQuery::ZSqlQuery(
    const QString &sql, const QSqlDatabase &db, ZSqlQuery::PREPARE prep )
  : QSqlQuery(db), sql(sql)
{ if(prep == PREPARE::YES) { prepare(); } }

//Назначение параметров(ULONGLONG).---------------------------------------------
//------------------------------------------------------------------------------
void ZSqlQuery::operator () (const QString &fld, const qulonglong var ) {
    if(fld[0] != ':') {
        QMessageBox::critical(
            nullptr, SQL_NME_MSG, fld+"\n" + this->lastQuery() );
    }// if(fld[0] != ':')

    this->bindValue(fld, QString::number(var));
}// operator ()

//Назначение параметров(VARIANT).-----------------------------------------------
//------------------------------------------------------------------------------
void ZSqlQuery::operator () (const QString &fld, const QVariant &var ) {
    if(fld[0] != ':') {
        QMessageBox::critical(
            nullptr, SQL_NME_MSG, fld+"\n" + this->lastQuery() );
    }// if(fld[0] != ':')
    this->bindValue(fld, var);
}// operator ()

//Обращение по имени поля.------------------------------------------------------
//------------------------------------------------------------------------------
QVariant ZSqlQuery::curVal(const QString &fld)
    { return this->record().value(fld); }

//Подготовить прикреплённый запрос.---------------------------------------------
//------------------------------------------------------------------------------
bool ZSqlQuery::prepare(void)
    { fl_prp = QSqlQuery::prepare(sql); return fl_prp; }

//Обновить.---------------------------------------------------------------------
//------------------------------------------------------------------------------
bool ZSqlQuery::upd(void) {
    bool ret;

    if(!fl_prp) { prepare(); }

    ret = this->exec();

    //Игнорируем ошибки подключения.
    if(!ret && this->lastError().type() != QSqlError::ConnectionError) {
        QString msg(this->lastError().databaseText());
        ExtMsg(
            SQL_ERR_MSG.arg(this->lastError().nativeErrorCode())
          , msg.left(FIND_END(msg)) + "\n" + this->lastError().driverText()
          , this->lastQuery() + BoundParams(*this));
    }// if(!ret)

    return ret;
}// upd


/* ZTableView *****************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
ZTableView::ZTableView(QWidget *parent)
  : QTableView(parent), mdl(parent), prx(parent)
{
    prx.setSourceModel(&mdl); this->setModel(&prx);

    prx.setObjectName("prx");
    mdl.setObjectName("mdl");

   // Шрифт заголовка таблицы.
    QFont fnt = this->font(); fnt.setBold(true);
    this->horizontalHeader()->setFont(fnt);

}// ZTableView

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
ZTableView::~ZTableView() {

}// ~ZTableView

// Установить ширину колонки. --------------------------------------------------
//------------------------------------------------------------------------------
void ZTableView::setColumnWdt(const QString &nme, int wdt)
    { QTableView::setColumnWidth(mdl.record().indexOf(nme), wdt); }

//Установить видимость секций.--------------------------------------------------
//------------------------------------------------------------------------------
void ZTableView::sec(const QStringList &sections, bool visible) {
    QStringList::const_iterator it = sections.begin(), end = sections.end();

    if(visible){
        for(int i=0; i < mdl.columnCount(); i++) { this->hideColumn(i); }
        while(it != end) { this->showColumn(mdl.record().indexOf(*it)); it++; }
    }// if(visible)
    else{
        for(int i=0; i < mdl.columnCount(); i++) { this->showColumn(i); }
        while(it != end) { this->hideColumn(mdl.record().indexOf(*it)); it++; }
    }// else // if(visible)

}// sec

//Установить название колонки.--------------------------------------------------
//------------------------------------------------------------------------------
void ZTableView::setFieldTitle(const QString &fld, const QString &ttl) {
    mdl.setHeaderData(
        mdl.record().indexOf(fld), Qt::Horizontal, ttl, Qt::DisplayRole );
    mdl.setHeaderData(
        mdl.record().indexOf(fld), Qt::Horizontal, fld.toLower(), Qt::EditRole);
}// setFieldTitle

//Считать текущее значение.-----------------------------------------------------
//------------------------------------------------------------------------------
QVariant ZTableView::curVal(const QString &fld){
    if(!CUR_IDX(this).isValid()) { return QVariant(QVariant::String); }
    return mdl.record(prx.mapToSource(CUR_IDX(this)).row()).value(fld);
}// curVal

//Сохранить внешний вид.--------------------------------------------------------
//------------------------------------------------------------------------------
void ZTableView::save(const QString &obj, QSettings *s){
    if(!this->isEnabled()) { return; }  //Обход сохранения "пустых" конфигов.

    s->setValue(obj + "/font_family", this->font().family());
    s->setValue(obj + "/font_psize", this->font().pointSize());
    s->setValue(obj + "/font_weight", this->font().weight());
    s->setValue(obj + "/font_italic", this->font().italic());

    s->setValue(obj + "/state_headerH", this->horizontalHeader()->saveState());
    s->setValue(obj + "/geom_headerH", this->horizontalHeader()->saveGeometry());
    s->setValue(obj + "/state_headerV", this->verticalHeader()->saveState());
    s->setValue(obj + "/geom_headerV", this->verticalHeader()->saveGeometry());
}// save

//Восстановить внешний вид.-----------------------------------------------------
//------------------------------------------------------------------------------
void ZTableView::rest(const QString &obj, QSettings *s){
    QFont fnt;

    fnt.setFamily(s->value(obj + "/font_family").toString());
    fnt.setPointSize(s->value(obj + "/font_psize").toInt());
    fnt.setWeight(s->value(obj + "/font_weight").toInt());
    fnt.setItalic(s->value(obj + "/font_italic").toBool());
    this->setFont(fnt);

    this->horizontalHeader()->restoreState(
        s->value(obj + "/state_headerH").toByteArray() );
    this->horizontalHeader()->restoreGeometry(
        s->value(obj + "/geom_headerH").toByteArray() );
    this->verticalHeader()->restoreState(
        s->value(obj + "/state_headerV").toByteArray() );
    this->verticalHeader()->restoreGeometry(
        s->value(obj + "/geom_headerV").toByteArray() );
}// rest

//------------------------------------------------------------------------------

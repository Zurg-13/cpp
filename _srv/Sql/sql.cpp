// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QHeaderView>

#include "sql.h"
#include "lst.h"


/* Служебные функции. *********************************************************/
/******************************************************************************/
const QString ERR_SQL_MSG("Ошибка SQL-запроса (%1)");
const QString ERR_NME_PRM("Ошибка именования параметров");

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
void MSG_EXT(const QString &txt, const QString &inf, const QString &dtl) {
    QMessageBox mb;
    mb.setStandardButtons(QMessageBox::NoButton);

    mb.setWindowTitle("ERR SQL");
    mb.setStyleSheet("QLabel{min-width: 700px;}");
    mb.setText(txt);
    mb.setInformativeText(inf);
    mb.setDetailedText(dtl);

    mb.exec();
}// ExtMsg

// Значения связанных с запросом параметров. -----------------------------------
//------------------------------------------------------------------------------
QString BND_PRM(const QSqlQuery &q) {
    return
        QString("\n\n ПАРАМЕТРЫ ==============\n")
      + BLD(
            LST(q.boundValues())
          , [](const QPair<QString, QVariant> &pr) -> QString
                { return  pr.first + "=" + pr.second.toString(); }, "\n" );
}// BND_PRM

// Выполненть SQL запрос.-------------------------------------------------------
//------------------------------------------------------------------------------
bool ExecSQL(QSqlQuery *q) { return ExecSQL(*q); }
bool ExecSQL(QSqlQuery &q) {
    if(!q.exec()){
        QString msg(q.lastError().databaseText());
        MSG_EXT(
            ERR_SQL_MSG.arg(q.lastError().nativeErrorCode())
          , msg.left(FIND_END(msg)) + "\n" + q.lastError().driverText()
          , q.lastQuery()
              + QString("\n\n ПАРАМЕТРЫ ==============\n")
              + BLD(LST(
                    q.boundValues())
                  , [](const QPair<QString, QVariant> &pr) -> QString
                        { return  pr.first + "=" + pr.second.toString(); }
                  , ", " ));
        return false;
    }// if(!q.exec())
    return true;
}// ExecSQL


/* ZSqlQuery ******************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
ZSqlQuery::ZSqlQuery(const QString &sql, const QSqlDatabase &db) : QSqlQuery(db)
    { prepare(sql); }

//Назначение параметров. -------------------------------------------------------
//------------------------------------------------------------------------------
void ZSqlQuery::verify_fld_name(const QString &name) {
    if(':' != name.front()) {
        QMessageBox::critical(
            nullptr, ERR_NME_PRM, name + "\n" + this->lastQuery() );
    }// if(':' != name.front())
}// verify_fld_name

ZSqlQuery& ZSqlQuery::operator () (const QString &fld, const qulonglong val)
    { verify_fld_name(fld); this->BV(fld, QString::number(val)); return *this; }
ZSqlQuery& ZSqlQuery::operator () (const QString &fld, const QVariant &val )
    { verify_fld_name(fld); this->BV(fld, val); return *this; }
ZSqlQuery& ZSqlQuery::operator ()(const QMap<QString, QVariant> &prm) {
    QMap<QString, QVariant>::const_iterator entt = prm.constBegin();
    for(; entt != prm.constEnd(); entt++)
        { verify_fld_name(entt.key()); this->BV(entt.key(), entt.value()); }
    return *this;
}// operator ()

//Обращение по имени поля.------------------------------------------------------
//------------------------------------------------------------------------------
QVariant ZSqlQuery::val(const QString &fld)
    { return this->record().value(fld); }

//Обновить.---------------------------------------------------------------------
//------------------------------------------------------------------------------
ZSqlQuery& ZSqlQuery::exe(const ZPrm &prm, bool *ok) {
    operator()(prm);

    if(exec()) {
        if(ok) { *ok = true; }
    } else {
        if(ok) { *ok = false; }

        QString msg(this->lastError().databaseText());
        MSG_EXT(
            ERR_SQL_MSG.arg(this->lastError().nativeErrorCode())
          , msg.left(FIND_END(msg)) + "\n" + this->lastError().driverText()
          , this->lastQuery() + BND_PRM(*this));
    }// if(exec())

    return *this;
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

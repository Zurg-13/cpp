#ifndef SQL_H
#define SQL_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QVariant>
#include <QSettings>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>


/* Константы. *****************************************************************/
/******************************************************************************/

/* Макросы ********************************************************************/
/******************************************************************************/
#define DB(n) QSqlDatabase::database(n)
#define BV(n, v) bindValue(n, v)
#define UPD(q) do { q.upd(); q.first(); } while(false)
#define EXEC(q) ExecSQL(q)

#define IDX(m, n) qobject_cast<QSqlQueryModel*>(m)->record().indexOf(n)
#define PRX(p) qobject_cast<QSortFilterProxyModel*>(p)
#define MDL(m) qobject_cast<QSqlQueryModel*>(m)

#define CUR_IDX(v) v->selectionModel()->currentIndex()
#define ROW(v) v->selectionModel()->currentIndex().row()
#define CBI(c) c->itemData(c->currentIndex())
#define CBT(c) c->itemText(c->currentIndex())


/* Служебные функции. *********************************************************/
/******************************************************************************/

// Выполнить SQL-запрос. -------------------------------------------------------
//------------------------------------------------------------------------------
bool ExecSQL(QSqlQuery &q);
bool ExecSQL(QSqlQuery *q);

/* Расширение QSqlQuery. ******************************************************/
/******************************************************************************/
class ZSqlQuery : public QSqlQuery {

 public:
    enum class PREPARE {YES, NOT };

    explicit ZSqlQuery(const QSqlDatabase &db): QSqlQuery(db) {}
             ZSqlQuery(
                 const QString &sql, const QSqlDatabase &db
               , PREPARE prep = PREPARE::YES );
             ZSqlQuery(void) {}

    bool upd    (void);
    bool prepare(void);
    void setSQL (const QString &sql) { this->sql = sql; un_prep(); }
    bool upd_fst(void) { bool ret = upd(); first(); return ret; }
    void pre_fst(void) { seek(-1); } // В положение ПЕРЕД первой записью.
    void un_prep(void) { fl_prp = false; }
    bool is_prep(void) { return fl_prp; }

    const QString& getSQL (void) { return this->sql; }

    QVariant    curVal  (const QString &fld); // Считать текущее значение.
    QVariant operator [](const QString &fld){ return curVal(fld); }

    void operator ()(const QString &fld, const QVariant &val);
    void operator ()(const QString &fld, const qulonglong val);

 private:
    bool fl_prp = false;
    QString sql;

};// ZSqlQuery

/* Расширение QTableView. *****************************************************/
/******************************************************************************/
class ZTableView : public QTableView {

 public:
    explicit ZTableView(QWidget *parent = nullptr);
    virtual ~ZTableView();

    void     setSourceSQL(const QSqlQuery &sql) { mdl.setQuery(sql); }
    void     setSourceSQL(const QSqlQuery *sql) { mdl.setQuery(*sql); }
    void     showSections(const QStringList &sections) { sec(sections, true); }
    void     hideSections(const QStringList &sections) { sec(sections, false); }

    void     setColumnWdt  (const QString &fld, int wdt);
    void     setFieldTitle (const QString &fld, const QString &ttl);

    QVariant curVal     (const QString &fld); // Считать текущее значение.
    QVariant operator [](const QString &fld) { return curVal(fld); }

    void save(const QString &obj, QSettings *s); // Сохранить внешний вид.
    void rest(const QString &obj, QSettings *s); // Восстановить внешний вид.

    QSqlQueryModel mdl;
    QSortFilterProxyModel prx;

 private:
    void sec(const QStringList&, bool);

};// ZTableView

//------------------------------------------------------------------------------
#endif // SQL_H

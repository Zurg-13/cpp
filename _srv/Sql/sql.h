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
#define EXE(q) ExecSQL(q)

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
typedef QMap<QString, QVariant> ZPrm;
class ZSqlQuery : public QSqlQuery {

 public:
    ZSqlQuery(const QString &sql, const QSqlDatabase &db);

    ZSqlQuery& exe(const ZPrm &prm = ZPrm(), bool *ok = nullptr);
    ZSqlQuery& fst(void) { first(); return *this; }
    ZSqlQuery& pre(void) { seek(-1); return *this; } // ПЕРЕД первой записью.

    ZSqlQuery& operator ()(const QString &fld, const QVariant &val);
    ZSqlQuery& operator ()(const QString &fld, const qulonglong val);
    ZSqlQuery& operator ()(const QMap<QString, QVariant> &prm);

    QVariant   val        (const QString &fld); // Считать текущее значение.
    QVariant   operator [](const QString &fld){ return val(fld); }

 private:
    void verify_fld_name(const QString &name);

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

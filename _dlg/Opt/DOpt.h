#ifndef DOPT_H
#define DOPT_H

//INCLUDE.----------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QDialog>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "_srv/sql.h"

//NAMESPACE.--------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class DOpt; }


//Класс формы.------------------------------------------------------------------
//------------------------------------------------------------------------------
class DOpt : public QDialog{
    Q_OBJECT

public:
    explicit DOpt(QWidget *parent = 0);
            ~DOpt();

    //Выборка по полю "RET", отображается поле "SHOW", поиск по параметру "FND".
    int from(ZSqlQuery &qry, const QString ttl, int width = 0);

    QVariant ret(void) { return val; }
    QVariant get(const QString &fld) { return rec.value(fld); }

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void on_btOk_clicked();
    void on_tv_doubleClicked(QModelIndex);
    void on_edFind_textChanged(QString);

private:
    Ui::DOpt *ui;

    ZSqlQuery *qry;
    QSqlRecord rec;
    QVariant val;

};// DOpt

//------------------------------------------------------------------------------
#endif // DOPT_H

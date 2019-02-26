#ifndef WOUT_H
#define WOUT_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QSqlQueryModel>

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class WOut; }

/* Отображение вывода запроса. ************************************************/
/******************************************************************************/
class WOut : public QWidget {
    Q_OBJECT

public:
    explicit WOut(QWidget *parent = 0);
            ~WOut();

    void setQuery(QSqlQuery &qry);

private slots:
    void on_cbAutoFit_clicked();

private:
    Ui::WOut *ui;
    QSqlQueryModel *mdl;

}; // WOut

//------------------------------------------------------------------------------
#endif // WOUT_H

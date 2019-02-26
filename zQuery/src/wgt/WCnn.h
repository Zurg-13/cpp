#ifndef WCNN_H
#define WCNN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QList>
#include <QXmlStreamWriter>

#include "WConnect.h"

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class WCnn; }


/* Список подключений. ********************************************************/
/******************************************************************************/
class WCnn : public QWidget {
    Q_OBJECT

public:
    explicit WCnn(QWidget *parent = 0);
            ~WCnn();

    const QList<WConnect*>& lst(void) { return cnn; }
    void write(QXmlStreamWriter &doc);
    void state(const QString &xml);

private:
    Ui::WCnn *ui;
    QList<WConnect*> cnn;

    WConnect* addConn(void);

private slots:
    void remove_conn(WConnect*);
    void on_btAdd_clicked();

    void on_btHide_clicked();
};// WCnn

//------------------------------------------------------------------------------
#endif // WCNN_H

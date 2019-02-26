#ifndef WCONNECT_H
#define WCONNECT_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QXmlStreamWriter>

namespace Ui { class WConnect; }

/* Параметры подключения к БД. ************************************************/
/******************************************************************************/
class WConnect : public QWidget {
    Q_OBJECT

public:
    explicit WConnect(QWidget *parent = 0);
            ~WConnect();

    QString nme(void);
    QString usr(void);
    QString pwd(void);
    QString url(void);

    void write(QXmlStreamWriter &doc);
    void state(const QString &xml);

signals:
    void remove(WConnect *);

private slots:
    void on_aDelete_triggered();
    void on_aTest_triggered();

private:
    Ui::WConnect *ui;
    static int global_count;   //Глобальный счётчик.
    int count = global_count++;

};// WConnect

//------------------------------------------------------------------------------
#endif // WCONNECT_H

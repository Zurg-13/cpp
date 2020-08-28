#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QtHttpServer>

namespace Ui { class FMain; }


/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

 public:
    explicit FMain(QWidget *prnt = nullptr);
            ~FMain();

private slots:
    void on_btTest_clicked();
    void on_btRun_clicked();

private:
    Ui::FMain *ui;
    QTcpServer *tcp = nullptr;
    QHttpServer srv;

    QHttpServerResponse proc(const QString&, const QHttpServerRequest&);
};

//------------------------------------------------------------------------------
#endif // FMAIN_H

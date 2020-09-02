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
    QString path;

    void SET_SQL(void);
    void SET_PRM(const QStringList &args);
    QHttpServerResponse proc(const QString&, const QHttpServerRequest&);
};

//------------------------------------------------------------------------------
#endif // FMAIN_H

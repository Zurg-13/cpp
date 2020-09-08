#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QtHttpServer>
#include <QWebSocket>
#include <QMoveEvent>

namespace Ui { class FMain; }


/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

 public:
    explicit FMain(QWidget *prnt = nullptr);
            ~FMain();

 protected:
    void moveEvent(QMoveEvent *evt);

 private slots:
    void on_ws_connect();
    void on_ws_txt_msg(QString msg);
    void on_ws_bin_msg(QByteArray msg);
    void on_ws_disconnect();

    void on_btRun_clicked();
    void on_aExit_triggered();
    void on_aLog_triggered();
    void on_aWsSend_triggered();
    void on_aTest_triggered();

 private:
    Ui::FMain *ui;
    QTcpServer *tcp = nullptr;
    QHttpServer srv;
    QWebSocket *last = nullptr;
    QString path;

    void SET_SQL(void);
    void ROUTING(void);
    void SET_PRM(const QStringList &args);
    QHttpServerResponse proc(const QString&, const QHttpServerRequest&);
    QHostAddress addr(void);

};

//------------------------------------------------------------------------------
#endif // FMAIN_H

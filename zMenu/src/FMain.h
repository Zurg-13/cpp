#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QMainWindow>
#include <QtHttpServer>
#include <QWebSocket>
#include <QMoveEvent>
#include <QSqlQuery>

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

    void on_aTestFillDB_triggered();

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

    // Обработчики.
    void EXEC(
        QWebSocket *rsp, const QJsonObject &obj, QSqlQuery &sql
      , std::function<void(QSqlQuery&)> done);


    void item_post(QWebSocket *rsp, const QJsonObject &obj);
    void item_list(QWebSocket *rsp, const QJsonObject &obj);
    void item_save(QWebSocket *rsp, const QJsonObject &obj);
    void item_drop(QWebSocket *rsp, const QJsonObject &obj);

    void room_post(QWebSocket *rsp, const QJsonObject &obj);
    void room_list(QWebSocket *rsp, const QJsonObject &obj);
    void room_save(QWebSocket *rsp, const QJsonObject &obj);
    void room_drop(QWebSocket *rsp, const QJsonObject &obj);

    void type_post(QWebSocket *rsp, const QJsonObject &obj);
    void type_list(QWebSocket *rsp, const QJsonObject &obj);
    void type_save(QWebSocket *rsp, const QJsonObject &obj);
    void type_drop(QWebSocket *rsp, const QJsonObject &obj);
};

//------------------------------------------------------------------------------
#endif // FMAIN_H

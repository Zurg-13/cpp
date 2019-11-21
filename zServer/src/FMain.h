#ifndef FMAIN_H
#define FMAIN_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QList>
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "WHandler.h"

namespace Ui { class FMain; }

// Главная форма. --------------------------------------------------------------
//------------------------------------------------------------------------------
class FMain : public QMainWindow {
    Q_OBJECT

 public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

 private slots:
    void on_btAdd_clicked();

 private:
    Ui::FMain *ui;
    QTcpServer *tcp = nullptr;
    QList<WHandler*> hdl;

    void log(const QString &msg, const QString &stl);
    void log(const QString &msg, const QColor &clr = QColor());
    void log_html(const QString &html);
    void spc(void);

    void addHandler(QString type, QString path, QColor color, QString text);
    void clearHdl(void);

 private slots:
    void remove_handler(WHandler*);

    void new_connect(void);
    void read_socket(void);
    void close_socket(void);
    void error_socket(QAbstractSocket::SocketError);

    void on_btStart_clicked();
    void on_btClearLog_clicked();
    void on_aClearHdl_triggered();
    void on_btUp_clicked();

    void on_aConfSave_triggered();
    void on_aConfLoad_triggered();
    void on_aExit_triggered();
    void on_btDebug_clicked();
    void on_btDebugGrab_clicked();
    void on_btDebugFree_clicked();
};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

#ifndef FMAIN_H
#define FMAIN_H

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

    void log(const QString &msg, const QColor &clr = QColor());
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
    void on_btSave_clicked();
    void on_btLoad_clicked();
    void on_aClearHdl_triggered();
    void on_btUp_clicked();

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

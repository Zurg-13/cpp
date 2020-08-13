#ifndef FMAIN_H
#define FMAIN_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QList>
#include <QMainWindow>
#include <QtHttpServer>

#include "WHandler.h"
#include "WLogEntry.h"

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
    QHttpServer srv;
    QList<WHandler*> hdl;

    void       post(const QString &msg, const QColor &clr = QColor());
    void       post(
        const QString &inp, const QString &out, const QColor &clr = QColor() );
    WLogEntry* grab(void);
    void       free(WLogEntry *entry);
    void       rift(void);

    void addHandler(QString type, QString path, QColor color, QString text);
    void clearHdl(void);

    QHttpServerResponse proc(const QString &path, const QHttpServerRequest &req);

 private slots:
    void remove_handler(WHandler*);

    void on_btStart_clicked();
    void on_btClearLog_clicked();
    void on_aClearHdl_triggered();
    void on_btUp_clicked();

    void on_aConfSave_triggered();
    void on_aConfLoad_triggered();
    void on_aExit_triggered();
    void on_btDebug_clicked();
};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

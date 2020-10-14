#ifndef TRACEMGR_H
#define TRACEMGR_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QObject>
#include <QThread>
#include <QList>
#include <QTextCodec>

#include "_unt/AtomicQueue.hpp"
#include "IcmpTrace.h"

// Структура хранения ошибки (возвращаемая). -----------------------------------
//------------------------------------------------------------------------------
struct GenError {
    int code;
    char *text = nullptr;

    GenError(void) {}
    GenError(int code, const QString &str, const char *enc): code(code) {
        QByteArray arr = QTextCodec::codecForName(enc)->fromUnicode(str);

        text = new char[arr.size() + 1]; text[arr.size()] = '\0';
        strcpy(text, arr.constData());
    }// Error

   ~GenError() { delete[] text; }
};// GenError

// Структура хранения ошибок (внутренняя). -------------------------------------
//------------------------------------------------------------------------------
struct InsError {
    int code; QString text;
    InsError(int code, const QString &text): code(code), text(text) {}
};// InsError


/* Обработчик однопоточной трассировки (потоковый). ***************************/
/******************************************************************************/
class TraceTrd : public QThread {
    Q_OBJECT

 public:
    virtual ~TraceTrd();
    explicit TraceTrd(
        QObject *prnt
      , const QString &addr, const QString &name
      , int deep, int tout, int hush );

 signals:
//    void ready(HopsRes rslt);
    void ready(QString addr, QString name, int term, int cerr, int hops);
    void finished(void);

 private:
   QString addr, name;
   int deep, tout, hush;
   IcmpTrace trace;

   void run() override;
};// TraceTrd

/* Обработчик однопоточной трассировки. ***************************************/
/******************************************************************************/
class TraceWrk : public QObject {
    Q_OBJECT

 public:
    virtual ~TraceWrk();
    explicit TraceWrk(
        const QString &addr, int deep, int tout, int hush
      , QObject *prnt = nullptr );

 signals:
    void ready(HopsRes rslt);
    void finished(void);

 public slots:
    void proc(void);

 private:
    QString addr;
    int deep, tout, hush;
    IcmpTrace trace;

};// TraceWrk

/* Управление многопоточным трассированием. ***********************************/
/******************************************************************************/
typedef QList<QPair<QString, HopsRes>> Res;
class TraceMgr : public QObject {
    Q_OBJECT

 public:
    explicit TraceMgr(QObject *prnt = nullptr);
    const Res& proc(
        const char *xml, int deep, int tout, int hush, int thread_num);

 signals:
    void ready(void);

 public slots:
    void accept(QString addr, QString name, int term, int cerr, int hops);

 private:
    int deep = 32, tout = 1000, hush = 1000, task = 0;
    AtomicQueue<QPair<QString, QString>> *que = nullptr; // addr, name
    GenError *err_out = nullptr;
    Res res;

    void RUN(TraceWrk *trace, QThread *thread);
    void RUN(TraceTrd *trace);

};// TraceMgr

//------------------------------------------------------------------------------
#endif // TRACEMGR_H

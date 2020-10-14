// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QXmlStreamReader>
#include <QHostInfo>
#include <QHostAddress>

#include "dbg.h"
#include "tme.h"

#include "TraceMgr.h"
#include "WaitForSignal.h"


/* TraceTrd. ******************************************************************/
/******************************************************************************/

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
TraceTrd::~TraceTrd() {
    FNC << "end";
}// ~TraceTrd

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
TraceTrd::TraceTrd(
    QObject *prnt
  , const QString &addr, const QString & name, int deep, int tout, int hush )
  : QThread(prnt), addr(addr), name(name), deep(deep), tout(tout), hush(hush)
{
    FNC << "bgn";
}// TYraceTrd

// Запускаемая в отдельном потоке функция. -------------------------------------
//------------------------------------------------------------------------------
void TraceTrd::run() {
    FNC << SYSDATE;

    PingRes ping = trace.ping(this->addr, this->deep, this->tout);
    HopsRes hops;
        hops.addr = ping.addr;
        hops.term = ping.term;
        hops.cerr = ping.cerr;

    if(ping.cerr == 0)
        { hops.hops = trace.hops(this->addr, this->deep, this->hush); }
    else
        { hops.hops = -1; }

    emit ready(hops.addr, this->name, hops.term, hops.cerr, hops.hops);
    emit finished();
}// run

/* TraceWrk. ******************************************************************/
/******************************************************************************/

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
TraceWrk::~TraceWrk() {
    FNC << "end";
}//~TraceWrk

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
TraceWrk::TraceWrk(
    const QString &addr, int deep, int tout, int hush
  , QObject *prnt
) : QObject(prnt), addr(addr), deep(deep), tout(tout), hush(hush) {}

// Поехали ! -------------------------------------------------------------------
//------------------------------------------------------------------------------
void TraceWrk::proc(void) {
    FNC << SYSDATE;

    PingRes ping = trace.ping(this->addr, this->deep, this->tout);
    HopsRes hops;
        hops.addr = ping.addr;
        hops.term = ping.term;
        hops.cerr = ping.cerr;

    if(ping.cerr == 0)
        { hops.hops = trace.hops(this->addr, this->deep, this->hush); }
    else
        { hops.hops = -1; }

    emit ready(hops);
    emit finished();
}// proc

/* TraceMgr. ******************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
TraceMgr::TraceMgr(QObject *prnt) : QObject(prnt) {

}// TraceMgr

// Многопоточная трассировка. --------------------------------------------------
//------------------------------------------------------------------------------
const Res& TraceMgr::proc(
    const char *xml, int deep, int tout, int hush, int thread_num )
{
    static const QString XML_ERR("Ошибка при разборке XML, причина: %1.");
    static const QString TSK_ERR("Ошибка описания задания, причина: %1.");
    static const QString ROOT = "root", ADDR = "addr";

    delete[] err_out; err_out = nullptr;
    delete this->que; this->que = new AtomicQueue<QPair<QString, QString>>;
    this->deep = deep; this->tout = tout; this->hush = hush; this->task = 0;

    // Разбор XML.
    QXmlStreamReader doc(xml);

    // Тег: ADDR.
    auto prsAddr = [&](void) -> void {
        if(doc.isStartElement() != true || doc.name() != ADDR) { return; }

        QString name = doc.readElementText();
        for(const QHostAddress &addr : QHostInfo::fromName(name).addresses()) {
            this->que->push(QPair<QString, QString>(addr.toString(), name));
            this->task++;
        }// addr

    };// prsAddr

    // Тег: ROOT.
    auto prsRoot = [&](void) -> void {
        if(doc.isStartElement() != true || doc.name() != ROOT) { return; }

        while(doc.atEnd() == false && doc.hasError() == false) {
            QXmlStreamReader::TokenType type = doc.readNext();
            if(type == QXmlStreamReader::StartDocument) { continue; }
            if(type == QXmlStreamReader::StartElement)
                { if(doc.name() == ADDR) { prsAddr(); }}
        }// while(doc.atEnd() == false && doc.hasError() == false)
    };// parseRoot

    // Разбор.
    if(doc.readNextStartElement() && doc.name() == ROOT) { prsRoot(); }
    if(doc.tokenType() == QXmlStreamReader::Invalid) { doc.readNext(); }

    // Обработка ошибок.
    if(doc.hasError()) {
        // todo: !!! ОБРАБОТАТЬ !!!
    }// if(xml.hasError())

    // Активация потоков.
    for(int i=0; i<thread_num; i++) {

        QPair<QString, QString> value;
        if(this->que->take(value)) {
            RUN(new TraceTrd(
                this, value.first, value.second
              , this->deep, this->tout, this->hush ));
        } else {
            break;
        }// else // if(this->que->take(addr))
    }// i

    // Дождаться сигнала готовности.
    WaitForSignal(this, SIGNAL(ready())).wait(10*MIN);
    return this->res;
}// proc

// Запустить трассировку. ------------------------------------------------------
//------------------------------------------------------------------------------
void TraceMgr::RUN(TraceTrd *trace) {
    connect(trace, &TraceTrd::ready, this, &TraceMgr::accept);
    connect(trace, &TraceTrd::finished, trace, &TraceTrd::quit);
    trace->start();
}// RUN

// Получить результат трассировки. ---------------------------------------------
//------------------------------------------------------------------------------
void TraceMgr::accept(QString addr, QString name, int term, int cerr, int hops){
    HopsRes rslt;
        rslt.addr = addr;
        rslt.cerr = cerr;
        rslt.term = term;
        rslt.hops = hops;

    this->res.append(QPair<QString, HopsRes>(name, rslt));
    this->task--;

    QPair<QString, QString> value;
    if(this->que->take(value)) {
        RUN(new TraceTrd(
            this, value.first, value.second
          , this->deep, this->tout, this->hush ));
    } else {
        if(this->task == 0) { emit ready(); }
    }// else // if(this->que->take(addr))

}// accept

//------------------------------------------------------------------------------


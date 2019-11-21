// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QTimer>
#include <QThread>
#include <QRegExp>

#include "env.h" // Глобальная среда приложения.
#include "std.h"
#include "lst.h"
#include "dbg.h"
#include "xml.h"
#include "tme.h"

#include "Event.h"
#include "MPhone.h"

// DEFINE. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#define BGN_WAIT(_SGL_) QEventLoop _LOOP_; \
        connect(this, _SGL_, &_LOOP_, &QEventLoop::quit);
#define END_WAIT _LOOP_.exec();

/* Дополнительные функции. ****************************************************/
/******************************************************************************/

// Выполнить список скриптов. --------------------------------------------------
//------------------------------------------------------------------------------
void EXE(QProcess *proc, const QList<QString> &scr) {
    for(const QString &command: scr) {
        proc->start(command);
        proc->waitForFinished(PROC_TIMEOUT);
        proc->waitForReadyRead(PROC_TIMEOUT);
    }// command
}// EXE

/* PhoneCsl. ******************************************************************/
/******************************************************************************/

// Обработчик запроса. ---------------------------------------------------------
//------------------------------------------------------------------------------
Result MPhone::wrk(Command command) {
    static auto STR = [](Status stt) {
        return (
            Status::BSY == stt ? "0"
          : Status::ERR == stt ? "1"
          : Status::RNG == stt ? "2"
          : Status::RDY == stt ? "3" : "1" ); };

    if(NOT(this->ready)) { return Result(); }
    FNC << "cmd:" << command.toString();

    // Маршрутизация.
    switch(this->cmd[command.name].toInt()) {

     // call_init
     case 1: return Result(
        command.ref, STR(call(command.p_list["target"]))
      , "try call_init" );

     // call_finish
     case 2: return Result(
        command.ref, STR(stop())
      , "try call_finish" );

     // call_status
     case 3: return Result(
        command.ref, STR(info())
      , "status" );

    }// switch(this->cmd[cmd].toInt())

    return Result();
}// req_worker

// Ошибка QProcess. ------------------------------------------------------------
//------------------------------------------------------------------------------
void MPhone::proc_err(QProcess::ProcessError err) { FNC << err; }

// Поток вывода ошибок. --------------------------------------------------------
//------------------------------------------------------------------------------
void MPhone::ready_err(void){
    E::Log->err(this->proc->readAllStandardError());
}// ready_err

// Поток вывода. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void MPhone::ready_out(void) {
    E::Log->inf(this->proc->readAllStandardOutput());
}// ready_out

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
MPhone::MPhone(const QString &cfg, QObject *prn) : QObject(prn), Module() {

    // Инициализация.
    this->init(cfg);
    proc = new QProcess();
    connect(proc, &QProcess::errorOccurred, this, &MPhone::proc_err);

}// PhoneCsl

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
MPhone::~MPhone(void) {
    delete proc;
}// ~PhoneCsl

// Задать конфигурацию. --------------------------------------------------------
//------------------------------------------------------------------------------
void MPhone::init(const QString &conf) {
    QString pfx(VAL("pfx", conf));

    this->open_scr = DCR(pfx + " ", LST("cmd", VAL("open", conf)));
    this->close_scr = DCR(pfx + " ", LST("cmd", VAL("close", conf)));

    this->info_scr = DCR(pfx + " ", LST("cmd", VAL("info", conf)));
    this->call_scr = DCR(pfx + " ", LST("cmd", VAL("call", conf)));
    this->stop_scr = DCR(pfx + " ", LST("cmd", VAL("stop", conf)));
}// init

// Открыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
void MPhone::open(const QString &conf) { init(conf); open(); }
void MPhone::open(void)
    { EXE(this->proc, this->open_scr); this->ready = true; }

// Закрыть устройство. ---------------------------------------------------------
//------------------------------------------------------------------------------
void MPhone::close(void)
    { EXE(this->proc, this->close_scr); this->ready = false; }

// Запросить информацию о состоянии. -------------------------------------------
//------------------------------------------------------------------------------
MPhone::Status MPhone::info() {
    static QRegExp RNG(R"(hook=ringing.*)");
    static QRegExp BSY(R"(Call out.*)");
    static QRegExp RDY(R"(hook=on-hook.*)");

    this->proc->start(this->info_scr.first());
    this->proc->waitForFinished(PROC_TIMEOUT);
    this->proc->waitForReadyRead(PROC_TIMEOUT);

    this->last_out = this->proc->readAllStandardOutput();
    this->last_err = this->proc->readAllStandardError();

    FNC << "info.out:" << this->last_out;

    if(RNG.exactMatch(this->last_out)) { return Status::RNG; }
    if(BSY.exactMatch(this->last_out)) { return Status::BSY; }
    if(RDY.exactMatch(this->last_out)) { return Status::RDY; }

    return Status::ERR;
}// info

// Выполнить звонок. -----------------------------------------------------------
//------------------------------------------------------------------------------
MPhone::Status MPhone::call(QString num) {
    Status ret = this->info();

    if(Status::RDY == ret) {
        this->proc->start(this->call_scr.first().replace("{NUM}", num));
        this->proc->waitForFinished(PROC_TIMEOUT);

        BGN_WAIT(&MPhone::sgl_wait_ok)
        QTimer::singleShot(CHECK_TIMEOUT, [this, &ret]
            { STAT(CHECK_COUNT, Status::RDY, Status::RNG, &ret); });
        END_WAIT

        // Запуск прослушивания события прерывания вызова.
        QTimer::singleShot(CHECK_TIMEOUT, [this, num] {
            BGN_WAIT(&MPhone::sgl_wait_ok)
            MPhone::Status stt;
            STAT(CHECK_COUNT, Status::RDY, Status::BSY, &stt);
            END_WAIT

            E::Main->evt_send(Event(
                "call_finish", {{"call_to" , num}, {"reason", "call stop"}} ));
        });// QTimer::singleShot

    }// if(Status::RDY == ret)

    return ret;
}// call

// Проверка состояния по таймеру (возврат через ret_stat). ---------------------
//------------------------------------------------------------------------------
void MPhone::STAT(int cnt, Status tgt, Status cng, Status *ret_stat) {
    (*ret_stat) = this->info();

    if(cng != (*ret_stat) || tgt == (*ret_stat) || cnt == 0)
        { emit this->sgl_wait_ok(); return; }

    QTimer::singleShot(CHECK_TIMEOUT, [=]
        { this->STAT(cnt-1, tgt, cng, ret_stat); });

}// STAT

// Прервать звонок. ------------------------------------------------------------
//------------------------------------------------------------------------------
MPhone::Status MPhone::stop() {
    EXE(this->proc, this->stop_scr);
    return this->info();
}// stop

//------------------------------------------------------------------------------



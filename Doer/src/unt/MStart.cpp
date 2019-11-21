// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include "env.h" // Глобальная среда приложения.
#include "std.h"
#include "dbg.h"
#include "xml.h"
#include "lst.h"

#include "MStart.h"

// DEFINE ----------------------------------------------------------------------
//------------------------------------------------------------------------------
#define LG E::log
#define INF(m) if(LG) { LG->add("START-INF", STR(SYSDATE) + STR(": ") + m); }
#define ERR(m) if(LG) { LG->add("START-ERR", STR(SYSDATE) + STR(": ") + m); }
#define FCE(m) E::Log->inf(STR(SYSDATE) + ": " + m)

/* Proc. **********************************************************************/
/******************************************************************************/

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
Proc::~Proc(void) { DBG << "end"; }

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
Proc::Proc(const QString &cfg, QObject *prn ) : QObject(prn) {

    // Инициализация.
    this->nme = VAL("name", cfg);
    this->tmr_chk.setInterval(
        [](int val, int dft) { return val == 0 ? dft : val; }
        (VAL("itv", VAL("check", cfg)).asINT, -1)); // -1: таймер не стартует
    this->cst.setPattern(VAL("cmd", VAL("check", cfg)));
    this->chk = VAL("cmd", VAL("check", cfg));

    this->opn = LST("cmd", VAL("open", cfg));
    this->cls = LST("cmd", VAL("close", cfg));

    // Подключение сигналов/слотов.
    connect(&proc, &QProcess::errorOccurred, this, &Proc::proc_err);

}// Proc

// Обработка ошибок. -----------------------------------------------------------
//------------------------------------------------------------------------------
void Proc::proc_err(QProcess::ProcessError err) {
    static QString msg("Процесс: %1 вызвал ошибку: 2%");

    ERR(msg.arg(this->nme).arg(QString(this->proc.readAllStandardError())));
    FCE(msg.arg(this->nme).arg(QString(this->proc.readAllStandardError())));
    DBG << "err:" << err;
}// proc_err

// Периодическая проверка работоспособности. -----------------------------------
//------------------------------------------------------------------------------
void Proc::on_check(void) {

    if(this->chk.isEmpty() || this->cst.isEmpty()) {
        if(state() == QProcess::ProcessState::NotRunning) { open(); }
    } else {
        EXE({this->chk});

        // Проверка на корректный отклик.
        if(NOT(this->cst.exactMatch(this->proc.readAllStandardOutput()))) {

            // Попытка корректно закрыть.
            if(state() != QProcess::ProcessState::NotRunning)
                { close(); this->proc.waitForFinished(PROC_TIMEOUT); }

            // Попытка мягко прервать.
            if(state() != QProcess::ProcessState::NotRunning)
                { terminate(); this->proc.waitForFinished(PROC_TIMEOUT); }

            // Жёстко прервать.
            if(state() != QProcess::ProcessState::NotRunning)
                { kill(); this->proc.waitForFinished(PROC_TIMEOUT); }

            open();
        }// if(NOT(this->cst.exactMatch(this->proc.readAllStandardOutput())))
    }

}// on_check

// Запустить процесс. ----------------------------------------------------------
//------------------------------------------------------------------------------
void Proc::open(void) { EXE(this->opn); tmr_chk.start(); }

// Завершить процесс. ----------------------------------------------------------
//------------------------------------------------------------------------------
void Proc::close(void) { tmr_chk.stop(); EXE(this->cls); }

// Попытка мягко прервать. -----------------------------------------------------
//------------------------------------------------------------------------------
void Proc::terminate(void) { this->proc.terminate(); }

// Принудительно остановить процесс. -------------------------------------------
//------------------------------------------------------------------------------
void Proc::kill(void) { this->proc.kill(); }

// Выполнить набор скриптов. ---------------------------------------------------
//------------------------------------------------------------------------------
void Proc::EXE(const QList<QString> &scr) {
    for(const QString &command: scr) {
        if(command.isEmpty()) { continue; }
        this->proc.start(command);
        this->proc.waitForReadyRead(PROC_TIMEOUT);
    }// command
}// EXE

//------------------------------------------------------------------------------


/* MStart. ********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
MStart::MStart(const QString &cfg, QObject *prn) : QObject(prn), Module() {
    init(cfg);

}// MStart

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
MStart::~MStart(void) {
    DEL(this->prc);

}//~MStart

// Обработчик команд. ----------------------------------------------------------
//------------------------------------------------------------------------------
Result MStart::wrk(Command command) {
    if(NOT(this->ready)) { return Result(); }

    // Маршрутизация.
    switch(this->cmd[command.name].toInt()) {

     // proc_list
     case 1: return Result();

     // proc_info
     case 2: return Result();

     // proc_start
     case 3: return Result();

     // proc_stop
     case 4: return Result();

     // proc_start_all
     case 5: return Result();

     // proc_stop_all
     case 6: return Result();

    }// switch(this->cmd[cmd].toInt())

    return Result();
}// wrk

// Задать конфигурацию. --------------------------------------------------------
//------------------------------------------------------------------------------
void MStart::init(const QString &cfg) {
    DEL(this->prc);
    for(const QString &process_cfg : LST("proc", cfg))
        { this->prc.append(new Proc(process_cfg)); }
}// init

// Запустить службу. -----------------------------------------------------------
//------------------------------------------------------------------------------
void MStart::open(void) {
    for(Proc *process : this->prc) { process->open(); }
    this->ready = true;
}// open

// Остановить службу. ----------------------------------------------------------
//------------------------------------------------------------------------------
void MStart::close(void) {
    this->ready = false;
    for(Proc *process : this->prc) { process->close(); }
}// close

//------------------------------------------------------------------------------



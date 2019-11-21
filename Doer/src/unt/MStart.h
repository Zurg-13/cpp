#ifndef MSTART_H
#define MSTART_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QObject>
#include <QProcess>
#include <QRegExp>
#include <QTimer>

#include "tme.h"

#include "Module.h"


/* Контролируемая утилита. ****************************************************/
/******************************************************************************/
class Proc : public QObject {
    Q_OBJECT
    const int PROC_TIMEOUT = 30*SEC;

 public:
    explicit Proc(const QString &cfg, QObject *prn = nullptr);
            ~Proc(void);

    void open(void);
    void close(void);
    void terminate(void);
    void kill(void);

    QProcess::ProcessState state(void) { return this->proc.state(); }
    const QString& name(void) { return nme; }

 private slots:
   void proc_err(QProcess::ProcessError err);
   void on_check(void);

 private:
   QProcess proc;
   QTimer tmr_chk;
   QString chk, nme;
   QRegExp cst;
   QStringList opn, cls;

   void EXE(const QList<QString> &scr);
};// Proc


/* Запуск утилит. *************************************************************/
/******************************************************************************/
class MStart : public QObject, public Module  {
    Q_OBJECT

 public:
    const QMap<QString, QVariant> cmd = {
        {"proc_list", 1}, {"proc_info", 2}
      , {"proc_start", 3}, {"proc_stop", 4}
      , {"proc_start_all", 5}, {"proc_stop_all", 6} };

    explicit MStart(const QString &cfg, QObject *prn = nullptr);
            ~MStart(void) override;

    Result wrk(Command command) override;  // Обработчик команд.
    bool   rdy(QString &cmd_name) override
        { return ready && cmd.contains(cmd_name); }

    void init(const QString &cfg);
    void open(const QString &cfg) { init(cfg); open(); }
    void open(void);
    void close(void);


 private:
    QList<Proc*> prc;

};// MStart


//------------------------------------------------------------------------------
#endif // MSTART_H

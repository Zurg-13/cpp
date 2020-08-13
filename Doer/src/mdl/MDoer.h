#ifndef MDOER_H
#define MDOER_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QObject>

#include "unt/Module.h"

/* Модуль управления (служебный). *********************************************/
/******************************************************************************/
class MDoer : public QObject, public Module {
    Q_OBJECT

 public:
    const QString name = "doer";
    const QMap<QString, int> cmd = {
        {"version", 1}, {"get_log", 2} };

    MDoer(const QString &conf, QObject *prnt = nullptr);
   ~MDoer(void) override;

    void wrk(Command cmnd) override;  // Обработчик команд.
    bool rdy(QString &cmd_name) override
        { return ready && cmd.contains(cmd_name); }
    const QString& nme() override { return name; }

    void init(const QString &conf);
    bool open(void);
    void close(void);

    void version(const Command &cmnd);
    void get_log(const Command &cmnd);

};// MDoer

//------------------------------------------------------------------------------
#endif // MDOER_H

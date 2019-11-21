#ifndef MODULE_H
#define MODULE_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "ZLogger.h"
#include "Result.h"
#include "Command.h"

/* Интерфейс модуля. **********************************************************/
/******************************************************************************/
class Module {

 public:
    explicit Module(void) {}
    virtual ~Module();

    virtual Result wrk(Command command) = 0;
    virtual bool   rdy(QString &cmd_name) = 0;


 protected:
    bool ready = false;

};// Module

// TYPEDEF. --------------------------------------------------------------------
//------------------------------------------------------------------------------
//typedef  Result (Module::*FS_cmd_worker) (const Command &cmd);

//------------------------------------------------------------------------------
#endif // MODULE_H

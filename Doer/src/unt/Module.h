#ifndef MODULE_H
#define MODULE_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMap>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "ZLogger.h"
#include "Result.h"
#include "Command.h"

// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define DONE QString("0")
#define FAIL QString("1")
#define NONE QString("9")

#define SEND(a) E::Main->send(a)

// Дополнительные функции. -----------------------------------------------------
//------------------------------------------------------------------------------
void TSS(const int msec, const std::function<void()> l);
void TSS(const std::function<void()> l);

// todo: переписать модули на взаимодействие через сигналы/слоты
/* Интерфейс модуля. **********************************************************/
/******************************************************************************/
class Module {

 public:
    explicit Module(void) {}
    virtual ~Module();

    virtual void wrk(Command command) = 0;
    virtual bool rdy(QString &cmd_name) = 0;
    virtual const QString& nme(void) = 0;

 protected:
    bool ready = false;

};// Module

//------------------------------------------------------------------------------
#endif // MODULE_H

#ifndef RESULT_H
#define RESULT_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QMap>
#include <QSharedPointer>

#include "Event.h"
#include "Command.h"

/* Формирование результата команды. *******************************************/
/******************************************************************************/
class Result {

 public:
    Result(void): has_result(false) {}
    Result(const Command &cmd, const QString &status, const QString &desc)
        : Result(cmd.ref, status, desc) {}
    Result(const QString &ref, const QString &status, const QString &desc)
        : has_result(true), ref(ref), status(status), desc(desc) {}
    Result(
        const QString &ref, const QString &status, const QString &desc
      , const QMap<QString, QString> &prm
    )   : has_result(true), ref(ref), status(status), desc(desc), p_list(prm) {}


    bool hasResult(void) { return has_result; }
    QSharedPointer<QByteArray> xml(const QString &sgn, const QString &pid);
    QString toString(void);
    Event toEVT(const QString &name) { return Event(name, p_list); }

 private:
    bool has_result;
    QString ref, status, desc;
    QMap<QString, QString> p_list;

};// Result

/*
### результат ###
element result {
    ## связка команды с ответом. Для event не указывается
    element ref { text }

  , ## "0" - успех, != 0 - неуспех, зависит от команды
    element status { text }

  , ## текстовое описание, чаще всего для неуспеха
    element desc { text }

  , element p_list {
        element p {

            attribute key {text}
          , ## значение параметра
            text
        }
    }
}
*/

//------------------------------------------------------------------------------
#endif // RESULT_H

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
    Result(const QString &stts, const QString &desc)
      : has_result(false), status(stts), desc(desc) {}
    Result(
        const QString &ref, const QString &module
      , const QString &status, const QString &desc
      , const QMap<QString, QString> &prm = {}
    ) : has_result(true), ref(ref), module(module)
      , status(status), desc(desc), p_list(prm) {}
    Result(
        const Command &cmnd
      , const QString &status, const QString &desc
      , const QMap<QString, QString> &prm = {}
    ) : Result(cmnd.ref, cmnd.module, status, desc, prm) {}
    Result(const Command &cmnd, const Result &rslt)
      : Result(cmnd, rslt.status, rslt.desc, rslt.p_list) {}

    explicit Result(const QString &xml);

    bool hasResult(void) const { return has_result; }
    QSharedPointer<QByteArray> xml(const QString &sgn, const QString &pid)const;
    QString toString(void) const;
    Event toEVT(const QString &name) const
        { return Event(name, module, p_list); }

    void setRef(const QString &ref) { this->ref = ref; }
    bool equStatus(const QString &status) const
        { return this->status == status; }

    static Result load(const QString &fnme);
    void          save(const QString &fnme) const;

 private:
    bool has_result;
    QString ref, module, status, desc;
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

#ifndef RESULT_H
#define RESULT_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QMap>

/* Формирование результата команды. *******************************************/
/******************************************************************************/
class Result {

 public:
    Result(void): has_result(false) {}
    Result(const QString &ref, const QString &status, const QString &desc)
        : has_result(true), ref(ref), status(status), desc(desc) {}
    Result(
        const QString &ref, const QString &status, const QString &desc
      , const QMap<QString, QString> &prm
    )   : has_result(true), ref(ref), status(status), desc(desc), p_list(prm) {}


    bool hasResult(void) { return has_result; }
    QByteArray xml(const QString &sgn);
    QString toString(void);

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

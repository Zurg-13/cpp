// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringBuilder>

#include "xml.h"
#include "lst.h"

#include "Result.h"


// Сформировать XML-ответ. -----------------------------------------------------
//------------------------------------------------------------------------------
QSharedPointer<QByteArray> Result::xml(const QString &sgn, const QString &pid) {
    return QSharedPointer<QByteArray>(new QByteArray(
        TAG("result",
            TAG("sgn", sgn) + TAG("pid", pid)
          + TAG("ref", this->ref)
          + TAG("status", this->status)
          + TAG("desc", this->desc)
          + (this->p_list.isEmpty()
              ? TAG("p_list")
              : TAG("p_list", TAG("p", "key", this->p_list)) )
    ).toUtf8() ));
}// xml

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString Result::toString(void) {
    return QString("Result(%1)").arg(this->has_result ? "yes": "not") % "{"
      % BLD({
            PR("ref", this->ref)
          , PR("status", this->status)
          , PR("desc", this->desc)
          , PR("p_list", "{" % BLD(LST(this->p_list)) % "}")
        }) % "}";
}// toString

//------------------------------------------------------------------------------


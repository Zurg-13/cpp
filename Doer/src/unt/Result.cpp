// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringBuilder>
#include <QFileInfo>

#include "xml.h"
#include "lst.h"
#include "std.h"

#include "Module.h"
#include "Result.h"

// Конструктор (из XML). -------------------------------------------------------
//------------------------------------------------------------------------------
Result::Result(const QString &xml) : has_result(true) {
    const QStringRef result(VAL("result", &xml));
    this->ref    = VAL("ref", result.string()).asSTR;
    this->module = VAL("module", result.string()).asSTR;
    this->status = VAL("status", result.string()).asSTR;
    this->desc   = VAL("desc", result.string()).asSTR;
    this->p_list = MAP("p", "key", CUT("p_list", result.string()).asSTR);
}// Result

// Сформировать XML-ответ. -----------------------------------------------------
//------------------------------------------------------------------------------
QSharedPointer<QByteArray> Result::xml(
    const QString &sgn, const QString &pid) const
{
    return QSharedPointer<QByteArray>(new QByteArray(
        TAG("result",
            TAG("sgn", sgn) % TAG("pid", pid)
          % TAG("ref", this->ref) % TAG("module", this->module)
          % TAG("status", this->status)
          % TAG("desc", this->desc)
          % (this->p_list.isEmpty()
              ? TAG("p_list")
              : TAG("p_list", TAG("p", "key", this->p_list)) )
    ).toUtf8() ));
}// xml

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString Result::toString(void) const {
    return QString("Result(%1)").arg(this->has_result ? "yes": "not") % "{"
      % BLD({
            PR("ref", this->ref), PR("module", this->module)
          , PR("status", this->status), PR("desc", this->desc)
          , PR("p_list", "{" % BLD(LST(this->p_list)) % "}")
        }) % "}";
}// toString

// Загрузить из файла. ---------------------------------------------------------
//------------------------------------------------------------------------------
Result Result::load(const QString &fnme) {
    return ( QFileInfo(fnme).exists()
        ? Result(FLE(fnme)) : Result(NONE, "file not found") );
}// load

// Сохранить в файл. -----------------------------------------------------------
//------------------------------------------------------------------------------
void Result::save(const QString &fnme) const
    { SVE(fnme) << QString::fromUtf8(*xml("NOT", "NOT").data()); }

//------------------------------------------------------------------------------


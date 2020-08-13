// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringBuilder>

#include "xml.h"
#include "lst.h"

#include "Command.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
Command::Command(const QString& xml) {
    this->ref    = VAL("ref", xml);
    this->action = VAL("action", xml); this->module = VAL("module", xml);
    this->p_list = MAP("p", "key", CUT("p_list", &xml).toString());
}// Command

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString Command::toString(void) {
    return QString("Command") % "{" % BLD({
        PR("ref", this->ref)
      , PR("action", this->action)
      , PR("module", this->module)
      , PR("p_list", "{" % BLD(LST(this->p_list)) % "}")
    }) % "}";
}// toString

//------------------------------------------------------------------------------


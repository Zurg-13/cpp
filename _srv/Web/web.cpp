// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "web.h"


// Оформить http-адрес. --------------------------------------------------------
//------------------------------------------------------------------------------
QString SURROUND(QString url) {
    return QString("http://").append(url).append("?data=");
}// SURROUND


//------------------------------------------------------------------------------

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "Fasten.h"

/* Fasten. ********************************************************************/
/******************************************************************************/

// Считать. --------------------------------------------------------------------
//------------------------------------------------------------------------------
QByteArray Fasten::read(QFile &fle) {

    if(fle.open(QIODevice::ReadOnly)) {
        qint64 pos;

        fle.seek(fle.size() - sizeof(pos));
        fle.read((char*)&pos, sizeof(pos));
        fle.seek(pos);
        QByteArray ret(fle.read(fle.size() - pos - sizeof(pos)));

        fle.close();
        return ret;
    }// if(fle.open(QIODevice::ReadOnly))

    return QByteArray();
}// read

// Пристегнуть. ----------------------------------------------------------------
//------------------------------------------------------------------------------
bool Fasten::write(QFile &fle, QByteArray &arr) {

    if(fle.open(QIODevice::Append)) {
        qint64 pos = fle.size();

        fle.write(arr);
        fle.write((char*)&pos, sizeof(pos));

        fle.close();
        return true;
    }// if(fle.open(QIODevice::Append))

    return false;
}// write

//------------------------------------------------------------------------------


// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QStringBuilder>

#include "lst.h"

// Декорировать каждый элемент списка. -----------------------------------------
//------------------------------------------------------------------------------
QList<QString> DCR(const QString &pfx, const QList<QString> &lst)
    { return DCR(pfx, lst, QString()); }
QList<QString> DCR(const QList<QString> &lst, const QString &pst)
    { return DCR(QString(), lst, pst); }
QList<QString> DCR(
    const QString &pfx, const QList<QString> &lst, const QString &pst )
{
    QList<QString> ret;
    for(const QString &string: lst) { ret.append(pfx % string % pst); }
    return ret;
}// DCR

// Собрать строку из списка элементов. -----------------------------------------
//------------------------------------------------------------------------------
QString BLD(const QList<QString> &lst) {
    static QString SEP(", ");
    auto CST = [](const QString &val) { return QString(val); };
    return BLD<QString, decltype(CST)>(lst, CST, SEP );
}// BLD

QString BLD(const QList<QPair<QString, QString>> &lst) {
    static QString SEP(", ");
    auto CST = [](const QPair<QString, QString> &pr)
        -> QString { return  pr.first + "=" + pr.second; };
    return BLD<QPair<QString, QString>, decltype(CST)>(lst, CST, SEP);
}// BLD

//------------------------------------------------------------------------------

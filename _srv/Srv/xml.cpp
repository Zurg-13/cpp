// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "xml.h"
#include "std.h"

// Извлечь значение тега. ------------------------------------------------------
//------------------------------------------------------------------------------
QString VAL(const QString tag, const QString xml) {
    QString opn = "<" + tag + ">", cls = "</" + tag +">";
    int bgn = xml.indexOf(opn) + opn.length();
    int end = xml.lastIndexOf(cls);

    return bgn >= 0 && end > 0 && bgn < end
        ? xml.mid(bgn, end - bgn)
        : EMPTY_STR;
}// VAL

// Собрать XML-тег. ------------------------------------------------------------
//------------------------------------------------------------------------------
QString TAG(QString tag, QString val) {
    return "<" + tag + ">" + val + "</" + tag + ">";
}// TAG

QString TAG(QString tag, QString attr, QString val) {
    return "<" + tag + " " + attr + ">" + val + "</" + tag + ">";
}// TAG

// Интерпретировать содержимое, как список тегов. ------------------------------
//------------------------------------------------------------------------------
QList<QString> LST(QString tag, QString xml) {
    QList<QString> ret;
    QString opn = "<" + tag + ">", cls = "</" + tag +">";

    int bgn = xml.indexOf(opn) + opn.length();
    int end = xml.indexOf(cls, bgn );
    QString res = xml.mid(bgn, end - bgn).trimmed();
    while(bgn >= 0 && end > 0) {
        ret.append(res);

        bgn = xml.indexOf(opn, end + cls.length());

        if(bgn >= 0) bgn += opn.length();
        else break;

        end = xml.indexOf(cls, bgn);
        res = xml.mid(bgn, end - bgn).trimmed();
    }// while(QString res = xml.mid(bgn, end))

    return ret;
}// LST

// Подмена символов: & = &amp, < = &lt, > = &gt. ---------------------------
//--------------------------------------------------------------------------
QString ESCPG(QString val) {
    return val
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;");
}// ESCPG

//------------------------------------------------------------------------------

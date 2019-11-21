// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QXmlStreamReader>
#include <QStringBuilder>

#include "xml.h"
#include "std.h"

// Извлечь значение тега. ------------------------------------------------------
//------------------------------------------------------------------------------
QString VAL(const QString &tag, const QString &xml) {
    QString opn = "<" % tag % ">", cls = "</" % tag % ">";
    int bgn = xml.indexOf(opn) + opn.length();
    int end = xml.lastIndexOf(cls);

    return bgn >= 0 && end > 0 && bgn < end
        ? xml.mid(bgn, end - bgn)
        : EMPTY_STR;
}// VAL

// Собрать XML-тег. ------------------------------------------------------------
//------------------------------------------------------------------------------
QString TAG(const QString &tag)
    { return "<" % tag % "/"; }
QString TAG(const QString &tag, const QString &val)
    { return "<" % tag % ">" % val % "</" % tag % ">"; }
QString TAG(const QString &tag, const QString &attr, const QString &val)
    { return "<" % tag % " " % attr % ">" % val % "</" % tag % ">"; }
QString TAG(const QString &tag, const QString &key, const StringMap &map) {
    QStringList bld;
    QString opn = "<" % tag % " " % key % R"(="%1")" % ">";
    QString cls = "</" % tag % ">";

    for(const QString &key: map.keys())
        { bld.append(opn.arg(key) % map[key] % cls); }

    return bld.join(QString());
}// TAG

// Интерпретировать содержимое, как список тегов. ------------------------------
//------------------------------------------------------------------------------
QList<QString> LST(const QString &tag, const QString &xml) {
    QList<QString> ret;
    QString opn = "<" % tag % ">", cls = "</" % tag % ">";

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

// Интерпретировать содержимое, как словарь. -----------------------------------
//------------------------------------------------------------------------------
QMap<QString, QString> MAP(
    const QString &tag, const QString &key, const QString &xml)
{
    QMap<QString, QString> ret;

    QXmlStreamReader doc(xml);
    while(doc.atEnd() == false || doc.hasError() == false) {
        QXmlStreamReader::TokenType token = doc.readNext();
        if(token == QXmlStreamReader::StartElement && doc.name() == tag) {
            QXmlStreamAttributes attr = doc.attributes();
            ret[attr.value(key).toString()] = doc.readElementText();
        }// if(token == QXmlStreamReader::StartElement && doc.name() == tag)
    }// while(reader.atEnd() == false || reader.hasError() == false)

    return ret;
}// MAP

// Подмена символов: & == &amp; < == &lt; > == &gt; ----------------------------
//------------------------------------------------------------------------------
QString ESCPG(QString val) {
    return val
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;");
}// ESCPG

QString UNESC(const QString &txt) {
    QStringList bld;
    static QChar AMP = '&', SCL = ';';
    static QMap<QString, QString> dec = {
        {"&lt;", "<"}, {"&gt;", ">"}
      , {"&amp;", "&"}, {"&quot;", R"(")"}, {"&#039;", "'"} };

    if(NOT(txt.contains(AMP))) { return txt; }

    int bgn = 0, pos = 0;
    while((pos = txt.indexOf(AMP, pos)) != -1) {
        int end = txt.indexOf(SCL, pos)+1;
        QString val = dec[txt.mid(pos, end - pos)];

        bld << txt.mid(bgn, pos - bgn);

        if(val.isEmpty()) {
            end = txt.indexOf(AMP, pos+1);
            bld << txt.mid(pos, end - pos);
        } else {
            bld << val;
        }// else // if(val.isEmpty())

        bgn = end; pos = end;
    }// while((pos = txt.indexOf(AMP, pos)) != -1)

    return bld.join(QString());
}// UNESC


//------------------------------------------------------------------------------

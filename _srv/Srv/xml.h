#ifndef XML_H
#define XML_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QList>
#include <QMap>

// TYPEDEF. --------------------------------------------------------------------
//------------------------------------------------------------------------------
typedef QMap<QString, QString> StringMap;


/* Библиотека работы с XML. ***************************************************/
/******************************************************************************/

// Проверить наличие тега. -----------------------------------------------------
//------------------------------------------------------------------------------
bool EXIST(const QString &tag, const QString &xml);

// Извлечь содержимое тега. ----------------------------------------------------
//------------------------------------------------------------------------------
QString VAL(const QString &tag, const QString &xml);

// Собрать XML-тег. ------------------------------------------------------------
//------------------------------------------------------------------------------
QString TAG(const QString &tag);
QString TAG(const QString &tag, const QString &val);
QString TAG(const QString &tag, const QString &attr, const QString &val);
QString TAG(const QString &tag, const QString &key, const StringMap &map);

// Интерпретировать содержимое, как список тегов. ------------------------------
//------------------------------------------------------------------------------
QList<QString> LST(const QString &tag, const QString &xml);
QMap<QString, QString> MAP(
    const QString &tag, const QString &key, const QString &xml );

// Подмена символов: & = &amp, < = &lt, > = &gt. -------------------------------
//------------------------------------------------------------------------------
QString  ESCPG(const QString &xml);
QString  UNESC(const QString &txt);


//------------------------------------------------------------------------------
#endif // XML_H

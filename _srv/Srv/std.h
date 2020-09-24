#ifndef STD_H
#define STD_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QApplication>
#include <QDesktopWidget>

/* Константы. *****************************************************************/
/******************************************************************************/
const QString EMPTY_STR("");


/* Макросы ********************************************************************/
/******************************************************************************/
#define APP_DIR QApplication::applicationDirPath()
#define SENDER(T) qobject_cast<T>(sender())

#define asID toULongLong()
#define asDT toDateTime()
#define asVRT toVariant()
#define asSTR toString()
#define asINT toInt()
#define asFLT toFloat()
#define asDBL toDouble()
#define asUTF toUtf8()
#define asTXT text().trimmed()

#define FCS(c) do { c->selectAll(); c->setFocus(); } while(false)

#define FIRST_RUN ([] {                     \
    static bool is_first_time = true;       \
    bool was_first_time = is_first_time;    \
    is_first_time = false;                  \
    return was_first_time; } ())

/* Служебные функции. *********************************************************/
/******************************************************************************/

// Сращивание через разлелитель. -----------------------------------------------
//------------------------------------------------------------------------------
QString FSN(QString lft, QString rgt, QString sep = "/");

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString STR(int val);
QString STR(const char* val);

// Приведение к целому. --------------------------------------------------------
//------------------------------------------------------------------------------
int INT(const QString &val);
int INT(const double &val);

// Проверка на пустое значение. ------------------------------------------------
//------------------------------------------------------------------------------
QString NVL(QString val, QString dft);

// НЕ. -------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool NOT(bool val);

// Значение по умолчанию. ------------------------------------------------------
//------------------------------------------------------------------------------
QString DFT(QString val, QString dft);

// Чтение файла. ---------------------------------------------------------------
//------------------------------------------------------------------------------
QString FLE(QString fnme);

// Сохранить файл. { SVE(fnme) << txt; } ---------------------------------------
//------------------------------------------------------------------------------
class SVE {

 public:
    explicit SVE(const QString &fnme) : file(fnme), strm(&file)
        { file.open(QIODevice::WriteOnly | QIODevice::Text); }
    QTextStream& operator << (const QString &str)
        { strm << str; return strm; }

 private:
    QFile file;
    QTextStream strm;

};// SVE


//------------------------------------------------------------------------------
#endif // STD_H

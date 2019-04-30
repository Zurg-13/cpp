#ifndef STD_H
#define STD_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QApplication>
#include <QDesktopWidget>

/* Константы. *****************************************************************/
/******************************************************************************/
const QString EMPTY_STR("");


/* Макросы ********************************************************************/
/******************************************************************************/
#define APP_DIR QApplication::applicationDirPath()

#define asID toULongLong()
#define asDT toDateTime()
#define asSTR toString()
#define asINT toInt()
#define asFLT toFloat()
#define asDBL toDouble()
#define asUTF toUtf8()
#define asTXT text().trimmed()

#define FCS(c) do { c->selectAll(); c->setFocus(); } while(false)
#define SELECT_ROW(v, r) do { v->selectRow(r); v->scrollTo(v->currentIndex()); } while(false)
#define TO_CENTER(r) this->move(r.center() - this->rect().center())
#define DESKTOP qApp->desktop()->availableGeometry(this)

#define ACCEPT QDialog::Accepted
#define REJECT QDialog::Rejected

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
//int INT(QString val);

// Проверка на пустое значение. ------------------------------------------------
//------------------------------------------------------------------------------
QString NVL(QString val, QString dft);

// НЕ. -------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool NOT(bool val);

// Значение по умолчанию. ------------------------------------------------------
//------------------------------------------------------------------------------
QString DFT(QString val, QString dft);

//------------------------------------------------------------------------------
#endif // STD_H

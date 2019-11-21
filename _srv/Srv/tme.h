#ifndef TME_H
#define TME_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QDateTime>


/* Библиотека работы с временем. **********************************************/
/******************************************************************************/
#define SYSDATE QDateTime::currentDateTime()
#define BGN_DAY QTime(0, 0)
#define END_DAY QTime(23, 59, 59, 999)
#define ONE_SECOND_VAL 1.15740741118012E-5
#define ONE_MINUTE_VAL 0.000694444446708076

#define TME_STD_FMT QString("yyyy-MM-dd HH:mm:ss")
#define TME_DTL_FMT QString("yyyy-MM-dd HH:mm:ss.zzz")
#define TME_SVE_FMT QString("yyyy-MM-dd_HHmmss_zzz")
#define TME_CMP_FMT QString("yyyyMMddHHmmsszzz")
#define DTE_DOT_FMT QString("dd.MM.yyyy")
#define TME_DOT_FMT QString("dd.MM.yyyy hh:mm")

const unsigned int MSC = 1;
const unsigned int SEC = 1000;
const unsigned int MIN = 60 * SEC;
const unsigned int HRS = 60 * MIN;
const unsigned int DAY = 24 * HRS;

// Приведение миллисекунд. -----------------------------------------------------
//------------------------------------------------------------------------------
unsigned int SS(unsigned int mills);
unsigned int MI(unsigned int mills);
unsigned int HH(unsigned int mills);
unsigned int DD(unsigned int mills);

// Приведение к строке. --------------------------------------------------------
//------------------------------------------------------------------------------
QString STR(QDateTime val);

//------------------------------------------------------------------------------
#endif // TME_H

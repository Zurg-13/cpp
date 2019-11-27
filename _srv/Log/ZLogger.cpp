// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>
#include <QDateTime>
#include "ZLogger.h"

#include <QDebug>

// Макросы. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#define STR(s) QString(s)
#define TME QDateTime::currentDateTime().toString("yyyy.MM.ddTHH-mm-ss")

// Константы. ------------------------------------------------------------------
//------------------------------------------------------------------------------
const QByteArray SEP(QString(": ").toUtf8());
const QByteArray END(QString("\n").toUtf8());
const QString LFLE("_%1.log");
const QString PRX("*.*.*T*-*-*");

/* SimpleLogger ***************************************************************/
/******************************************************************************/

// Предоставить имя файла. -----------------------------------------------------
//------------------------------------------------------------------------------
QString ZLogger::giveFileName(void) {
    static QStringList flt = (QStringList() << PRX + LFLE.arg(this->name));
    auto NME = [this]() -> QString { return TME + LFLE.arg(name); };
    QStringList all = this->dir.entryList(flt, QDir::Files);

    all.sort();
    if(all.isEmpty())
        { return NME(); }

    int num = all.length() - static_cast<int>(this->page_quan);
    qint64 sze = QFile(this->dir.filePath(all.last())).size();
    int cnt = (sze > this->page_size ? -1 : 0);

    for(const QString& file: all) {
        if(cnt < num) { QFile::remove(dir.filePath(file)); }
        cnt++;
    }// info

    if(sze > this->page_size)
        { return NME(); }

    return all.last();
}// giveFileName

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
ZLogger::ZLogger(
    const QDir &dir, const QString &name
  , unsigned int page_size, unsigned  int page_quan )
:   dir(dir), name(name), page_size(page_size), page_quan(page_quan)
{
    file = new QFile(dir.filePath(giveFileName()));
    if(!dir.exists()) { this->dir.mkdir(dir.path()); }

    file->open(QIODevice::Append);
}// Logger

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
ZLogger::~ZLogger() {
    delete file;
}//~ZLogger

// Добавить запись в лог. ------------------------------------------------------
//------------------------------------------------------------------------------
void ZLogger::add(const QString &type, const QString &msg) {
    file->write(type.toUtf8()); file->write(SEP);
    file->write(msg.toUtf8()); file->write(END);

    if(file->size() > this->page_size) {
        file->close();
        file->setFileName(dir.filePath(giveFileName()));
        file->open(QIODevice::Append);
    }// if(fle->size() > this->sze)
}// add

//------------------------------------------------------------------------------

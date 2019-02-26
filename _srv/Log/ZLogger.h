#ifndef ZLOGGER_H
#define ZLOGGER_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QDir>
#include <QString>
#include <QFile>

/* Логирование. ***************************************************************/
/******************************************************************************/
class ZLogger {

  public:
    ZLogger(
        const QDir &dir, const QString &name
      , unsigned int page_size = 100*1024, unsigned int page_quan = 2 );
   ~ZLogger();

    void setDir(const QDir& dir) { this->dir = dir; }
    void setName(const QString& name) { this->name = name; }

    void add(const QString& type, const QString& msg);

  private:
    QDir dir;
    QString name;
    unsigned int page_size, page_quan;

    QFile *file;

    QString giveFileName(void); // Предоставить имя файла.

};// SimpleLogger

//------------------------------------------------------------------------------
#endif // ZLOGGER_H

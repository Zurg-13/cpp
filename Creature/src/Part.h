#ifndef PART_H
#define PART_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QPainter>
#include <QString>
#include <QList>

/* Часть. *********************************************************************/
/******************************************************************************/
class Part {

 public:
    virtual ~Part();
             Part(
                const QString &name, const QList<double> &data
              , std::function<void(QPainter &pntr)> fdrw, int x, int y
              , const QList<Part*> chld );

    void virtual draw(QPainter &pntr) = 0;

 private:
    QString name;
    int x, y;
    Part *prnt;
    QList<double> data;
    QList<Part*> chld;
    std::function<void(QPainter &pntr)> fdrw;

};// Part

//------------------------------------------------------------------------------
#endif // PART_H

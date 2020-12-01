#ifndef PART_H
#define PART_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QPainter>
#include <QString>
#include <QList>

/* Состояние + допустимое отклонение. *****************************************/
/******************************************************************************/
struct Form {
    double std, val, dev;
    Form(double std, double val, double dev) : std(std), val(val), dev(dev) {}
    Form(double std, double dev) : std(std), val(std), dev(dev) {}
};// Form

/* Часть. *********************************************************************/
/******************************************************************************/
class Part {

 public:

    virtual~Part();
            Part(const QString &name, const QList<Form> &dna
              , std::function<void(int &x, int &y)> morf
              , std::function<void(
                    QPainter &pntr, const QList<Form> &dna, int x, int y)> fdrw
              , const QList<Part*> dsc = QList<Part*>() );

    void draw(QPainter &pntr, int x, int y);

 private:

    QString name;
    QList<Form> dna;
    std::function<void(int &x, int &y)> morf;
    std::function<void(
        QPainter &pntr, const QList<Form> &dna, int x, int y)> fdrw;
    QList<Part*> dsc;

};// Part

//------------------------------------------------------------------------------
#endif // PART_H

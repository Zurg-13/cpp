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
    qreal std, val, dev;
    Form(qreal std, qreal val, qreal dev) : std(std), val(val), dev(dev) {}
    Form(qreal std, qreal dev) : std(std), val(std), dev(dev) {}
};// Form

/* Часть. *********************************************************************/
/******************************************************************************/
class Part {

 public:
    QList<Form> dna;
    QList<Part*> dsc;

    virtual~Part();
            Part(const QString &name, const QList<Form> &dna
              , std::function<void(int &x, int &y)> morf
              , std::function<void(
                    QPainter &pntr, const QList<Form> &dna, int x, int y)> fdrw
              , const QList<Part*> dsc = QList<Part*>() );

    void draw(QPainter &pntr, int x, int y);

 private:
    QString name;
    std::function<void(int &x, int &y)> morf;
    std::function<void(
        QPainter &pntr, const QList<Form> &dna, int x, int y)> fdrw;

};// Part

//------------------------------------------------------------------------------
#endif // PART_H

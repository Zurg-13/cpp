#ifndef WSEL_H
#define WSEL_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QMap>

namespace Ui { class WSel; }

/* Выбор элемента из списка. **************************************************/
/******************************************************************************/
class WSel : public QWidget {
    Q_OBJECT

public:
    explicit WSel(QWidget *parent = 0);
            ~WSel();

    bool sel(QMap<QWidget*, QString> *map);
    QWidget* ret(void) { return wgt; }

private:
    Ui::WSel *ui;
    QMap<QWidget*, QString> *map;
    QWidget* wgt;

private slots:
    void selected(const QModelIndex &idx);

};

//------------------------------------------------------------------------------
#endif // WSEL_H

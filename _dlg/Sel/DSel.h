#ifndef DSEL_H
#define DSEL_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QDialog>
#include <QMap>
#include <QListWidgetItem>
#include <QVariant>
#include <QShowEvent>

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class DSel; }

/* Выбор элемента из списка. **************************************************/
/******************************************************************************/
class DSel : public QDialog {
    Q_OBJECT

public:
    explicit DSel(QWidget *parent = 0);
            ~DSel();

    int sel(QMap<QVariant, QString> *map);
    const QVariant& ret(void) { return key; }

private:
    Ui::DSel *ui;
    QMap<QVariant, QString> *map;
    QVariant key;

    void showEvent(QShowEvent *evt);

private slots:
    void selected(QListWidgetItem *item);

};// DSel

//------------------------------------------------------------------------------
#endif // DSEL_H

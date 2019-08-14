#ifndef DLIST_H
#define DLIST_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QDialog>
#include <QMap>
#include <QListWidgetItem>

namespace Ui { class DList; }


//------------------------------------------------------------------------------
typedef QMap<int, QString> Set;

/* Список выбираемых сущностей. ***********************************************/
/******************************************************************************/
class DList : public QDialog {
    Q_OBJECT

public:
    explicit DList(QWidget *parent = nullptr);
            ~DList();

    void showEvent(QShowEvent *event);

    void set(const Set &set);
    Set  get(void);

private:
    Ui::DList *ui;

};// DList

/* Выбираемый элемент. ********************************************************/
/******************************************************************************/
class Itm : public QListWidgetItem {

 public:
    Itm(
        int id, const QString &text
      , QListWidget *parent = nullptr, int type = Type );

    int id;

};// QListWidgetItem


//------------------------------------------------------------------------------
#endif // DLIST_H

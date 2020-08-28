#ifndef ITEM_H
#define ITEM_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class Item; }

/* Элемент меню. **************************************************************/
/******************************************************************************/
class Item : public QWidget {
    Q_OBJECT
    static int count;

 public:
    explicit Item(QWidget *prnt = nullptr);
            ~Item();

    void    set(const QString &xml);
    void    set(const QMap<QString, QString> &prm);
    QString jsn(void);

    static QString ELM(const QString &nme, const QString &val);

private slots:
    void on_btSel_clicked();

private:
    Ui::Item *ui;
    bool slct = false;
    int cnt;

};// Item


//------------------------------------------------------------------------------
#endif // ITEM_H

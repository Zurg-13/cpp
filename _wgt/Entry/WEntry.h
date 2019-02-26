#ifndef WENTRY_H
#define WENTRY_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class WEntry; }

// Запись в списке. ------------------------------------------------------------
//------------------------------------------------------------------------------
class WEntry : public QWidget {
    Q_OBJECT

public:

    explicit WEntry(QWidget *parent = 0);
            ~WEntry();

    WEntry* setReadOnly(bool);
    WEntry* setText(QString);
    WEntry* setState(bool);
    WEntry* setConfirmDelete(bool) ;
    WEntry* setPlaceholder(QString);
    WEntry* setSign(QString);

    QString text(void);
    QString sign(void) { return entry_sign; }
    int     id(void) { return entry_id; }
    void    setFocus(void);

signals:
    void remove(WEntry *);    // Попытка удаления.

private slots:
    void on_btDelete_clicked();

private:
    Ui::WEntry *ui;
    bool entry_state = false, confirm_delete;
    QString entry_sign;
    int entry_id = 0;

};// WEntry

//------------------------------------------------------------------------------
#endif // WENTRY_H

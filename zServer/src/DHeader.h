#ifndef DHEADER_H
#define DHEADER_H

#include <QDialog>

namespace Ui { class DHeader; }

// Диалог ввода заголовка. -----------------------------------------------------
//------------------------------------------------------------------------------
class DHeader : public QDialog {
    Q_OBJECT

public:
    explicit DHeader(QWidget *parent = nullptr);
            ~DHeader();

    QString type(void);
    QString value(void);
    QString title(void);

private:
    Ui::DHeader *ui;
    void showEvent(QShowEvent*);

};// DHeader

#endif // DHEADER_H

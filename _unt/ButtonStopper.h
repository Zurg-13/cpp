#ifndef BUTTONSTOPPER_H
#define BUTTONSTOPPER_H

#include <QString>
#include <QAbstractButton>

// Удерживает кнопку до завершения действия. -----------------------------------
//------------------------------------------------------------------------------
class ButtonStopper {

public:
    explicit ButtonStopper(QAbstractButton *btn,  const QString &msg = "");
            ~ButtonStopper();

private:
    QAbstractButton *btn;
    QString text;

};

//------------------------------------------------------------------------------
#endif // BUTTONSTOPPER_H

#ifndef FMAIN_H
#define FMAIN_H

#include <QMainWindow>

namespace Ui {
class FMain;
}

class FMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = nullptr);
    ~FMain();

private:
    Ui::FMain *ui;
};

#endif // FMAIN_H

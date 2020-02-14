#ifndef WPRINTER_H
#define WPRINTER_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

namespace Ui { class WPrinter; }

class WPrinter : public QWidget {
    Q_OBJECT

public:
    explicit WPrinter(QWidget *parent = nullptr);
    ~WPrinter();

private:
    Ui::WPrinter *ui;
};

#endif // WPRINTER_H

#ifndef WTEXTBROWZER_H
#define WTEXTBROWZER_H

#include <QWidget>
#include <QLabel>

namespace Ui { class WTextBrowzer; }

/* Виджет работы с QTextBrowzer. **********************************************/
/******************************************************************************/
class WTextBrowzer : public QWidget {
    Q_OBJECT

public:
    explicit WTextBrowzer(QWidget *parent = nullptr);
            ~WTextBrowzer();

private slots:
    void on_tb_anchorClicked(const QUrl &url);
    void on_tb_highlighted(const QUrl &url);

private:
    Ui::WTextBrowzer *ui;

    QLabel *lbl = nullptr;

};// WTextBrowzer

//------------------------------------------------------------------------------
#endif // WTEXTBROWZER_H

#ifndef WEDIT_H
#define WEDIT_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>
#include <QTextEdit>
#include <QXmlStreamWriter>

#include "SqlHighlighter.h"
#include "WOut.h"

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class WEdit; }

// Виджет редактирования запросов. ---------------------------------------------
//------------------------------------------------------------------------------
class WEdit : public QWidget {
    Q_OBJECT

public:
    explicit WEdit(QWidget *parent = 0);
            ~WEdit();

    void keyPressEvent(QKeyEvent *evt);
    void wheelEvent(QWheelEvent *evt);

    void write(QXmlStreamWriter &doc);


private slots:
    void on_aOpen_triggered();
    void on_aExec_triggered();
    void on_btTest_clicked();
    void on_aTest_triggered();
    void on_btCnn_clicked();

    void on_edSql_cursorPositionChanged();

private:
    Ui::WEdit *ui;
    SqlHighlighter *highlighter;
    WOut *out = nullptr;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QTextEdit::ExtraSelection selection;
    int txt_bgn, txt_end;

    QString curSql(void);

};// WEdit

//------------------------------------------------------------------------------
#endif // WEDIT_H

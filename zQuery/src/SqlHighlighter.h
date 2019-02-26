#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

class SqlHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    SqlHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct Rule {
        QRegExp pattern;
        QTextCharFormat format;

        Rule(){}
        Rule(QString pattern, QTextCharFormat format)
            : pattern(pattern), format(format) {}
        Rule(QRegExp pattern, QTextCharFormat format)
            : pattern(pattern), format(format) {}
    };

    QVector<Rule> rules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat commentFormat;

};// SqlHighlighter

#endif // SQLHIGHLIGHTER_H

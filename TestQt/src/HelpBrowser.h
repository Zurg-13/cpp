#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QWidget>

class QPushButton;
class QTextBrowser;

class HelpBrowser : public QWidget {
    Q_OBJECT

public:
    explicit HelpBrowser(QWidget *parent = nullptr);
             HelpBrowser(
                const QString &path, const QString &page
              , QWidget *parent = nullptr, const char *name = nullptr );

    static void showPage(const QString &page);

private slots:
  void updateCaption();

private:
  QTextBrowser *textBrowser;
  QPushButton  *homeButton;
  QPushButton  *backButton;
  QPushButton  *closeButton;
};// HelpBrowser

#endif // HELPBROWSER_H

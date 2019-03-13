#include <QApplication>
#include <QLayout>
#include <QPushButton>
#include <QTextBrowser>

#include "HelpBrowser.h"

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
HelpBrowser::HelpBrowser(QWidget *parent) : QWidget(parent) {

}// HelpBrowser

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
HelpBrowser::HelpBrowser(
    const QString &path, const QString &page, QWidget *parent, const char *name)
//  : QWidget(parent, name, WGroupLeader | WDestructiveClose)
    : QWidget(parent)
{
  textBrowser = new QTextBrowser(this);
  homeButton = new QPushButton(tr("&Home"), this);
  backButton = new QPushButton(tr("&Back"), this);
  closeButton = new QPushButton(tr("Close"), this);

//  closeButton->setAccel(tr("Esc"));
  closeButton->setShortcut(tr("Esc"));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

//  QHBoxLayout *buttonLayout = new QHBoxLayout(mainLayout);
  QHBoxLayout *buttonLayout = new QHBoxLayout();

  mainLayout->addLayout(buttonLayout); // ++zrg

  buttonLayout->addWidget(homeButton);
  buttonLayout->addWidget(backButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(closeButton);
  mainLayout->addWidget(textBrowser);

  connect(homeButton, SIGNAL(clicked()),
          textBrowser, SLOT(home()));
  connect(backButton, SIGNAL(clicked()),
          textBrowser, SLOT(backward()));
  connect(closeButton, SIGNAL(clicked()),
          this, SLOT(close()));
  connect(textBrowser, SIGNAL(sourceChanged(const QString &)),
          this, SLOT(updateCaption()));

// todo: чем заменить ?
//  textBrowser->mimeSourceFactory()->addFilePath(path);

  textBrowser->setSource(page);
}

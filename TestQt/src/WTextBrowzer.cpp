#include "ui_WTextBrowzer.h"
#include "WTextBrowzer.h"

#include <QTimer>

#include "_srv/Srv/dbg.h"
#include "_srv/Srv/std.h"



// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WTextBrowzer::WTextBrowzer(QWidget *parent)
    : QWidget(parent), ui(new Ui::WTextBrowzer)
{
    ui->setupUi(this);
    ui->tb->setOpenLinks(false);

    this->lbl = new QLabel(nullptr, Qt::WindowStaysOnTopHint);
    this->lbl->setWordWrap(true);
    this->lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
}// WTextBrowzer

//  Деструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WTextBrowzer::~WTextBrowzer() {
    delete lbl;
    delete ui;
}// ~WTextBrowzer

//------------------------------------------------------------------------------
void WTextBrowzer::on_tb_anchorClicked(const QUrl &url) {

    QObject *obj = sender();

    FNC << "on_tb_anchorClicked" << url.path();
    FNC << obj->objectName();


    QPoint pos = this->cursor().pos();
    pos.setY(pos.y());

    lbl->setText(url.path());
    lbl->move(pos);
    lbl->resize(300, 100);
    lbl->show();

}// on_tb_anchorClicked

void WTextBrowzer::on_tb_highlighted(const QUrl &url) {
    FNC << "on_tb_highlighted" << (url.isEmpty() ? "empty" : "full");
}// on_tb_highlighted

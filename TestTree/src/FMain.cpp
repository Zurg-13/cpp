#include <QMessageBox>
#include <QFile>
#include <QtXml>
#include <QFileDialog>

#include "_srv/dbg.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "unt/FileSystemModel.h"
#include "unt/ListTreeModel.h"


QVariant tree;

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {
    ui->setupUi(this);

    // Файловая модель.
    ui->filesTree->setUniformRowHeights(true);
    ui->filesTree->setModel(new FilesystemModel(this));

    // DOM-модель.
    dom_mdl = new DomTreeModel(QDomDocument(), this);
//    ui->viewTree->setUniformRowHeights(true);
    ui->viewTree->setModel(dom_mdl);

/*
    // Чтение через fetchMore.
    QMap<QString, QVariant> r_one = {
        {"nme", "r_one"}
      , {"one", "1"}, {"two", "11"}, {"thr", "111"}
    };

    QMap<QString, QVariant> r_two = {
        {"nme", "r_two"}
      , {"one", "2"}, {"two", "22"}, {"thr", "222"}
    };

    QMap<QString, QVariant> r_thr = {
        {"nme", "r_thr"}
      , {"one", "3"}, {"two", "33"}, {"thr", "333"}
    };

    QList<QVariant> root = { r_one, r_two, r_thr };
    tree = root;

    ui->listTree->setUniformRowHeights(true);
    ui->listTree->setModel(new ListTreeModel(&tree, this));
/**/

/*
    // Создаём структуру в ручную.
    ListTreeModel *mdl = new ListTreeModel(this);

    NodeInfo one_r(Info("one_r", "1", "11", "111"), true);
    mdl->nodes.append(one_r);

    mdl->nodes[0].childs.append(NodeInfo(Info("one_1", "1", "11", "111"), true, &mdl->nodes[0]));
    mdl->nodes[0].childs.append(NodeInfo(Info("one_2", "1", "11", "111"), true, &mdl->nodes[0]));
    mdl->nodes[0].childs.append(NodeInfo(Info("one_3", "1", "11", "111"), true, &mdl->nodes[0]));


//    mdl->nodes.append(NodeInfo(Info("r_two", "2", "22", "222")));
//    mdl->nodes.append(NodeInfo(Info("r_thr", "3", "33", "333")));

    ui->listTree->setUniformRowHeights(true);
    ui->listTree->setModel(mdl);
/**/


}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}// ~FMain

// Файл -> Выход. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {

    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }

}// on_aExit_triggered

// ОТладка -> Проба. -----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    FNC << R"(/bgn)";

    QString fnme("d:/temp/17/test.xml");
    QDomDocument doc;
    QFile file(fnme);

    // Загрузка XML.
    if(!file.open(QIODevice::ReadOnly) || doc.setContent(&file)) { return; }


    QDomElement xmlroot = doc.firstChildElement();
    FNC << "root" << "tag:" << xmlroot.tagName();


    FNC << R"(\end)" << endl;
}// on_aTest_triggered

void readTag(QDomDocument &doc, QDomElement &elm, int lvl) {
    QString LVL;
    for(int i=0; i<lvl; i++) { LVL += "----"; }
}// readTag

// Отладка -> Открыть XML. -----------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aOpenXML_triggered() {
    QString path = "d:/temp/17";
    QString fnme = QFileDialog::getOpenFileName(
        this, tr("Открыть XML-документ"), path, "XML: (*.xml);;HTML: (*.html)");

    if(!fnme.isEmpty()) {
        QFile file(fnme);
        if(file.open(QIODevice::ReadOnly)) {
            QDomDocument doc;
            if(doc.setContent(&file)) {
                DomTreeModel *new_mdl = new DomTreeModel(doc, this);
                ui->viewTree->setModel(new_mdl);
                delete dom_mdl; dom_mdl = new_mdl;
                path = fnme;
            }// if(doc.setContent(&file))
        }// if(file.open(QIODevice::ReadOnly))
        file.close();
    }// if(!fnme.isEmpty())

}// on_aOpenXML_triggered

//------------------------------------------------------------------------------


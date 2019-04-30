#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //create the model
    Filename = "d:/temp/17/MyXML.xml";
//    Filename = "d:/temp/17/test.xml";
    model = new QStandardItemModel(0,1,this);

    ReadFile();

    ui->treeView->setModel(model);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::ReadFile()
{
    QStandardItem *root = new QStandardItem("Books");
    model->appendRow(root);

    QDomDocument document;

    //load the xml file
    QFile file(Filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        document.setContent(&file);
        file.close();
    }

    //get the xml root element
    QDomElement xmlroot = document.firstChildElement();

    //read the books
    QDomNodeList books = xmlroot.elementsByTagName("Book");
    for(int i = 0; i < books.count(); i++)
    {
        QDomElement book = books.at(i).toElement();
        QStandardItem *bookitem = new QStandardItem(book.attribute("Name"));

        //read the chapters of the book
        QDomNodeList chapters = book.elementsByTagName("Chapter");
        for(int h = 0; h < chapters.count(); h++)
        {
            QDomElement chapter = chapters.at(h).toElement();
            QStandardItem *chapteritem = new QStandardItem(chapter.attribute("Name"));

            bookitem->appendRow(chapteritem);
        }

        root->appendRow(bookitem);
    }

}

void Dialog::WriteFile()
{
    //write the xml file
    QDomDocument document;

    //Make a root node
    QDomElement xmlroot = document.createElement("Books");
    document.appendChild(xmlroot);

    QStandardItem *root = model->item(0,0);
    for(int i = 0; i < root->rowCount(); i++)
    {
        QStandardItem *book = root->child(i,0);

        QDomElement xmlbook = document.createElement("Book");
        xmlbook.setAttribute("Name",book->text());
        xmlbook.setAttribute("ID", i);
        xmlroot.appendChild(xmlbook);

        for(int h = 0; h < book->rowCount(); h++)
        {
            QStandardItem *chapter = book->child(h,0);
            QDomElement xmlchapter = document.createElement("Chapter");
            xmlchapter.setAttribute("Name",chapter->text());
            xmlchapter.setAttribute("ID", h);
            xmlbook.appendChild(xmlchapter);
        }
    }

    //save to disk
    QFile file(Filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to write file";
    }

    QTextStream stream(&file);
    stream << document.toString();
    file.close();

     qDebug() << "Finished";
}

void Dialog::on_pushButton_clicked()
{
    //save the document
    WriteFile();
}

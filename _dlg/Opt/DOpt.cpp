// INCLUDE. --------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <functional>
#include <QKeyEvent>
#include <QDesktopWidget>

#include "_srv/std.h"

#include "ui_DOpt.h"
#include "DOpt.h"


// Выбрать ключ из запроса. ----------------------------------------------------
//------------------------------------------------------------------------------
int DOpt::from(ZSqlQuery &qry, const QString ttl, int width) {

    QSqlQueryModel *mdl = qobject_cast<QSqlQueryModel*>(ui->tv->model());
    this->qry = &qry;

    // Подготовка запроса.
    if(!this->qry->is_prep()) { this->qry->prepare(); }
    if(ui->edFind->text().isEmpty())
        { this->qry->BV(":fnd", ""); this->qry->exec(); }
    else
        { ui->edFind->setText(""); }

    // Кофигурирование модели отображения.
    mdl->setQuery(*this->qry);
    int col = IDX(mdl, "SHOW");

    mdl->setHeaderData(col, Qt::Horizontal, ttl);
    for(int i = 0; i < mdl->columnCount(); i++) {
        if(i == col) { ui->tv->showColumn(i); }
        else         { ui->tv->hideColumn(i); }
    }// i

    QFont fnt; fnt = ui->tv->font(); fnt.setBold(true);
    ui->tv->horizontalHeader()->setFont(fnt);

    QModelIndex idx = ui->tv->currentIndex();
    ui->tv->setCurrentIndex(idx.sibling(0, 0));

    // Запуск.
    this->setGeometry(this->x(), this->y(), width, this->height());
    ui->edFind->setFocus();
    ui->tv->selectRow(0);
    return this->exec();

}// From

//Конструктор.------------------------------------------------------------------
//------------------------------------------------------------------------------
DOpt::DOpt(QWidget *parent): QDialog(parent), ui(new Ui::DOpt) {

    //Инициализация интерфейса.
    ui->setupUi(this); TO_CENTER(DESKTOP);
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    ui->tv->setModel(new QSqlQueryModel(ui->tv)); //Удаляется родителем.
    ui->edFind->installEventFilter(this);

}// DOpt

//Деструктор.-------------------------------------------------------------------
//------------------------------------------------------------------------------
DOpt::~DOpt() { delete ui; }

//Фильтр событий.---------------------------------------------------------------
//------------------------------------------------------------------------------
bool DOpt::eventFilter(QObject *obj, QEvent *event) {

    if(obj == ui->edFind){
        switch(event->type()){
          case QEvent::KeyPress: //Перехват нажатия кнопки.
                if(static_cast<QKeyEvent*>(event)->key() == Qt::Key_Down)
                    { ui->tv->setFocus(); return true; }
          return false;

          default : return false;
        }// switch(event->type())
    }// if(obj == ui->edFind)

    return QDialog::eventFilter(obj, event);
}// eventFilter

//Двойной щелчёк на таблице выбора.---------------------------------------------
//------------------------------------------------------------------------------
void DOpt::on_tv_doubleClicked(QModelIndex) { on_btOk_clicked(); }

//Нажатие кнопки "ПРИНЯТЬ".-----------------------------------------------------
//------------------------------------------------------------------------------
void DOpt::on_btOk_clicked() {
    auto RECORD = [this](void) -> QSqlRecord {
        return qobject_cast<QSqlQueryModel*>(ui->tv->model())->record(
            CUR_IDX(ui->tv).row() );
    }; // RECORD

    val = RECORD().contains("RET") ? RECORD().value("RET") : QString();
    rec = RECORD();

    this->accept();
}// on_btOk_clicked

//Изменение в строке поиска.-------------------------------------------------
//---------------------------------------------------------------------------
void DOpt::on_edFind_textChanged(QString str) {

    if(!str.isEmpty() && str.length() <= 1) { return; }

    this->qry->BV(":fnd", str);
    this->qry->exec();

    qobject_cast<QSqlQueryModel*>(ui->tv->model())->setQuery(*this->qry);
    ui->tv->selectRow(0);

}// on_edFind_textChanged

//---------------------------------------------------------------------------


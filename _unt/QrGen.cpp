#include <QPainter>
#include <QColor>
#include <QFile>
#include <QDir>

#include "QrGen.h"


// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------

// Приведение QSize к QRect. ---------------------------------------------------
//------------------------------------------------------------------------------
const QRect RCT(const QSize &sze)
    { return QRect(0, 0, sze.width(), sze.height()); }

// Создать изображение.  --------------------------------------------------------
//------------------------------------------------------------------------------
void QrGen::new_img(int wdh, int hgt)
    { this->img = new QImage(wdh, hgt, fmt); }

// Конструкторы. ---------------------------------------------------------------
//------------------------------------------------------------------------------
/*
QrGen::QrGen(QObject *parent, const QList<QrItem> &itm)
  : QThread(parent), itm(itm), fmt(fmt), quality(quality), ecc(ecc)
{
    this->can_work = YES_WRK;
}// QrGen
*/

QrGen::QrGen(
    QObject *parent
  , const QList<QrItem> &itm
  , QImage::Format fmt, int quality, qrcodegen::QrCode::Ecc ecc)
  : QThread(parent), itm(itm), fmt(fmt), quality(quality), ecc(ecc)
{
    this->can_work = YES_WRK;
}// QrGen

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
QrGen::~QrGen() { delete img; }

// Отрисовать Qr-код. ----------------------------------------------------------
//------------------------------------------------------------------------------
void QrGen::gen(
    QPainter &painter, int scale, int indent, const qrcodegen::QrCode &qr )
{
    for(int y=0; y<qr.size; y++) {
        for(int x=0; x<qr.size; x++) {
            const int color = qr.getModule(x, y);  // 0 for white, 1 for black
            if(0 != color) {
                const int rx = x*scale, ry = y*scale;
                painter.drawRect(rx+indent, ry+indent, scale, scale);
            }// if(0 != color)
        }// x
    }// y
}// gen

// Запускаемая в отдельном потоке функция. -------------------------------------
//------------------------------------------------------------------------------
void QrGen::run(){
    static const QDir dir;
    static const QString ERR_SVE("Не удалось сохраниь файл:{%1}");
    static const QString ERR_GEN("Не удалось создать код:{%1}, причина: %2.");
    static const QString ERR_WHY("Размер изображения: %1, размер кода: %2");

    for(const QrItem &item : this->itm ) {
        if(this->can_work == NOT_WRK) { break; }

        // Подготовка
        std::string str(item.text.toStdString());
        qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(str.c_str(), ecc);
        int scale = item.size / qr.size;
        int size = (item.mrgn == MARGIN::NOT ? qr.size*scale : item.size);
        int indent = (size % qr.size) / 2;

        if(this->img == nullptr)
            { new_img(size, size); }
        else if(size != this->img->size().height())
            { delete this->img; new_img(size, size); }

        // Контроль размеров.
        if(size < qr.size) {
            emit error(ERR::GEN, ERR_GEN.arg(item.text, ERR_WHY.arg(
                QString::number(size), QString::number(qr.size) )));
            continue;
        }// if(this->size < qr.size)

        this->pnt.begin(this->img); // ### BGN ###
        this->pnt.setPen(Qt::NoPen);
        this->img->fill(item.mclr);
        this->pnt.setBrush(item.fclr);
        this->pnt.drawRect(indent, indent, qr.size*scale, qr.size*scale);

        // Отрисовка.
        this->pnt.setBrush(item.pclr);
        gen(this->pnt, scale, indent, qr);
        dir.mkpath(QFileInfo(item.fnme).path());
        if(!this->img->save(item.fnme, nullptr, quality))
            { emit error(ERR::SVE, ERR_SVE.arg(item.fnme)); };

        this->pnt.end();            // ### END ###
    }// item

    emit ready(this);
}// run

//------------------------------------------------------------------------------


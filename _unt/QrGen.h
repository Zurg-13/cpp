#ifndef QRGEN_H
#define QRGEN_H

#include <QAtomicInt>
#include <QObject>
#include <QThread>
#include <QImage>
#include <QPainter>
#include <QList>

#include "_lib/QrCodeGenerator/QrCode.hpp"

enum { YES_WRK, NOT_WRK };
enum class MARGIN { NOT=0, YES=1, TRP=3 };

// Элемент генерации Qr-кода. --------------------------------------------------
//------------------------------------------------------------------------------
struct QrItem {
    QString fnme, text;
    int size;
    MARGIN mrgn;
    QColor mclr, fclr, pclr;

    QrItem(
        const QString &fnme, const QString &text, int size, MARGIN mrgn
      , const QColor &mclr, const QColor &fclr, const QColor &pclr )
      : fnme(fnme), text(text), size(size), mrgn(mrgn)
      , mclr(mclr), fclr(fclr), pclr(pclr) {}
};// QrItem

// Генератор Qr-кодов. ---------------------------------------------------------
//------------------------------------------------------------------------------
class QrGen : public QThread {
    Q_OBJECT

  public:
    enum class ERR { GEN, SVE };

    const QImage::Format fmt;
    const int quality;
    const qrcodegen::QrCode::Ecc ecc;

    QrGen(
        QObject *parent
      , const QList<QrItem> &itm
      , QImage::Format fmt = QImage::Format_Mono, int quality = -1
      , qrcodegen::QrCode::Ecc ecc = qrcodegen::QrCode::Ecc::LOW );

   ~QrGen();

    void stop(void) { can_work = NOT_WRK; }

  signals:
    void ready(QrGen *gen);
    void error(QrGen::ERR code, QString err);

  private:
    QAtomicInt can_work;
    const QList<QrItem> &itm;
    QImage *img = nullptr;
    QPainter pnt;

    void gen(
        QPainter &painter, int scale, int indent, const qrcodegen::QrCode &qr );
    void new_img(int wdh, int hgt);

    void run() override;
};// QrGen

//------------------------------------------------------------------------------
#endif // QRGEN_H

#ifndef MSCANER_H
#define MSCANER_H


// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QObject>
#include <QMap>
#include <QUsb>
#include <QTimer>

#include "Module.h"

/* Работа со сканером штрих-кодов. ********************************************/
/******************************************************************************/
class MScaner : public QObject, public Module {
    Q_OBJECT

 public:
    QUsbDevice *usb = nullptr;

    const QMap<QString, QVariant> cmd = {
        {"scan_qrcode", 1}, {"scan_status", 2} };

    explicit MScaner(const QString &cfg, QObject *prn = nullptr);
            ~MScaner(void) override;

    Result wrk(Command command) override;  // Обработчик команд.
    bool   rdy(QString &cmd_name) override
        { return ready && cmd.contains(cmd_name); }

    void init(const QString &cfg);
    void open(const QString &cfg) { init(cfg); open(); }
    void open(void);
    void close(void);

    QString scan(void);
    QString info(void);

 signals:
   void sgl_wait_ok(void);

 private slots:
   void on_poll(void); // Опрос сканера.

 private:
   bool isDeviceOpened = false;
   QTimer tmr;
   QUsbManager mng;
   QtUsb::DeviceFilter fltr;
   QtUsb::DeviceConfig conf;

   QtUsb::DeviceConfig takeConf(const QtUsb::DeviceFilter &fltr);
   bool usb_open(void);
   void usb_close(void);
   void usb_read(QByteArray *buf);
   void usb_write(QByteArray *buf);

};// MScaner

//------------------------------------------------------------------------------
#endif // MSCANER_H

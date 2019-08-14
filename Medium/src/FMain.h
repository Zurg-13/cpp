#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QUsb>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>

#include "ZLogger.h"

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class FMain; }


/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();

    void SET_PRM(QList<QString>);   // Передать аргументы командной строки.

    bool openDev(void);
    bool closeDev(void);
    void read(QByteArray *buf);
    void write(QByteArray *buf);

public slots:
    void on_DevInsert(QtUsb::FilterList lst);
    void on_DevRemove(QtUsb::FilterList lst);

private slots:
    void on_poll(void); // Опрос сканера.
    void on_reply(QNetworkReply *reply);

    void on_aExit_triggered();
    void on_aTest_triggered();
    void on_aHide_triggered();

    void on_btNotifyUSB_clicked();
    void on_btOpen_clicked();
    void on_btRead_clicked();
    void on_btListDev_clicked();
    void on_btConfig_clicked();
    void on_aTestInit_triggered();
    void on_aDemugMode_triggered();
    void on_btClose_clicked();

private:
    Ui::FMain *ui;

    QTimer timer;

    bool isDeviceOpened = false;
    QUsbDevice *dev = nullptr;

    QtUsb::DeviceFilter fltr;
    QtUsb::DeviceConfig conf;

    QNetworkAccessManager net;
    QNetworkRequest req;

    ZLogger *log = nullptr;

    void testInitDev(void); //отладка

    void CONNECT(void); // Подключить сигналы/слоты.
    void DEV_INIT(void); // Инициализировать сканер.
    void LOG(const QString nme);

    QtUsb::DeviceConfig takeConf(const QtUsb::DeviceFilter &fltr);

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

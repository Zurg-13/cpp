#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <windows.h>

#include <QMainWindow>
#include <QAbstractNativeEventFilter>

namespace Ui { class FMain; }


#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC  static_cast<USHORT>(0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE static_cast<USHORT>(0x02)
#endif

class RawEvent : public QAbstractNativeEventFilter {
    virtual bool nativeEventFilter(
        const QByteArray &eventType, void *message, long *result) override;
};

/* Главная форма. *************************************************************/
/******************************************************************************/
class FMain : public QMainWindow {
    Q_OBJECT

  public:
    explicit FMain(QWidget *parent = nullptr);
            ~FMain();
/*
    RawEvent rawEvt;
*/

  protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

  private slots:
    void on_aExit_triggered();
    void on_aTest_triggered();
    void on_btAdd_clicked();

  private:
    Ui::FMain *ui;

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H

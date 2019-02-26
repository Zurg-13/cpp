#ifndef WAITFORSIGNAL_H
#define WAITFORSIGNAL_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QEventLoop>

/* Ожидание сигнала с таймаутом. **********************************************/
/******************************************************************************/
class WaitForSignal : public QObject {
    Q_OBJECT

public:
    WaitForSignal(QObject &object, const char *signal);
    bool wait(int timeoutMs); // false - если время вышло.

public slots:
    void timeout(void);

private:
    bool tout;
    QEventLoop eloop;

};// WaitForSignal

//------------------------------------------------------------------------------
#endif // WAITFORSIGNAL_H

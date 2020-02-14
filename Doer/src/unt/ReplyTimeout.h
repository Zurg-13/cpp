#ifndef REPLYTIMEOUT_H
#define REPLYTIMEOUT_H

#include <QObject>
#include <QBasicTimer>
#include <QNetworkReply>
#include <QTimerEvent>

/* Заверешение запроса по таймауту. *******************************************/
/******************************************************************************/
class ReplyTimeout : public QObject {
    Q_OBJECT

 public:
    enum HandleMethod { Abort, Close };

    ReplyTimeout(QNetworkReply* rpl, int tout, HandleMethod method = Abort)
    :  QObject(rpl), mtd(method)
    {
        if(rpl && rpl->isRunning()) {
            tmr.start(tout, this);
            connect(rpl, &QNetworkReply::finished, this, &QObject::deleteLater);
        }// if(rpl && rpl->isRunning())
    }// ReplyTimeout

    static void set(
        QNetworkReply* rpl, int msec, HandleMethod method = Abort)
        { new ReplyTimeout(rpl, msec, method); }

 protected:
      void timerEvent(QTimerEvent * evt) {
        if(!tmr.isActive() || evt->timerId() != tmr.timerId())
           { return; }

        auto rpl = static_cast<QNetworkReply*>(parent());
        if(rpl->isRunning()) {
            switch(mtd) {
                case Close: rpl->close(); break;
                case Abort: rpl->abort(); break;
            }// switch(mtd)

            tmr.stop();
        }//if(rpl->isRunning())
    }// timerEvent

 private:
    QBasicTimer tmr;
    HandleMethod mtd;

};// ReplyTimeout

//------------------------------------------------------------------------------
#endif // REPLYTIMEOUT_H

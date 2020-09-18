#ifndef WCLIENTWS_H
#define WCLIENTWS_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWebSocket>
#include <QAbstractSocket>
#include <QWidget>

#include "WBrd.h"

namespace Ui { class WClientWS; }

/* Клиент WebSocket. **********************************************************/
/******************************************************************************/
class WClientWS : public QWidget {
    Q_OBJECT

 public:
            ~WClientWS();
    explicit WClientWS(QWidget *parent = nullptr);

 private:
    Ui::WClientWS *ui;
    QWebSocket *ws = nullptr;
    WBrd *bd = nullptr;

 protected:
    void showEvent(QShowEvent *evt);

 private slots:
   void ws_connect(void);
   void ws_disconnect(void);
   void ws_error(QAbstractSocket::SocketError err);
   void ws_msg_rcv(QString msg);
   void ws_bin_rcv(const QByteArray &frm, bool last);
   void ws_txt_rcv(const QString &frm, bool last);

   void on_btRun_clicked();
   void on_btStp_clicked();
   void on_btSnd_clicked();
};// WClientWS

//------------------------------------------------------------------------------
#endif // WCLIENTWS_H

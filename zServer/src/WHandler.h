#ifndef WHANDLER_H
#define WHANDLER_H

#include <QWidget>
#include <QColor>
#include <QMenu>
#include <QMap>
#include <QPair>
#include <QHttpServerResponse>
#include <QHttpServerRequest>

namespace Ui { class WHandler; }
typedef QPair<QString, QString> Header;

/* Обработчик. ****************************************************************/
/******************************************************************************/
class WHandler : public QWidget {
    Q_OBJECT
    enum class WAIT_TYPE { NOT, BTN };
    enum class ANSWER_TYPE { TEXT, FILE, QUERY };

public:
    QColor color;
    QString path;
    ANSWER_TYPE answer_type = ANSWER_TYPE::TEXT;
    WAIT_TYPE wait_type = WAIT_TYPE::NOT;

            ~WHandler();
    explicit WHandler(QWidget *parent = nullptr);
             WHandler(
                QString type, QString path, QColor color, QString text
              , QWidget *parent = nullptr );


    QHttpServerResponse answer(void);

    QString plane_text(void);
    QString answer_type_name(void);

    void interrupt(void);

signals:
    void remove(WHandler *);

private:
    Ui::WHandler *ui;
    QMenu *header = nullptr;
    QMap<QMenu*, Header> hdr;
    bool is_interrupted = false;

    void setColor(QColor clor);
    void setType(QString type_name);
    void setWait(WAIT_TYPE type);

    QHttpServerResponse answer_text(void);
    QHttpServerResponse answer_file(void);
    QHttpServerResponse answer_query(void);

private slots:
    void on_edPath_textChanged(const QString&);
    void on_DelHeader();

    void on_aSetColor_triggered();
    void on_aAsText_triggered();
    void on_aAsFile_triggered();
    void on_aAsQuery_triggered();
    void on_aQuit_triggered();
    void on_aAddHeader_triggered();
    void on_aRequest_triggered();

    void on_aWaitNot_triggered();
    void on_aWaitBtn_triggered();
};// WHandler

//------------------------------------------------------------------------------
#endif // WHANDLER_H

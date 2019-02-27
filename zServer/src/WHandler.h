#ifndef WHANDLER_H
#define WHANDLER_H

#include <QWidget>
#include <QColor>
#include <QMenu>
#include <QMap>
#include <QPair>

namespace Ui { class WHandler; }
typedef QPair<QString, QString> Header;

// Обработчик. -----------------------------------------------------------------
//------------------------------------------------------------------------------
class WHandler : public QWidget {
    Q_OBJECT
    enum class ANSWER_TYPE { TEXT, FILE, QUERY };

public:
    QColor color;
    QString path;
    ANSWER_TYPE answer_type = ANSWER_TYPE::TEXT;

    explicit WHandler(QWidget *parent = nullptr);
             WHandler(QString type, QString path, QColor color, QString text
               , QWidget *parent = nullptr );
            ~WHandler();

    QByteArray answer(void);
    QString plane_text(void);
    QString answer_type_name(void);
    QList<QString> header_lst(void);

signals:
    void remove(WHandler *);

private:
    Ui::WHandler *ui;
    QMenu *header = nullptr;
    QMap<QMenu*, Header> hdr;

    void setColor(QColor color);
    void setType(QString type_name);

    QByteArray answer_text(void);
    QByteArray answer_file(void);
    QByteArray answer_query(void);

private slots:
    void on_edPath_textChanged(const QString&);
    void on_DelHeader();

    void on_aSetColor_triggered();
    void on_aAsText_triggered();
    void on_aAsFile_triggered();
    void on_aAsQuery_triggered();
    void on_aQuit_triggered();

    void on_aAddHeader_triggered();
};// WHandler

//------------------------------------------------------------------------------
#endif // WHANDLER_H

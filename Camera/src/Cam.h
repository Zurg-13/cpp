#ifndef CAM_H
#define CAM_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QObject>
#include <QPointer>
#include <QPixmap>
#include <QCamera>
#include <QCameraInfo>
#include <QTimer>

#include <QCameraImageCapture>

// CONST. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
const int ACTIVE_PERIOD = 5000;

/* Захват изображения с камер. ************************************************/
/******************************************************************************/
class Cam : public QObject {
    Q_OBJECT

 public:
    explicit Cam(const QCameraInfo &info,  QObject *parent = nullptr);
            ~Cam();
    const QPixmap& captPix(void);
    void  reactive(void) { camera->unload(); };

 public slots:
    void camera_err(QCamera::Error err);
    void camera_cpt(int id, const QImage& img);

    void capture_err(int id, QCameraImageCapture::Error err, const QString &str);

 private:
    QTimer timer;
    QPixmap pixmap;

    QCamera *camera = nullptr;
    QCameraImageCapture *capture = nullptr;

};// Cam

//------------------------------------------------------------------------------
#endif // CAM_H

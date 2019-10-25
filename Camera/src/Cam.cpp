// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QCameraInfo>

#include "dbg.h"

#include "Cam.h"

/* Cam. ***********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
Cam::Cam(const QCameraInfo &info,  QObject *parent) : QObject(parent) {
    FNC << "bgn";

    // Камера.
    camera = new QCamera(info);
    capture = new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    connect(camera, QOverload<QCamera::Error>::of(&QCamera::error)
          , this, &Cam::camera_err );
    connect(capture, &QCameraImageCapture::imageCaptured
          , this, &Cam::camera_cpt );

    connect(capture, SIGNAL(error()), this, SLOT(capture_err()));

    // Таймер.
    connect(&timer, &QTimer::timeout, [this]() {
        FNC << "stop";
        if(QCamera::Status::ActiveStatus == camera->status()){ camera->stop(); }
        timer.stop();
    });

}// Cam

void Cam::capture_err(
    int id, QCameraImageCapture::Error err, const QString &str )
    { FNC << str; }

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
Cam::~Cam() {
    delete camera;
    delete capture;

    FNC << "end";
}// ~Cam

// Захват изображения. ---------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMultimedia>
const QPixmap& Cam::captPix() {

    FNC << camera->status();

    switch(camera->status()) {
     case QCamera::Status::ActiveStatus:
        capture->capture(); timer.start(ACTIVE_PERIOD); break;

     case QCamera::Status::UnloadedStatus:
        camera->load(); break;

     case QCamera::Status::LoadedStatus:
        camera->start(); break;

     default: break;
    }// switch(camera->status())

/*
    if(QCamera::Status::ActiveStatus == camera->status()) {

    } else if(QCamera::Status::ActiveStatus == camera->status())

    if(QCamera::Status::ActiveStatus != camera->status()) {
        camera->start();
    } else {
    }// else // if(QCamera::State::ActiveState != camera->state())
*/

    return pixmap;
}// captPix

// Ошибка работы с камерой. ----------------------------------------------------
//------------------------------------------------------------------------------
void Cam::camera_err(QCamera::Error err) {
    FNC << err << reinterpret_cast<QCamera*>(sender())->errorString();
}// camera_err

// Захват изображения. ---------------------------------------------------------
//------------------------------------------------------------------------------
void Cam::camera_cpt(int id, const QImage& img) {
    FNC << id;
    pixmap = QPixmap::fromImage(img);
}// camera_cpt

//------------------------------------------------------------------------------





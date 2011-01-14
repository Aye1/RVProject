/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 *  remarks :
 *      based on video library furnished with ARToolkit
 *      linux : support V4L, IEEE1394
 */

#include "Camera.h" 
#include <iostream>
#include <sstream>

namespace arv {

#define ERROR_RETURN(msg, ret) { std::cerr << "- error in arv::Camera : " << msg << "." << std::endl; return ret; }

Camera* Camera::create(Type type, Format format) {
    Camera* camera = new Camera();
    if (!camera->init(type, format)) { delete camera; return NULL; }
    return camera;
    }

bool Camera::init(Type type, Format format) {
    std::stringstream config;
    
    // R.G. : charger le fichier config, recuperer info sur la camera, puis tuning sur le format et basta
    switch (type) {
        case SONY           : config << "-channel=2 -mode=PAL        -dev=/dev/video0"; break;   // Sony EVI-D31 camera on serial port
        case PHILIPS        : config << "-channel=0 -palette=YUV420P -dev=/dev/video1"; break;   // Philips ToUcam Pro camera on USB port
        case SONY_COMPOSITE : config << "-channel=1 -mode=PAL        -dev=/dev/video0"; break;   // composite input
        case PHILIPS_SECOND : config << "-channel=0 -palette=YUV420P -dev=/dev/video2"; break;   // second camera on USB port
        }
    
    switch (format) {
        case FORMAT_160x120 : config << " -width=" << 160 << " -height=" << 120; break;
        case FORMAT_320x240 : config << " -width=" << 320 << " -height=" << 240; break;
        case FORMAT_640x480 : config << " -width=" << 640 << " -height=" << 480; break;
        case FORMAT_768x576 : config << " -width=" << 768 << " -height=" << 576; break;
        }
    
    // we need to cast <config> characters into a non-const char* because of the (bad) way 'ar2VideoOpen' is declared :
    handle = ar2VideoOpen(const_cast<char*>(config.str().c_str()));
    if (handle == NULL) ERROR_RETURN("problem during initialization by ARToolKit", false)
    
    ar2VideoInqSize(handle, &w, &h);
    
    return true;
    }

Camera::~Camera() {
    if (handle != NULL) ar2VideoClose(handle);
    }

bool Camera::start() {
    return ar2VideoCapStart(handle) == 0;
    }

bool Camera::stop() {
    return ar2VideoCapStop(handle) == 0;
    }

bool Camera::update() {
    return ar2VideoCapNext(handle) == 0;
    //return ar3VideoCapNext(handle, &time) == 0;
    }

Image Camera::getImage() {
    Time time;
    //TIME_UPDATE(time)
    ARUint8 *data = ar3VideoGetImage(handle, &time);
    // if (data == NULL) usleep(10);
    return Image(w, h, (char*)data, time);
    }

}


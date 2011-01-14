/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

#include "MotorizedCamera.h" 
#include <iostream>
#include <stdlib.h>
#include <cc_evid.hpp>
using namespace std;

namespace arv {

long getValue(CEVID30* obj, void (CEVID30::*method)(long &)) { long val; (obj->*method)(val); return val; }


/*********************************************************************************************/

MotorizedCamera::MotorizedCamera(CEVID30 *mcam) : Camera(),
        mcam(mcam),
        PAN_MIN(getValue(mcam,  &CEVID30::reqMinAbsPan)),
        PAN_MAX(getValue(mcam,  &CEVID30::reqMaxAbsPan)),
        PAN_SPEED_MIN(getValue(mcam,  &CEVID30::reqMinPanSpeed)),
        PAN_SPEED_MAX(getValue(mcam,  &CEVID30::reqMaxPanSpeed)),
        TILT_MIN(getValue(mcam, &CEVID30::reqMinAbsTilt)),
        TILT_MAX(getValue(mcam, &CEVID30::reqMaxAbsTilt)),
        TILT_SPEED_MIN(getValue(mcam, &CEVID30::reqMinTiltSpeed)),
        TILT_SPEED_MAX(getValue(mcam, &CEVID30::reqMaxTiltSpeed)),
        ZOOM_MIN(getValue(mcam, &CEVID30::reqMinZoom)),
        ZOOM_MAX(getValue(mcam, &CEVID30::reqMaxZoom)),
        ZOOM_SPEED_MIN(getValue(mcam, &CEVID30::reqMinZoomSpeed)),
        ZOOM_SPEED_MAX(getValue(mcam, &CEVID30::reqMaxZoomSpeed))
        {
    panSpeed  = 0;
    tiltSpeed = 0;
    zoomSpeed = 0;
    }

MotorizedCamera* MotorizedCamera::create(Type type, Format format, int serialLine) {
    CEVID30 *mcam = new CEVID30();
    if (mcam->Init(serialLine)) {
        cerr << "error while initializing camera" << endl;
        cerr << "check cable or enter another serial line number" << endl;
        return NULL;
        }
    
    MotorizedCamera* camera = new MotorizedCamera(mcam);
    if (!camera->init(type, format)) { delete camera; return NULL; }
    return camera;
    }

/*********************************************************************************************/

//void MotorizedCamera::reset() {}

void MotorizedCamera::pan(int pan, bool relative) {
    mcam->FastPanAbsPosition((long)pan + (relative ? getPan() : 0L));
    }

void MotorizedCamera::tilt(int tilt, bool relative) {
    mcam->FastTiltAbsPosition((long)tilt + (relative ? getTilt() : 0L));
    }

void MotorizedCamera::zoom(int zoom, bool relative) {
    mcam->setAbsZoomPos((long)zoom + (relative ? getZoom() : 0L));
    }

void MotorizedCamera::panAndTilt(int pan, int tilt, bool relative) {
    mcam->FastPanTiltAbsPosition((long)pan + (relative ? getPan() : 0L), (long)tilt + (relative ? getTilt() : 0L));
    }


long MotorizedCamera::getPan()  const { long val; mcam->reqPanAbsPos(val);  return val; }
long MotorizedCamera::getTilt() const { long val; mcam->reqTiltAbsPos(val); return val; }
long MotorizedCamera::getZoom() const { long val; mcam->reqAbsZoomPos(val); return val; }

void MotorizedCamera::resetPanTilt() {
    mcam->moveHome();
    }

/*********************************************************************************************/

long MotorizedCamera::getZoomSpeed() const { return zoomSpeed; }
long MotorizedCamera::getPanSpeed()  const { return panSpeed;  }
long MotorizedCamera::getTiltSpeed() const { return tiltSpeed; }

void MotorizedCamera::setZoomSpeed(long speed) {
    zoomSpeed = speed;
    mcam->setZoomSpeed(labs(zoomSpeed));
    if (zoomSpeed == 0)         mcam->stopZoom();
    else if (zoomSpeed < 0)     mcam->startFastWideZoom();
    else                        mcam->startFastTeleZoom();
    }

void MotorizedCamera::setPanTiltSpeed(long panSpeed, long tiltSpeed) {
    this->panSpeed = panSpeed;
    this->tiltSpeed = tiltSpeed;
    mcam->setPanSpeed(labs(panSpeed));
    mcam->setTiltSpeed(labs(tiltSpeed));
    if (tiltSpeed == 0) {
        if (panSpeed == 0)      mcam->stopPanTilt();
        else if (panSpeed < 0)  mcam->startPanLeft();
        else                    mcam->startPanRight();
        }
    else if (tiltSpeed < 0) {
        if (panSpeed == 0)      mcam->startTiltDown();
        else if (panSpeed < 0)  mcam->startGoDownleft();
        else                    mcam->startGoDownright();
        }
    else {
        if (panSpeed == 0)      mcam->startTiltUp();
        if (panSpeed < 0)       mcam->startGoUpleft();
        else                    mcam->startGoUpright();
        }
    }

}

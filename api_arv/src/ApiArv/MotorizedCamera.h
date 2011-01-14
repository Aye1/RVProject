/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

 
#ifndef __ARV_MOTORIZED_CAMERA__
#define __ARV_MOTORIZED_CAMERA__

#include "Camera.h"

class CEVID30;

namespace arv {

    /** @brief Interface with a physical motorized camera.
     * This is a subclass of \c Camera. It allows to control camera zoom and orientation in real-time.
     */
    class MotorizedCamera : public Camera {
        public:
            static MotorizedCamera* create(Type type = SONY, Format format = FORMAT_640x480, int serialLine = 2);
                /**< Creates a new instance.
                 @param type defines the type of camera
                 @param format defines the video format
                 @param serialLine defines the serial port (COM) on which the camera is plugged.
                 @return the created object or \c NULL if an error occured during initialization.
                 */
            
            void pan(int pan,   bool relative=false);
                /**< Changes the pan value.
                 @param pan the new pan value to set.
                 @param relative tells wether \p pan is an absolute value or is relative to the current state of the camera.
                 @warning if \p relative is set to \c true, the current camera pan state needs to be queried which is costly in time.
                 */
            void tilt(int tilt, bool relative=false);
                /**< Changes the tilt value.
                 @param tilt the new tilt value to set.
                 @param relative tells wether \p tilt is an absolute value or is relative to the current state of the camera.
                 @warning if \p relative is set to \c true, the current camera tilt state needs to be queried which is costly in time.
                 */
            void zoom(int zoom, bool relative=false);
                /**< Changes the zoom value.
                 @param zoom the new zoom value to set.
                 @param relative tells wether \p zoom is an absolute value or is relative to the current state of the camera.
                 @warning if \p relative is set to \c true, the current camera zoom state needs to be queried which is costly in time.
                 */
            void panAndTilt(int pan, int tilt, bool relative=false);
                /**< Changes both pan and tilt value.
                 This is usually faster than panning and tilting successively.
                 @param pan the new pan value to set.
                 @param tilt the new tilt value to set.
                 @param relative tells wether \p pan and \p tilt are absolute values or are relative to the current state of the camera.
                 @warning if \p relative is set to \c true, the current camera pan and tilt states needs to be queried which is costly in time.
                 */
            
            long getPan()  const;
                /**< Returns the current pan value (horizontal angle) of the camera.
                 @return the current pan value.
                 */
            long getTilt() const;
                /**< Returns the current tilt value (vertical angle) of the camera.
                 @return the current tilt value.
                 */
            long getZoom() const;
                /**< Returns the current zoom value of the camera.
                 @return the current zoom value.
                 */
    
            void resetPanTilt();
                /**< Resets pan and tilt states to their natural position.
                 */

            /////////////////////
            
            void setPanTiltSpeed(long panSpeed, long tiltSpeed);
                /**< Changes the panning and tilting speed.
                 @param panSpeed the new panning speed.
                 @param tiltSpeed the new tilting speed.
                 */

            void setZoomSpeed(long zoomSpeed);
                /**< Changes the zooming speed.
                 @param zoomSpeed the new zooming speed.
                 */

            long getPanSpeed() const;
                /**< Returns the panning speed.
                 @return the current panning speed.
                 */
            long getTiltSpeed() const;
                /**< Returns the tilting speed.
                 @return the current tilting speed.
                 */
            long getZoomSpeed() const;
                /**< Returns the zooming speed.
                 @return the current zooming speed.
                 */
                 
        private:
            MotorizedCamera(CEVID30 *mcam);
            
        private:
            CEVID30* mcam;
            long panSpeed;
            long tiltSpeed;
            long zoomSpeed;
    
        public:
            const long PAN_MIN;           ///< Minimum pan value.
            const long PAN_MAX;           ///< Maximum pan value.
            const long PAN_SPEED_MIN;     ///< Minimum pan speed.
            const long PAN_SPEED_MAX;     ///< Maximum pan speed.
            const long TILT_MIN;          ///< Minimum tilt value.
            const long TILT_MAX;          ///< Maximum tilt value.
            const long TILT_SPEED_MIN;    ///< Minimum tilt speed.
            const long TILT_SPEED_MAX;    ///< Maximum tilt speed.
            const long ZOOM_MIN;          ///< Minimum zoom value.
            const long ZOOM_MAX;          ///< Maximum zoom value.
            const long ZOOM_SPEED_MIN;    ///< Minimum zoom speed.
            const long ZOOM_SPEED_MAX;    ///< Maximum zoom speed.
        };
    
    }
        
#endif


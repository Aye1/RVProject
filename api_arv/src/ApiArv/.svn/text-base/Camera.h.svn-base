/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 *  remarks :
 *      based on video library furnished with ARToolkit
 *      linux : support V4L, IEEE1394
 */



#ifndef __ARV_CAMERA__
#define __ARV_CAMERA__

#include "Image.h"
#include <string>
#include <AR/video.h>
//#include <sys/time.h>

namespace arv {

    /** Interface with a physical camera.
     * This class allows to read the real-time video stream obtained from a camera.
     * A typical usage is:
     * \code
     *  Camera* camera = new Camera(Camera::SONY, Camera::FORMAT_640x480);
     *  if (camera == NULL) error();
     *  camera->start();
     *  while (acquiring) {
     *      camera->update();
     *      Image image = camera->getImage();
     *      ... // process image
     *      }
     *  camera->stop();
     *  delete camera;
     * \endcode
     */
    class Camera {
        public:
            /** @brief Defines a camera type. */
            enum Type {
                SONY,              /**< @brief Sony EVI-D31 camera on serial port                  */
                PHILIPS,           /**< @brief Philips ToUcam Pro camera on USB port               */
                SONY_COMPOSITE,    /**< @brief Sony EVI-D31 camera on serial port, composite input */
                PHILIPS_SECOND     /**< @brief second Philips ToUcam Pro camera on USB port        */
                };
            
            /** @brief Defines a video format. */
            enum Format {
                FORMAT_160x120,   /**< (QSIF) */
                FORMAT_320x240,   /**< (SIF)  */
                FORMAT_640x480,   /**< (VGA)  */
                FORMAT_768x576    /**< (PAL)  */
                };
    
        public:
            /** Creates a new instance and initializes it.
             * @return the created object or \c NULL if an error occured during initialization.
             * @param type defines the type of camera
             * @param format defines the video format
             */
            static Camera* create(Type type = SONY, Format format = FORMAT_640x480);
            
            ~Camera();
                /**< Frees resources associated with this object. */

            bool start();
                /**< Starts video acquisition.
                 * This method needs to be called before any call to \c update().
                 @return \c true on success, \c false if some error occured.
                 */

            bool update();
                /**< Updates video.
                 * This method needs to be called before getting an image with \c getImage().
                 @return \c true on success, \c false if some error occured.
                 */

            bool stop();
                /**< Stops video acquisition.
                 * Further calls to \c update() or \c getImage() are undefined.
                 @return \c true on success, \c false if some error occured.
                 */
            
            Image getImage();
                /**< Returns the image obtained during the last update.
                 @return the image data obtained during last call to \c update()
                 */
            
            int width()  const { return w; }     /**< The video image width.  \return the video image width for this camera. */
            int height() const { return h; }     /**< The video image height. \return the video image height for this camera. */
            
        protected:
            Camera() : handle(NULL) {}
            bool init(Type type = SONY, Format format = FORMAT_640x480);
        private:
            int w, h;                   // width and height of the images returned by this camera
            AR2VideoParamT *handle;     // the handle with the camera representation for ARToolkit
        };
    
    }

#endif


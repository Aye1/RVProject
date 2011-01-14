/******************************************
 *  creation :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_IMAGE__
#define __ARV_IMAGE__

#include <sys/time.h>

#ifndef NULL
    #define NULL (void*)0
#endif
    

namespace arv {
    typedef struct timeval Time;    // to clarify code
    
    #define TIME_UPDATE(time) gettimeofday(time, NULL)
    
    /** This class represents an image obtained from a camera.
     * It contains its data, size and date of capture.
     * We use GL_BGR as internal representation, for compatibility with
     * output format of cameras. However this prevents to load images into OpenGL
     * textures if their dimensions are not power of two.
     */
    class Image {
        public:
            Image(int w, int h, const char *data, Time t);
            /**< Instantiate a new image.
             @param w image width.
             @param h image height
             @param data a \p w * \p h array containing the data of the image (usually obtained from a camera).
             @param t the time at which this image was captured.
             */
            
            Image(const char* fileName);
                /**< Loads an image from a file.
                 @param fileName file name for a valid image file (png, jpg).
                 */
             
            int width() const { return w; }
                /**< Gets the width of this image.
                 @return the width of this image.
                 */
            int height() const { return h; }
                /**< Gets the height of this image.
                 @return the height of this image.
                 */
            Time time() const { return t; }
                /**< Gets the time at which this image was captured.
                 @return the time at which this image was captured.
                 */
            
            bool defined() const { return data != NULL; }
                /**< Tells if this image contains data.
                 @return \c true iff the image contains data.
                 */
            
            void draw(float zoom = 1.0f) const;
                /**< Draws the image in an OpenGL window using the specified zoom factor.
                 @param zoom the zoom factor.
                 * This method keeps OpenGL matrices unchanged but can modify other OpenGL state (viewport, depth test, lighting).
                 */

            //void draw(int x=0, int y=0) const;
            
            void loadTexture() const;
                /** Loads this image to texture memory.
                 * The image is loaded into currently bound 2D texture.
                 */
            
        private:
            int w, h;
            char *data;
            Time t;
            
        friend class Tracker;   // to allow class Tracker to directly access the data
        };
    
    }

#endif


/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_TRACKER__
#define __ARV_TRACKER__

#include "Image.h"
#include <AR/param.h>
#include <AR/ar.h>

namespace arv {

    //typedef struct timeval TimeVal;
    
    struct Marker {
        int id;
        char name[256];
        double width;
        double center[2];
        double pos[3][4];
        bool visible;
        };
    
    /** Multiple markers tracking.
     * This class uses the ARToolkit Library to provide real-time
     * tracking from video images. It allows to track the 6 degrees-of freedom
     * from multiple trackers from a video stream.
     */
    class Tracker {
        public:
            static Tracker* create(const char* markerFile=NULL, int width=640, int height=480, const char* cameraFile="cameras/camera_para.dat", int threshold=100);
                /**< Creates and initializes a new instance.
                 @param markerFile the file name for markers data. If \c NULL is given, a single marker (\c "patt.hiro") is used.
                 @param width width of the video stream
                 @param height height of the video stream
                 @param cameraFile the file name for camera calibration data. If \c NULL is given, the default file \c "camera_para.dat" is used.
                 @param threshold the threshold used to decide wether a marker can be considered to be detected or not.
                 @return the newly created tracker or \c NULL if some error occured.
                 */

            ~Tracker();
                /**< Frees ressources associated with this object. */
            
            void setThreshold(int threshold);
                /**< Sets the detection threshold value. */
            
            void info();
                /**< Prints informations about this tracker on standard output.  */
            
            bool update(const Image& image);
                /**< Updates tracking infos from input image.
                 * This method needs to be called each time the input video stream is updated.
                 * It recomputes the poses for detected markers on the new image.
                 @param image the input image.
                 @return true on success, false if some error occured.
                 */
            
            int numMarkers() const { return markerNum; }
                /**< Returns the number of markers.
                 @return the number of markers tracked by this tracker.
                 */
            
            bool isMarkerVisible(int m);
                /**< Tells if a marker was found.
                 @param m index of the marker to query.
                 @return true if the marker was visible, false else.
                 */
            
            float markerSize(int m) const { return markers[m].width; }
                /**< Returns the physical size of a marker.
                  @param m index of the marker to query.
                  @return the size (in world space) of the specified marker.
                  */
            
            void getPos(int m, double transfo[3][4]);
                /**< Returns the pose (position and orientation) of the specified detected marker.
                  @param m the index of the marker.
                  @param transfo takes the value of the pose of the marker relative to the camera, if detected.
                  @return the position and orientation of the specified if detected.
                  @warning undefined behavior if the specified marker was not detected.
                  */
            
            void getInversePos(int m, double transfo[3][4]);
                /**< Returns the inverse pose of the specified detected marker.
                  @param m the index of the marker
                  @param transfo takes the value of the inverse pose of the marker, if detected.
                  @warning undefined behavior if the specified marker was not detected.
                  */
            
            void getRelativePos(int marker, int markerRef, double transfo[3][4]);
                /**< Returns the pose of the specified detected marker, relative to another detected marker.
                  @param marker the index of the marker
                  @param markerRef the index of the reference marker
                  @param transfo takes the value of the relative pose of the marker, if both markers detected.
                  @warning undefined behavior if the specified markers were not detected.
                  */
            
            
            // /* Get screen position of detected marker */
            //void getPos2D(int marker_request, double vertex_pos[4][4]);
            
            void getGLModelMatrix(int marker, double matrix[16]);
                /**< Gets the OpenGL model-view matrix corresponding to the specified detected marker.
                 * The matrix obtained this way can be used in OpenGL calls:
                 @code
                 *  glMatrixMode(GL_MODELVIEW);
                 *  glLoadMatrixd(matrix);
                 @endcode
                 @param marker the index of the marker
                 @param matrix takes the matrix corresponding to the pose of the specified marker if detected,
                        in the format of an OpenGL model(view) matrix.
                 */
            
            void getGLProjectionMatrix(float near, float far, double matrix[16]);
                /**< Gets the OpenGL projection matrix corresponding to the current camera (its intrinsic parameters).
                 * The matrix obtained this way can be used in OpenGL calls:
                 @code
                 *  glMatrixMode(GL_PROJECTION);
                 *  glLoadMatrixd(matrix);
                 @endcode
                 @param near distance to the near plane of the projection matrix
                 @param far distance to the far plane of the projection matrix
                 @param matrix takes the matrix corresponding to the intrinsic parameters of the camera associated
                        with this tracker, in the format of an OpenGL projection matrix.
                 */
            
        private:
            Tracker(Marker *markers, int markerNum, ARParam cameraParam, int width, int height, int threshold) :
                markers(markers), markerNum(markerNum), cameraParam(cameraParam), width(width), height(height), threshold(threshold) {}
        private:
            Marker* markers;        // the list of markers
            int markerNum;          // number of markers
            ARParam cameraParam;    // camera parameters
            int width, height;      // video image size
            int threshold;          // detection threshold
        };
    
    }

#endif


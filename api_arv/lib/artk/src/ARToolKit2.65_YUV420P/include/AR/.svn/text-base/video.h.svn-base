/*******************************************************
 *
 * Author: Hirokazu Kato, Atsishi Nakazawa
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *         nakazawa@inolab.sys.es.osaka-u.ac.jp
 *
 * Revision: 4.3
 * Date: 2002/01/01
 *
*******************************************************/
#ifndef AR_VIDEO_H
#define AR_VIDEO_H
#ifdef  __cplusplus
extern "C" {
#endif

#include <AR/config.h>
#include <AR/ar.h>


#ifdef __linux
#ifdef AR_INPUT_V4L
#include <AR/sys/videoLinuxV4L.h>
#endif

#ifdef  AR_INPUT_DV
#include <AR/sys/videoLinuxDV.h>
#endif

#ifdef  AR_INPUT_1394CAM
#include <AR/sys/videoLinux1394Cam.h>
#endif
#endif

#ifdef __sgi
#include <AR/sys/videoSGI.h>
#endif

#ifdef __APPLE__
#include <AR/sys/videoMacOSX.h>
#endif


#include <sys/time.h>


int        arVideoOpen( char *config );
int        arVideoClose( void );
int        arVideoDispOption( void );

int        arVideoInqSize( int *x, int *y );
ARUint8   *arVideoGetImage( void );

int        arVideoCapStart( void );
int        arVideoCapStop( void );
int        arVideoCapNext( void );



int             ar2VideoDispOption ( void );
AR2VideoParamT *ar2VideoOpen       ( char *config );
int             ar2VideoClose      ( AR2VideoParamT *vid );
int             ar2VideoCapStart   ( AR2VideoParamT *vid );

int             ar2VideoCapNext    ( AR2VideoParamT *vid );
int 		ar3VideoCapNext	   ( AR2VideoParamT *vid, struct timeval *timeStr );

int             ar2VideoCapStop    ( AR2VideoParamT *vid );

ARUint8        *ar2VideoGetImage   ( AR2VideoParamT *vid );
ARUint8        *ar3VideoGetImage   ( AR2VideoParamT *vid, struct timeval *timeStr );

int             ar2VideoInqSize    ( AR2VideoParamT *vid, int *x,int *y );

#ifdef  __cplusplus
}
#endif
#endif

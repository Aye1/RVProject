#ifndef AR_VIDEO_MACOSX_H
#define AR_VIDEO_MACOSX_H
#ifdef  __cplusplus
extern "C" {
#endif

#if __APPLE_CC__
#include <Carbon/Carbon.h>
#include <QuicKTime/QuickTime.h>
#endif

#include <AR/config.h>
#include <AR/ar.h>

typedef struct {
    int                    width;
    int                    height;
    Rect                   theRect;
    SeqGrabComponent       seqGrab;
    SGChannel              sgchanVideo;
    GWorldPtr              pGWorld;
    ImageSequence          decomSeq;
    ImageDescriptionHandle imageDesc;
    int                    status;
    ARUint8               *imgPtr;
    int                    imgLen;
} AR2VideoParamT;

#ifdef  __cplusplus
}
#endif
#endif

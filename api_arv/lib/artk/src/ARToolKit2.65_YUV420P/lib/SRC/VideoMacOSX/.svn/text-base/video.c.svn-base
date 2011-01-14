/* 
 *   Video capture subrutine for Linux/libdc1394 devices
 *   author: Kiyoshi Kiyokawa ( kiyo@crl.go.jp )
             Hirokazu Kato ( kato@sys.im.hiroshima-cu.ac.jp )
 *
 *   Revision: 1.0   Date: 2002/01/01
 */
#if __APPLE_CC__
#include <Carbon/Carbon.h>
#include <QuicKTime/QuickTime.h>
#endif
#include <AR/ar.h>
#include <AR/config.h>
#include <AR/video.h>

#define BailErr(x)  {err = x; if(err != noErr) return -1;}
#define BailErr2(x) {err = x; if(err != noErr) return NULL;}

static AR2VideoParamT        *vid = NULL;


int arVideoDispOption( void )
{
    return ar2VideoDispOption();
}

int arVideoOpen( char *config )
{
    if( vid != NULL ) {
        printf("Device has been opened!!\n");
        return -1;
    }
    vid = ar2VideoOpen( config );
    if( vid == NULL ) return -1;

    return 0;
}

int arVideoClose( void )
{
    if( vid == NULL ) return -1;
  
    return ar2VideoClose( vid );
}  
    
int arVideoInqSize( int *x, int *y )
{
    if( vid == NULL ) return -1;
 
    return ar2VideoInqSize( vid, x, y );
}       
        
ARUint8 *arVideoGetImage( void )
{   
    if( vid == NULL ) return NULL;

    return ar2VideoGetImage( vid );
}
 
int arVideoCapStart( void )
{
    if( vid == NULL ) return -1;
  
    return ar2VideoCapStart( vid );
}  
    
int arVideoCapStop( void )
{
    if( vid == NULL ) return -1;
 
    return ar2VideoCapStop( vid );
}       
        
int arVideoCapNext( void )
{   
    if( vid == NULL ) return -1;  

    return ar2VideoCapNext( vid ); 
}

/*-------------------------------------------*/

pascal OSErr mySGDataProc(SGChannel c, Ptr p, long len, long *offset, long chRefCon, TimeValue time, short writeType, long refCon);


int ar2VideoDispOption( void )
{
    printf(" -width=N\n");
    printf("    specifies expected width of image.\n");
    printf(" -height=N\n");
    printf("    specifies expected height of image.\n");
    printf("\n");

    return 0;
}


AR2VideoParamT *ar2VideoOpen( char *config )
{
    static int                initF = 0;
    OSErr                     err = noErr;
    AR2VideoParamT            *vid;
    char                      *a, line[256];
    int                       i;

    arMalloc( vid, AR2VideoParamT, 1 );
    vid->width          = DEFAULT_VIDEO_WIDTH;
    vid->height         = DEFAULT_VIDEO_HEIGHT;
    vid->theRect.top    = 0;
    vid->theRect.left   = 0;
    vid->theRect.bottom = DEFAULT_VIDEO_HEIGHT - 1;
    vid->theRect.right  = DEFAULT_VIDEO_WIDTH  - 1;
    vid->pGWorld        = NULL;
    vid->seqGrab        = NULL;
    vid->sgchanVideo    = NULL;
    vid->decomSeq       = NULL;
    vid->imageDesc      = NULL;
    vid->status         = 0;
    vid->imgPtr         = NULL;
    vid->imgLen         = 0;

    a = config;
    if( a != NULL) {
        for(;;) {
            while( *a == ' ' || *a == '\t' ) a++;
            if( *a == '\0' ) break;

            if( strncmp( a, "-width=", 7 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[7], "%d", &vid->width ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
                vid->theRect.right = vid->width - 1;
            }
            else if( strncmp( a, "-height=", 8 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[8], "%d", &vid->height ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
                vid->theRect.bottom = vid->height - 1;
            }
            else {
                ar2VideoDispOption();
                free( vid );
                return 0;
            }

            while( *a != ' ' && *a != '\t' && *a != '\0') a++;
        }           
    }           

    if( initF == 0 ) {
        InitCursor();
        EnterMovies();
        initF = 1;
    }

    if( vid->pGWorld == NULL ) {
        err = QTNewGWorld( &(vid->pGWorld), k32ARGBPixelFormat, &(vid->theRect),
                           0, NULL, 0);
        BailErr2(err);
        if(!LockPixels(GetPortPixMap(vid->pGWorld))) BailErr2(-1);
    }
    
    vid->seqGrab = OpenDefaultComponent(SeqGrabComponentType, 0);
    err = SGInitialize(vid->seqGrab);
    BailErr2(err);
    
    err = SGSetDataRef(vid->seqGrab, 0, 0, seqGrabDontMakeMovie);
    BailErr2(err);
     
    err = SGSetGWorld(vid->seqGrab, vid->pGWorld, NULL);
    //err = SGSetGWorld(vid->seqGrab, vid->pGWorld, GetMainDevice());
    BailErr2(err);
    
    err = SGNewChannel(vid->seqGrab, VideoMediaType, &(vid->sgchanVideo));
    BailErr2(err);

    err = SGSetChannelBounds(vid->sgchanVideo, &(vid->theRect));
    BailErr2(err);

    err = SGSetChannelUsage(vid->sgchanVideo, seqGrabRecord);
    BailErr2(err);
    
    err = SGSetDataProc(vid->seqGrab, NewSGDataUPP(mySGDataProc), (long)vid);
    BailErr2(err);

    return vid;
}

int ar2VideoClose( AR2VideoParamT *vid )
{
    if( vid->seqGrab != NULL ) {
    	CloseComponent(vid->seqGrab);
        vid->seqGrab = NULL;
    }
    
    if( vid->pGWorld != NULL ) {
        DisposeGWorld(vid->pGWorld);
        vid->pGWorld = NULL;
    }

    if( vid->decomSeq != NULL ) {
        CDSequenceEnd(vid->decomSeq);
        vid->decomSeq = NULL;
    }

    if( vid->imageDesc != NULL ) {
        DisposeHandle((Handle)(vid->imageDesc));
        vid->imageDesc = NULL;
    }

    free( vid );

    return 0;

}

int ar2VideoCapStart( AR2VideoParamT *vid )
{
    SGStartRecord(vid->seqGrab);
    vid->status = 0;

    return 0;
}

int ar2VideoCapNext( AR2VideoParamT *vid )
{
    return 0;
}

int ar2VideoCapStop( AR2VideoParamT *vid )
{   
    if(vid->seqGrab) SGStop(vid->seqGrab);

    return 0;
}

int ar2VideoInqSize(AR2VideoParamT *vid, int *x,int *y)
{
    *x = vid->width;
    *y = vid->height;

    return 0;
}

ARUint8 *ar2VideoGetImage( AR2VideoParamT *vid )
{   
    CodecFlags 		ignore;
    OSErr		err = noErr;

    while(!err && vid->status == 0) {
    	err = SGIdle(vid->seqGrab);
    }

    if( vid->decomSeq == NULL ) {
        err = DecompressSequenceBegin( &(vid->decomSeq), vid->imageDesc, vid->pGWorld,
            			       NULL, &(vid->theRect), NULL, srcCopy, NULL, 0,
                                       codecNormalQuality, bestSpeedCodec);
        BailErr2(err);
    }

    err = DecompressSequenceFrameS( vid->decomSeq, vid->imgPtr, vid->imgLen, 0, &ignore, NULL);
    BailErr2(err);

    vid->status = 0;
    return GetPixBaseAddr(GetGWorldPixMap(vid->pGWorld))+1;
}


pascal OSErr mySGDataProc(SGChannel c, Ptr p, long len, long *offset, long chRefCon, TimeValue time, short writeType, long refCon)
{
#pragma unused(offset,chRefCon,time,writeType)
    AR2VideoParamT        *vid;

    vid = (AR2VideoParamT *)refCon;

    if( vid->imageDesc == NULL ) {
        vid->imageDesc = (ImageDescriptionHandle)NewHandle(0);
        SGGetChannelSampleDescription(c,(Handle)(vid->imageDesc));
        printf("CTYPE = %c%c%c%c\n", 
            ((*(vid->imageDesc))->cType >> 24) & 0xFF,
            ((*(vid->imageDesc))->cType >> 16) & 0xFF,
            ((*(vid->imageDesc))->cType >>  8) & 0xFF,
            ((*(vid->imageDesc))->cType >>  0) & 0xFF );
    }

    vid->status = 1;
    vid->imgPtr = (ARUint8 *)p;
    vid->imgLen = len;

    return noErr;
}

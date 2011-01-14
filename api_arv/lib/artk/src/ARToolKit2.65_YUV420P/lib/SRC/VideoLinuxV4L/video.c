/* 
 *   Video capture subrutine for Linux/Video4Linux devices 
 *   author: Nakazawa,Atsushi ( nakazawa@inolab.sys.es.osaka-u.ac.jp )
             Hirokazu Kato ( kato@sys.im.hiroshima-cu.ac.jp )
 *
 *   Revision: 5.2   Date: 2000/08/25
 */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include <linux/videodev.h>
#include <AR/config.h>
#include <AR/ar.h>
#include <AR/video.h>

#define MAXCHANNEL   10

static AR2VideoParamT   *vid = NULL;

int arVideoDispOption( void )
{
    return  ar2VideoDispOption();
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

int ar2VideoDispOption( void )
{
    printf(" -width=N\n");
    printf("    specifies expected width of image.\n");
    printf(" -height=N\n");
    printf("    specifies expected height of image.\n");
    printf(" -contrast=N\n");
    printf("    specifies contrast. (0.0 <-> 1.0)\n");
    printf(" -brightness=N\n");
    printf("    specifies brightness. (0.0 <-> 1.0)\n");
    printf(" -color=N\n");
    printf("    specifies color. (0.0 <-> 1.0)\n");
    printf(" -channel=N\n");
    printf("    specifies source channel.\n");
    printf(" -dev=filepath\n");
    printf("    specifies device file.\n");
    printf(" -mode=[PAL|NTSC|SECAM]\n");
    printf("    specifies TV signal mode.\n");
    printf(" -palette=[RGB|YUV420P]\n");
    printf("    specifies the camera palette.\n");
    printf("\n");

    return 0;
}

AR2VideoParamT *ar2VideoOpen( char *config )
{
    AR2VideoParamT            *vid;
    struct video_capability   vd;
    struct video_channel      vc[MAXCHANNEL];
    struct video_picture      vp;
    char                      *a, line[256];
    int                       i;

    arMalloc( vid, AR2VideoParamT, 1 );
    strcpy( vid->dev, DEFAULT_VIDEO_DEVICE );
    vid->width      = DEFAULT_VIDEO_WIDTH;
    vid->height     = DEFAULT_VIDEO_HEIGHT;
    vid->channel    = DEFAULT_VIDEO_CHANNEL;
    vid->mode       = DEFAULT_VIDEO_MODE;
#if defined(AR_PIX_FORMAT_BGRA)
    vid->int_mode = VIDEO_PALETTE_RGB32;     /* palette format */
#elif defined(AR_PIX_FORMAT_BGR)
    vid->int_mode = VIDEO_PALETTE_RGB24;     /* palette format */
#endif
    vid->debug      = 0;
    vid->contrast   = 0.5;
    vid->brightness = 0.5;
    vid->color      = 0.5;

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
            }
            else if( strncmp( a, "-height=", 8 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[8], "%d", &vid->height ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-contrast=", 10 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[10], "%lf", &vid->contrast ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-brightness=", 12 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[12], "%lf", &vid->brightness ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-color=", 7 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[7], "%lf", &vid->color ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-channel=", 9 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[9], "%d", &vid->channel ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-dev=", 5 ) == 0 ) {
                sscanf( a, "%s", line );
                if( sscanf( &line[5], "%s", vid->dev ) == 0 ) {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-mode=", 6 ) == 0 ) {
                if( strncmp( &a[6], "PAL", 3 ) == 0 )        vid->mode = VIDEO_MODE_PAL;
                else if( strncmp( &a[6], "NTSC", 4 ) == 0 )  vid->mode = VIDEO_MODE_NTSC;
                else if( strncmp( &a[6], "SECAM", 5 ) == 0 ) vid->mode = VIDEO_MODE_SECAM;
                else {
                    ar2VideoDispOption();
                    free( vid );
                    return 0;
                }
            }
            else if( strncmp( a, "-palette=", 9 ) == 0 ) {
                if( strncmp( &a[9], "RGB", 3 ) == 0 ) {
#if defined(AR_PIX_FORMAT_BGRA)
                    vid->int_mode = VIDEO_PALETTE_RGB32;     /* palette format */
#elif defined(AR_PIX_FORMAT_BGR)
                    vid->int_mode = VIDEO_PALETTE_RGB24;     /* palette format */
#endif
                }
                else if( strncmp( &a[9], "YUV420P", 7 ) == 0 ) vid->int_mode = VIDEO_PALETTE_YUV420P;
            }
            else if( strncmp( a, "-debug", 6 ) == 0 ) {
                vid->debug = 1;
            }
            else {
                ar2VideoDispOption();
                free( vid );
                return 0;
            }

            while( *a != ' ' && *a != '\t' && *a != '\0') a++;
        }
    }

    vid->fd = open(vid->dev, O_RDWR);
    if(vid->fd < 0){
        printf("video device (%s) open failed\n",vid->dev); 
        free( vid );
        return 0;
    }

    if(ioctl(vid->fd,VIDIOCGCAP,&vd) < 0){
        printf("ioctl failed\n");
        free( vid );
        return 0;
    }

    if( vid->debug ) {
        printf("=== debug info ===\n");
        printf("  vd.name      =   %s\n",vd.name);
        printf("  vd.channels  =   %d\n",vd.channels);
        printf("  vd.maxwidth  =   %d\n",vd.maxwidth);
        printf("  vd.maxheight =   %d\n",vd.maxheight);
        printf("  vd.minwidth  =   %d\n",vd.minwidth);
        printf("  vd.minheight =   %d\n",vd.minheight);
    }
    
    /* check capture size */
    if(vd.maxwidth  < vid->width  || vid->width  < vd.minwidth ||
       vd.maxheight < vid->height || vid->height < vd.minheight ) {
        printf("arVideoOpen: width or height oversize \n");
        free( vid );
        return 0;
    }

    /* check channel */
    if(vid->channel < 0 || vid->channel >= vd.channels){
        printf("arVideoOpen: channel# is not valid. \n");
        free( vid );
        return 0;
    }

    if( vid->debug ) {
        printf("==== capture device channel info ===\n");
    }

    for(i = 0;i < vd.channels && i < MAXCHANNEL; i++){
        vc[i].channel = i;
        if(ioctl(vid->fd,VIDIOCGCHAN,&vc[i]) < 0){
            printf("error: acquireing channel(%d) info\n",i);
            free( vid );
            return 0;
        }

        if( vid->debug ) {
            printf("    channel = %d\n",  vc[i].channel);
            printf("       name = %s\n",  vc[i].name);
            printf("     tuners = %d",    vc[i].tuners);

            printf("       flag = 0x%08x",vc[i].flags);
            if(vc[i].flags & VIDEO_VC_TUNER) 
                printf(" TUNER");
            if(vc[i].flags & VIDEO_VC_AUDIO) 
                printf(" AUDIO");
            printf("\n");

            printf("     vc[%d].type = 0x%08x", i, vc[i].type);
            if(vc[i].type & VIDEO_TYPE_TV) 
                printf(" TV");
            if(vc[i].type & VIDEO_TYPE_CAMERA) 
                printf(" CAMERA");
            printf("\n");       
        }
    }

    /* select channel */
    vc[vid->channel].norm = vid->mode;       /* 0: PAL 1: NTSC 2:SECAM 3:AUTO */
    if(ioctl(vid->fd, VIDIOCSCHAN, &vc[vid->channel]) < 0){
        printf("error: selecting channel %d\n", vid->channel);
        free( vid );
        return 0;
    }

    /* set video picture */
    vp.brightness = 32767 * 2.0 * vid->brightness;
    vp.hue        = 32767;
    vp.colour     = 32767 * 2.0 * vid->color;
    vp.contrast   = 32767 * 2.0 * vid->contrast;
    vp.whiteness  = 32767;
    vp.depth      = 24;                      /* color depth    */
#if defined(AR_PIX_FORMAT_BGRA)
    vp.palette    = VIDEO_PALETTE_RGB32;     /* palette format */
#elif defined(AR_PIX_FORMAT_BGR)
    vp.palette    = VIDEO_PALETTE_RGB24;     /* palette format */
#endif

    if( vid->int_mode != vp.palette ) vp.palette = vid->int_mode;

    if(ioctl(vid->fd, VIDIOCSPICT, &vp)) {
        printf("error: setting palette\n");
        free( vid );
        return 0;
    }

    /* get mmap info */
    if(ioctl(vid->fd,VIDIOCGMBUF,&vid->vm) < 0){
        printf("error: videocgmbuf\n");
        free( vid );
        return 0;
    }

    if( vid->debug ) {
        printf("===== Image Buffer Info =====\n");
        printf("   size   =  %d[bytes]\n", vid->vm.size);
        printf("   frames =  %d\n", vid->vm.frames);
    }
    if(vid->vm.frames < 2){
        printf("this device can not be supported by libARvideo.\n");
        printf("(vm.frames < 2)\n");
        free( vid );
        return 0;
    }


    /* get memory mapped io */
    if((vid->map = (ARUint8 *)mmap(0, vid->vm.size, PROT_READ|PROT_WRITE, MAP_SHARED, vid->fd, 0)) < 0){
        printf("error: mmap\n");
        free( vid );
        return 0;
    }

    /* setup for vmm */ 
    vid->vmm.frame  = 0;
    vid->vmm.width  = vid->width;
    vid->vmm.height = vid->height;

    vid->vmm.format = vid->int_mode;
/*
#if defined(AR_PIX_FORMAT_BGRA)
    vid->vmm.format = VIDEO_PALETTE_RGB32;
#elif defined(AR_PIX_FORMAT_BGR)
    vid->vmm.format = VIDEO_PALETTE_RGB24;
#endif
*/

    vid->image = NULL;

#if defined(AR_PIX_FORMAT_BGRA)
    if( vid->int_mode != VIDEO_PALETTE_RGB32 )
        arMalloc( vid->image, ARUint8, (vid->vmm.width * vid->vmm.height * AR_PIX_SIZE) );
#elif defined(AR_PIX_FORMAT_BGR)
    if( vid->int_mode != VIDEO_PALETTE_RGB24 )
        arMalloc( vid->image, ARUint8, (vid->vmm.width * vid->vmm.height * AR_PIX_SIZE) );
#endif

    vid->video_cont_num = -1;

    return vid;
}

int ar2VideoClose( AR2VideoParamT *vid )
{
    if(vid->video_cont_num >= 0){
        ar2VideoCapStop( vid );
    }
    close(vid->fd);
    if( vid->image != NULL ) free( vid->image );
    free( vid );

    return 0;
} 


int ar2VideoCapStart( AR2VideoParamT *vid )
{
    if(vid->video_cont_num >= 0){
        printf("arVideoCapStart has already been called.\n");
        return -1;
    }

    vid->video_cont_num = 0;
    vid->vmm.frame      = vid->video_cont_num;
    if(ioctl(vid->fd, VIDIOCMCAPTURE, &vid->vmm) < 0) {
        return -1;
    }                                  
    vid->vmm.frame = 1 - vid->vmm.frame;
    if( ioctl(vid->fd, VIDIOCMCAPTURE, &vid->vmm) < 0) {
        return -1;
    }

    return 0;
}

int ar2VideoCapNext( AR2VideoParamT *vid )
{
    if(vid->video_cont_num < 0){
        printf("arVideoCapStart has never been called.\n");
        return -1;
    }

    vid->vmm.frame = 1 - vid->vmm.frame;
    ioctl(vid->fd, VIDIOCMCAPTURE, &vid->vmm);

    return 0;
}


int ar3VideoCapNext( AR2VideoParamT *vid, struct timeval *timeStr )
{
    if(vid->video_cont_num < 0){
        printf("arVideoCapStart has never been called.\n");
        return -1;
    }

    vid->vmm.frame = 1 - vid->vmm.frame;
    ioctl(vid->fd, VIDIOCMCAPTURE, &vid->vmm);
    
    gettimeofday(timeStr, NULL);
    
    return 0;
}



int ar2VideoCapStop( AR2VideoParamT *vid )
{
    if(vid->video_cont_num < 0){
        printf("arVideoCapStart has never been called.\n");
        return -1;
    }
    if(ioctl(vid->fd, VIDIOCSYNC, &vid->video_cont_num) < 0){
        printf("error: videosync\n");
        return -1;
    }
    vid->video_cont_num = -1;

    return 0;

}


ARUint8 *ar3VideoGetImage( AR2VideoParamT *vid, struct timeval *timeStr )
{
	register ARUint8 *buf, *bufNextLine;   /* buf gets an internal image buffer for palette conversion */
	register ARUint8 *buf2;  /* buf2 gets the capture buffer */
	register ARUint8 *Y_offset, *U_offset, *V_offset, *Y_offsetNextLine;
	register int row, col;
	register int Y0, Y1, Y2, Y3, U, V, UV, V2, U5, R, G, B;
	register int img_width, buf_width;  /* buf_width is img_width times 3 for BGA, or times 4 for BGRA */


	
    if(vid->video_cont_num < 0){
        printf("arVideoCapStart has never been called.\n");
        return NULL;
    }

    if(ioctl(vid->fd, VIDIOCSYNC, &vid->video_cont_num) < 0){
        printf("error: videosync\n");
        return NULL;
    }
    vid->video_cont_num = 1 - vid->video_cont_num;

    if(vid->video_cont_num == 0)
        buf2 = (vid->map + vid->vm.offsets[1]); 
    else
        buf2 = (vid->map + vid->vm.offsets[0]);


    gettimeofday(timeStr, NULL);


#if defined(AR_PIX_FORMAT_BGRA)
    if( vid->int_mode == VIDEO_PALETTE_RGB32 ) return buf2;
#elif defined(AR_PIX_FORMAT_BGR)
    if( vid->int_mode == VIDEO_PALETTE_RGB24 ) return buf2;
#endif
        
    switch ( vid->int_mode )
    {
        case VIDEO_PALETTE_YUV420P:

            img_width = vid->vmm.width;
#if defined(AR_PIX_FORMAT_BGRA)
            buf_width = 4 * img_width;
#else
            buf_width = 3 * img_width;
#endif

            buf = vid->image;
            bufNextLine = vid->image + buf_width;
            /* YUV420P is planar mode: first come all Y-values (1 byte for
               each pixel), then come all U-values (1 byte for 2x2 pixels),
               then all V-values (1 byte for 2x2 pixels).
            */
            Y_offset = buf2;
            Y_offsetNextLine = buf2 + img_width;
            U_offset = Y_offset + (vid->vmm.width * vid->vmm.height);
            V_offset = U_offset + (vid->vmm.width * vid->vmm.height) / 4;

// handle two lines simultaneously!
            for ( row = vid->vmm.height/2; row; row-- )
            {
                for ( col = vid->vmm.width/2; col; col-- )
                {
                    U   = ((ARUint8)*U_offset++ - 128);// * 0.354;
                    U5  = 5*U;
                    Y0  = (ARUint8)*Y_offset++;
                    Y1  = (ARUint8)*Y_offset++;
                    V   = ((ARUint8)*V_offset++ - 128);// * 0.707;
                    V2  = 2*V;
                    Y2  = (ARUint8)*Y_offsetNextLine++;
                    Y3  = (ARUint8)*Y_offsetNextLine++;
                    UV  = - U - V;

                    // Original equations
                    // R = Y           + 1.402 V
                    // G = Y - 0.344 U - 0.714 V
                    // B = Y + 1.772 U

                    R = Y0 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y0 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y0 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *buf++ = (ARUint8)B;
                    *buf++ = (ARUint8)G;
                    *buf++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *buf++ = (ARUint8)0;
#endif

                    //---
                    R = Y1 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y1 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y1 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *buf++ = (ARUint8)B;
                    *buf++ = (ARUint8)G;
                    *buf++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *buf++ = (ARUint8)0;
#endif

                    //---
                    R = Y2 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y2 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y2 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *bufNextLine++ = (ARUint8)B;
                    *bufNextLine++ = (ARUint8)G;
                    *bufNextLine++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *bufNextLine++ = (ARUint8)0;
#endif

                    //---
                    R = Y3 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y3 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y3 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *bufNextLine++ = (ARUint8)B;
                    *bufNextLine++ = (ARUint8)G;
                    *bufNextLine++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *bufNextLine++ = (ARUint8)0;
#endif
                }
				Y_offset += img_width;
				Y_offsetNextLine += img_width;
				buf += buf_width;
				bufNextLine += buf_width;
            }
            return vid->image;
    }
}




















ARUint8 *ar2VideoGetImage( AR2VideoParamT *vid )
{
	register ARUint8 *buf, *bufNextLine;   /* buf gets an internal image buffer for palette conversion */
	register ARUint8 *buf2;  /* buf2 gets the capture buffer */
	register ARUint8 *Y_offset, *U_offset, *V_offset, *Y_offsetNextLine;
	register int row, col;
	register int Y0, Y1, Y2, Y3, U, V, UV, V2, U5, R, G, B;
	register int img_width, buf_width;  /* buf_width is img_width times 3 for BGA, or times 4 for BGRA */

	
    if(vid->video_cont_num < 0){
        printf("arVideoCapStart has never been called.\n");
        return NULL;
    }

    if(ioctl(vid->fd, VIDIOCSYNC, &vid->video_cont_num) < 0){
        printf("error: videosync\n");
        return NULL;
    }
    vid->video_cont_num = 1 - vid->video_cont_num;

    if(vid->video_cont_num == 0)
        buf2 = (vid->map + vid->vm.offsets[1]); 
    else
        buf2 = (vid->map + vid->vm.offsets[0]);

#if defined(AR_PIX_FORMAT_BGRA)
    if( vid->int_mode == VIDEO_PALETTE_RGB32 ) return buf2;
#elif defined(AR_PIX_FORMAT_BGR)
    if( vid->int_mode == VIDEO_PALETTE_RGB24 ) return buf2;
#endif
        
    switch ( vid->int_mode )
    {
        case VIDEO_PALETTE_YUV420P:

            img_width = vid->vmm.width;
#if defined(AR_PIX_FORMAT_BGRA)
            buf_width = 4 * img_width;
#else
            buf_width = 3 * img_width;
#endif

            buf = vid->image;
            bufNextLine = vid->image + buf_width;
            /* YUV420P is planar mode: first come all Y-values (1 byte for
               each pixel), then come all U-values (1 byte for 2x2 pixels),
               then all V-values (1 byte for 2x2 pixels).
            */
            Y_offset = buf2;
            Y_offsetNextLine = buf2 + img_width;
            U_offset = Y_offset + (vid->vmm.width * vid->vmm.height);
            V_offset = U_offset + (vid->vmm.width * vid->vmm.height) / 4;

// handle two lines simultaneously!
            for ( row = vid->vmm.height/2; row; row-- )
            {
                for ( col = vid->vmm.width/2; col; col-- )
                {
                    U   = ((ARUint8)*U_offset++ - 128);// * 0.354;
                    U5  = 5*U;
                    Y0  = (ARUint8)*Y_offset++;
                    Y1  = (ARUint8)*Y_offset++;
                    V   = ((ARUint8)*V_offset++ - 128);// * 0.707;
                    V2  = 2*V;
                    Y2  = (ARUint8)*Y_offsetNextLine++;
                    Y3  = (ARUint8)*Y_offsetNextLine++;
                    UV  = - U - V;

                    // Original equations
                    // R = Y           + 1.402 V
                    // G = Y - 0.344 U - 0.714 V
                    // B = Y + 1.772 U

                    R = Y0 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y0 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y0 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *buf++ = (ARUint8)B;
                    *buf++ = (ARUint8)G;
                    *buf++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *buf++ = (ARUint8)0;
#endif

                    //---
                    R = Y1 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y1 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y1 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *buf++ = (ARUint8)B;
                    *buf++ = (ARUint8)G;
                    *buf++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *buf++ = (ARUint8)0;
#endif

                    //---
                    R = Y2 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y2 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y2 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *bufNextLine++ = (ARUint8)B;
                    *bufNextLine++ = (ARUint8)G;
                    *bufNextLine++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *bufNextLine++ = (ARUint8)0;
#endif

                    //---
                    R = Y3 + V2;
                    if ((R >> 8) > 0) R = 255; else if (R < 0) R = 0;

                    G = Y3 + UV;
                    if ((G >> 8) > 0) G = 255; else if (G < 0) G = 0;

                    B = Y3 + U5;
                    if ((B >> 8) > 0) B = 255; else if (B < 0) B = 0;

                    *bufNextLine++ = (ARUint8)B;
                    *bufNextLine++ = (ARUint8)G;
                    *bufNextLine++ = (ARUint8)R;
#if defined(AR_PIX_FORMAT_BGRA)
                    *bufNextLine++ = (ARUint8)0;
#endif
                }
				Y_offset += img_width;
				Y_offsetNextLine += img_width;
				buf += buf_width;
				bufNextLine += buf_width;
            }
            return vid->image;
    }
}

int ar2VideoInqSize(AR2VideoParamT *vid, int *x,int *y)
{
    *x = vid->vmm.width;
    *y = vid->vmm.height;

    return 0;
}

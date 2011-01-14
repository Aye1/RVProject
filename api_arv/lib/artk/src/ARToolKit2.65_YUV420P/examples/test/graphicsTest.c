/*
** simpleTest.c - test file to demonstrate the ARToolkit tracking code
**
** author: Mark Billinghurst, grof@hitl.washington.edu
**
** August 18th 1999
*/

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/ar.h>

#define   CPARA   "../../bin/Data/camera_para.dat"

ARParam         cparam;
int             xsize = 640;
int             ysize = 480;
ARUint8         dataPtr[640*480*AR_PIX_SIZE];
ARMarkerInfo    *marker_info;
int             marker_num;
int             count = 0;
int             imgMode = 0;



/* function definitions */
static int    init( int argc, char *argv[] );
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static void   dispMode(void);
static void   gen_image( int mode );

main(int argc, char **argv)
{
    if( init( argc, argv ) < 0 ) exit(0);

    argMainLoop( NULL, keyEvent, mainLoop );
}

static void   keyEvent( unsigned char key, int x, int y)
{
    if( key == ' ' ) {
        imgMode = (imgMode+1)%3;
        gen_image( imgMode );
    }

    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        /* print out frame/sec and shut everything down */
        cleanup();
        exit(0);
    }

    /* turn on and off the debug mode with right mouse */
    if( key == 'd' ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        arDebug = 1 - arDebug;
        if( arDebug == 0 ) {
            glClearColor( 0.0, 0.0, 0.0, 0.0 );
            glClear(GL_COLOR_BUFFER_BIT);
            argSwapBuffers();
            glClear(GL_COLOR_BUFFER_BIT);
            argSwapBuffers();
        }
        count = 0;
        dispMode();
    }

    if( key == 'c' ) {
        if( argDrawMode == AR_DRAW_BY_GL_DRAW_PIXELS ) {
            argDrawMode  = AR_DRAW_BY_TEXTURE_MAPPING;
            argTexmapMode = AR_DRAW_TEXTURE_FULL_IMAGE;
		}
		else if( argTexmapMode == AR_DRAW_TEXTURE_FULL_IMAGE ) {
            argTexmapMode = AR_DRAW_TEXTURE_HALF_IMAGE;
		}
		else {
            argDrawMode  = AR_DRAW_BY_GL_DRAW_PIXELS;
		}
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        count = 0;
        dispMode();
    }
}

/* main loop */
static void mainLoop(void)
{
    int             i, j, k;

    if( count == 100 ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        count = 0;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0, 0 );

    /* if the debug mode is on draw squares 
       around the detected squares in the video image */
    if( arDebug ) {
        argDispImage( dataPtr, 1, 1 );
        if( arImageProcMode == AR_IMAGE_PROC_IN_HALF )
            argDispHalfImage( arImage, 2, 1 );
        else
            argDispImage( arImage, 2, 1);

        glColor3f( 1.0, 0.0, 0.0 );
        glLineWidth( 3.0 );
        for( i = 0; i < marker_num; i++ ) {
            if( marker_info[i].id < 0 ) continue;
            argDrawSquare( marker_info[i].vertex, 2, 1 );
        }
        glLineWidth( 1.0 );
    }

    argSwapBuffers();
}

/* set up the application parameters - read in from command line*/
static int init( int argc, char *argv[] )
{
    ARParam  wparam;

    /* open the video path */
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(CPARA, 1, &wparam) < 0 ) {
       printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );

    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 2, 1, 0 );


    arFittingMode   = AR_FITTING_TO_IDEAL;
    arImageProcMode = AR_IMAGE_PROC_IN_HALF;
    argDrawMode     = AR_DRAW_BY_TEXTURE_MAPPING;
    argTexmapMode   = AR_DRAW_TEXTURE_FULL_IMAGE;
    gen_image( imgMode );

    dispMode();

    return 0;
}

static void dispMode( void )
{
	if( arFittingMode == AR_FITTING_TO_INPUT ) {
		printf("FittingMode (Z): INPUT IMAGE\n");
	}
	else {
		printf("FittingMode (Z): COMPENSATED IMAGE\n");
	}

	if( arImageProcMode == AR_IMAGE_PROC_IN_FULL ) {
		printf("ProcMode (X)   : FULL IMAGE\n");
	}
	else {
		printf("ProcMode (X)   : HALF IMAGE\n");
	}

	if( argDrawMode == AR_DRAW_BY_GL_DRAW_PIXELS ) {
		printf("DrawMode (C)   : GL_DRAW_PIXELS\n");
	}
	else if( argTexmapMode == AR_DRAW_TEXTURE_FULL_IMAGE ) {
		printf("DrawMode (C)   : TEXTURE MAPPING (FULL RESOLUTION)\n");
	}
	else {
		printf("DrawMode (C)   : TEXTURE MAPPING (HALF RESOLUTION)\n");
	}

	if( arTemplateMatchingMode == AR_TEMPLATE_MATCHING_COLOR ) {
		printf("TemplateMatchingMode (M)   : Color Template\n");
	}
	else {
		printf("TemplateMatchingMode (M)   : BW Template\n");
	}
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    argCleanup();
}

static void gen_image( int mode )
{
    int      i, j, k;
    ARUint8  *p;

    if( mode == 0 ) {
        for( j = 0; j < ysize; j++ ) {
            p = dataPtr + j*xsize*AR_PIX_SIZE;
            k = (j%2)? 0: 255;
            for( i = 0; i < xsize; i++ ) {
                *(p+0) = *(p+1) = *(p+2) = k;
                p += AR_PIX_SIZE;
            }
        }
    }
    else if( mode == 1 ) {
        for( i = 0; i < xsize; i++ ) {
            p = dataPtr + i*AR_PIX_SIZE;
            k = (i%2)? 0: 255;
            for( j = 0; j < ysize; j++ ) {
                *(p+0) = *(p+1) = *(p+2) = k;
                p += xsize*AR_PIX_SIZE;
            }
        }
    }
    else if( mode == 2 ) {
        p = dataPtr;
        for( j = 0; j < ysize; j++ ) {
            for( i = 0; i < xsize; i++ ) {
                if( i%64 == 0 || j%48 == 0 || i == (xsize-1) || j == (ysize-1) ) k = 255;
                else k = 0;
                *(p+0) = *(p+1) = *(p+2) = k;
                p += AR_PIX_SIZE;
            }
        }
    }

    i = arDebug;
    arDebug = 1;
    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, 100, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }
    arDebug = i;
}

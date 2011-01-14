#include "ul.h"

/* ONLY COMPILE THIS FILE FOR GLX/X11 EQUIPPED SYSTEMS */

#ifdef UL_GLX

#include "pw.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

extern "C"
{
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
}

#include <GL/gl.h>
#include <GL/glx.h>

/*
  These are supposed to be defined in /usr/include/GL/glxext.h
  But at time of writing, they were missing.  The OpenGL ARB
  people have been informed by email.
*/

#ifndef GLX_SAMPLE_BUFFERS_ARB
#define GLX_SAMPLE_BUFFERS_ARB 100000
#define GLX_SAMPLES_ARB        100001
#endif


#ifdef HAVE_MOTIF

extern "C"
{
#include <X11/Xm/MwmUtil.h>
}

#else

/* bit definitions for MwmHints.flags */
#define MWM_HINTS_FUNCTIONS	(1L << 0)
#define MWM_HINTS_DECORATIONS	(1L << 1)
#define MWM_HINTS_INPUT_MODE	(1L << 2)
#define MWM_HINTS_STATUS	(1L << 3)

/* bit definitions for MwmHints.decorations */
#define MWM_DECOR_ALL		(1L << 0)
#define MWM_DECOR_BORDER	(1L << 1)
#define MWM_DECOR_RESIZEH	(1L << 2)
#define MWM_DECOR_TITLE		(1L << 3)
#define MWM_DECOR_MENU		(1L << 4)
#define MWM_DECOR_MINIMIZE	(1L << 5)
#define MWM_DECOR_MAXIMIZE	(1L << 6)

typedef struct
{
  unsigned long flags       ;
  unsigned long functions   ;
  unsigned long decorations ;
           long inputMode   ;
  unsigned long status      ;
} PropMotifWmHints ;

#define PROP_MOTIF_WM_HINTS_ELEMENTS	5

#endif

static int initialised = 0 ;
static int modifiers   = 0 ;
static int origin [2]  = {   0,   0 } ;
static int size   [2]  = { 640, 480 } ;
static int currScreen  = 0 ;
static int currConnect = 0 ;
static int currCursor  = PW_CURSOR_LEFT ;

static pwResizeCB     *resizeCB = NULL ;
static pwExitCB       *exitCB   = NULL ;
static pwKeybdFunc    *kbCB     = NULL ;
static pwMouseFunc    *msCB     = NULL ;
static pwMousePosFunc *mpCB     = NULL ;

static Display     *currDisplay = NULL ;
static XVisualInfo *visualInfo  = NULL ;
static Window       currHandle  ;
static GLXContext   currContext ;
static Window       rootWindow  ;
static Atom         delWinAtom  ;


struct PixelFormat
{
  int num_samples ;
  int bits_per_pixel ; ;
  int z_bits ;
} ;


static PixelFormat preferred_pixel_formats [] =
{
  /* NumSamples, RGB_bits, Z_bits */

  {  0, 24, 24 },  /* Progressively nastier image formats */
  {  0, 16, 24 },
  {  0, 16, 16 },
  {  0,  3, 16 },
  {  0,  3,  1 },
  { -1, -1, -1 }   /* Magic end marker */
} ;


void defaultExitFunc ()
{
  pwCleanup () ;
  exit ( 0 ) ;
}


static void chooseVisual ( PixelFormat *pf )
{
  int attribs [ 100 ] ;
  int n = 0 ;

  attribs [n++] = GLX_RGBA ;

  switch ( pf->bits_per_pixel )
  {
    case 3 :
      attribs [n++] = GLX_RED_SIZE   ; attribs [n++] = 1 ;
      attribs [n++] = GLX_GREEN_SIZE ; attribs [n++] = 1 ;
      attribs [n++] = GLX_BLUE_SIZE  ; attribs [n++] = 1 ;
      break ;

    case 16 :
      attribs [n++] = GLX_RED_SIZE   ; attribs [n++] = 5 ;
      attribs [n++] = GLX_GREEN_SIZE ; attribs [n++] = 6 ;
      attribs [n++] = GLX_BLUE_SIZE  ; attribs [n++] = 5 ;
      break ;

    case 24 :
      attribs [n++] = GLX_RED_SIZE   ; attribs [n++] = 8 ;
      attribs [n++] = GLX_GREEN_SIZE ; attribs [n++] = 8 ;
      attribs [n++] = GLX_BLUE_SIZE  ; attribs [n++] = 8 ;
      break ;
  }

  switch ( pf->z_bits )
  {
    case  1 : attribs [n++] = GLX_DEPTH_SIZE ; attribs [n++] =  1 ; break ; 
    case 16 : attribs [n++] = GLX_DEPTH_SIZE ; attribs [n++] = 16 ; break ; 
    case 24 : attribs [n++] = GLX_DEPTH_SIZE ; attribs [n++] = 24 ; break ;
    case 32 : attribs [n++] = GLX_DEPTH_SIZE ; attribs [n++] = 32 ; break ;
  }

  if ( pf->num_samples > 0 )
  {
    attribs [n++] = GLX_SAMPLE_BUFFERS_ARB ; attribs [n++] = 1 ;
    attribs [n++] = GLX_SAMPLES_ARB        ; attribs [n++] = pf->num_samples ;
  }

  attribs [n++] = GLX_DOUBLEBUFFER ;
  attribs [n++] = None ;

  visualInfo = glXChooseVisual ( currDisplay, currScreen, attribs ) ;
}


void pwInit ( int multisample, int num_samples )
{
  pwInit ( 0, 0, -1, -1, multisample, "NoName", FALSE, num_samples ) ;
}


void pwSetCallbacks ( pwKeybdFunc *kb, pwMouseFunc *ms,
                      pwMousePosFunc *mp, pwResizeCB *rcb,
                      pwExitCB *ecb )
{
  if ( ! initialised )
  {
    fprintf ( stderr, "PW: You must not call pwSetCallbacks before pwInit.\n");
    exit ( 1 ) ;
  }

  kbCB = kb ;
  msCB = ms ;
  mpCB = mp ;
  resizeCB = rcb ;
  exitCB   = ecb ? ecb : defaultExitFunc ;
}


void pwInit ( int x, int y, int w, int h, int multisample,
              char *title, int border, int num_samples )
{
  char *displayName = getenv ( "DISPLAY" ) ;

  if ( displayName == NULL ) displayName = ":0.0" ;

  currDisplay = XOpenDisplay ( displayName ) ;

  if ( currDisplay == NULL )
  {
    fprintf ( stderr, "PW: Can't open display '%s'",
                            XDisplayName ( displayName ) ) ;
    exit ( 1 ) ;
  }


  /* OpenGL GLX extension availability? */

  if ( ! glXQueryExtension ( currDisplay, NULL, NULL ) )
  {
    fprintf ( stderr, "PW: GLX extension not available on display '%s'?!?",
                                             XDisplayName ( displayName ) ) ;
    exit ( 1 ) ;
  }

  currScreen   = DefaultScreen    ( currDisplay ) ;
  rootWindow   = RootWindow       ( currDisplay, currScreen ) ;
  currConnect  = ConnectionNumber ( currDisplay ) ;
  delWinAtom   = XInternAtom      ( currDisplay, "WM_DELETE_WINDOW", 0 ) ;

  if ( w == -1 ) w = DisplayWidth  ( currDisplay, currScreen ) ;
  if ( h == -1 ) h = DisplayHeight ( currDisplay, currScreen ) ;

  origin [ 0 ] = x ;
  origin [ 1 ] = y ;
  size   [ 0 ] = w ;
  size   [ 1 ] = h ;

  XSetWindowAttributes attribs ;
  XTextProperty   textProperty ;
  XSizeHints         sizeHints ;
  XWMHints             wmHints ;
  unsigned int            mask ;
  PixelFormat               pf ;

  for ( int i = 0 ; preferred_pixel_formats [ i ] . num_samples >= 0 ; i++ )
  {
    pf = preferred_pixel_formats [ i ] ;
    pf . num_samples = num_samples ;

    chooseVisual ( &pf ) ;

    if ( visualInfo != NULL )
      break ;
  }

  if ( visualInfo == NULL )
  {
    num_samples = 0 ;

    for ( int i = 0 ; preferred_pixel_formats [ i ] . num_samples >= 0 ; i++ )
    {
      pf = preferred_pixel_formats [ i ] ;
      pf . num_samples = num_samples ;

      chooseVisual ( &pf ) ;

      if ( visualInfo != NULL )
        break ;
    }

    if ( visualInfo == NULL )
    {
      fprintf ( stderr, "PW: Unable to open a suitable graphics window,\n" ) ;
      exit ( 1 ) ;
    }
  }

  attribs.event_mask = StructureNotifyMask | ExposureMask         |
                       ButtonPressMask     | ButtonReleaseMask    |
                       KeyPressMask        | KeyReleaseMask       |
		       EnterWindowMask     | LeaveWindowMask      |
                       PointerMotionMask   | ButtonMotionMask     |
                       VisibilityChangeMask ;

  attribs.background_pixmap = None ;
  attribs.background_pixel  = 0    ;
  attribs.border_pixel      = 0    ;

  attribs.colormap = XCreateColormap ( currDisplay, rootWindow,
                                       visualInfo->visual, AllocNone ) ;

  mask = CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

  currHandle = XCreateWindow ( currDisplay, rootWindow,
                           x, y, w, h, 0, visualInfo->depth,
                           InputOutput, visualInfo->visual,
                           mask, &attribs ) ;

  currContext = glXCreateContext ( currDisplay, visualInfo, NULL, 1 ) ;

  glXMakeCurrent ( currDisplay, currHandle, currContext ) ;

  if ( ! glXIsDirect ( currDisplay, glXGetCurrentContext() ) )
  {
    fprintf ( stderr,"PW: This is an *INDIRECT* rendering context.") ;
    fprintf ( stderr,"PW: That may be bad for performance." ) ;
  }

  sizeHints.flags = 0 ;

  if ( x >= 0 && y >= 0 )
    sizeHints.flags |= USPosition ;

  sizeHints.flags |= USSize ;

  sizeHints.x      = x ; sizeHints.y      = y ;
  sizeHints.width  = w ; sizeHints.height = h ;

  wmHints.flags = StateHint;
  wmHints.initial_state = NormalState ;

  PropMotifWmHints hints  ;
  Atom prop_t ;
  Atom prop   ;

  hints . flags = MWM_HINTS_DECORATIONS ;
  hints . decorations = border ? MWM_DECOR_ALL : 0 ;

  prop_t = prop = XInternAtom ( currDisplay, "_MOTIF_WM_HINTS", True ) ;

  if ( prop != 0 )
    XChangeProperty ( currDisplay, currHandle, prop, prop_t, 32,
                      PropModeReplace, (unsigned char *) &hints,
                      PROP_MOTIF_WM_HINTS_ELEMENTS) ;

  XStringListToTextProperty ( (char **) &title, 1, &textProperty ) ;

  XSetWMProperties ( currDisplay, currHandle,
	                  &textProperty, &textProperty, 0, 0,
                          &sizeHints, &wmHints, NULL ) ;
  XSetWMProtocols  ( currDisplay, currHandle, &delWinAtom  , 1 );
  XMapWindow       ( currDisplay, currHandle ) ;
  glXMakeCurrent   ( currDisplay, currHandle, currContext ) ;

  pwSetCursor ( PW_CURSOR_LEFT ) ;

#ifdef GL_MULTISAMPLE_FILTER_HINT_NV
  glHint ( GL_MULTISAMPLE_FILTER_HINT_NV, multisample ) ;
#endif

  kbCB     = NULL ;
  msCB     = NULL ;
  mpCB     = NULL ;
  resizeCB = NULL ;
  exitCB   = defaultExitFunc ;

  initialised = 1 ;

  glClear ( GL_COLOR_BUFFER_BIT ) ;
  pwSwapBuffers () ;
  glClear ( GL_COLOR_BUFFER_BIT ) ;
  pwSwapBuffers () ;
}


void pwGetSize ( int *w, int *h )
{
  if ( w ) *w = size[0] ;
  if ( h ) *h = size[1] ;
}


void pwSetCursor ( int c )
{
  switch ( c )
  {
    case PW_CURSOR_RIGHT  : currCursor = XC_right_ptr      ; break ;
    case PW_CURSOR_LEFT   : currCursor = XC_left_ptr       ; break ;
    case PW_CURSOR_QUERY  : currCursor = XC_question_arrow ; break ;
    case PW_CURSOR_AIM    : currCursor = XC_target         ; break ;
    case PW_CURSOR_CIRCLE : currCursor = XC_circle         ; break ;
    case PW_CURSOR_WAIT   : currCursor = XC_watch          ; break ;
    case PW_CURSOR_CROSS  : currCursor = XC_crosshair      ; break ;

    case PW_CURSOR_NONE  :
    default:
      {
        Pixmap pix ;
        char   blank_cursor [ 16*16 ] = { 0 } ;
        XColor bcol = { 0 } ;
  
        pix = XCreateBitmapFromData ( currDisplay,
                                      rootWindow,
                                      blank_cursor, 16, 16 ) ;
        XDefineCursor ( currDisplay, currHandle,
                        XCreatePixmapCursor ( currDisplay,
                            pix, pix, &bcol, &bcol, 0, 0 ) ) ;
        XFreePixmap   ( currDisplay, pix ) ;

        currCursor = 0 ;
      }
      return ;
  }
 
  XDefineCursor( currDisplay, currHandle,
                 XCreateFontCursor ( currDisplay, currCursor ) ) ;
}


void pwSetSize ( int w, int h )
{
  XResizeWindow ( currDisplay, currHandle, w, h ) ;
  XFlush        ( currDisplay ) ;
}


void pwSetOrigin ( int x, int y )
{
  XMoveWindow ( currDisplay, currHandle, x, y ) ;
  XFlush      ( currDisplay ) ;
}


void pwSetSizeOrigin ( int x, int y, int w, int h )
{
  XMoveWindow   ( currDisplay, currHandle, x, y ) ;
  XResizeWindow ( currDisplay, currHandle, w, h ) ;
  XFlush        ( currDisplay ) ;
}


static void refreshModifiers( XEvent *event )
{
  modifiers = 0 ;

  if( event->xkey.state & ( ShiftMask | LockMask ) ) modifiers |= PW_SHIFT ;
  if( event->xkey.state & ControlMask              ) modifiers |= PW_CTRL  ;
  if( event->xkey.state & Mod1Mask                 ) modifiers |= PW_ALT   ;
}


int pwGetModifiers ()
{
  return modifiers ;
}


static void getEvents ()
{
  XEvent event ;

  while ( XPending ( currDisplay ) )
  {
    int updown = PW_UP ;

    XNextEvent ( currDisplay, &event ) ;

    refreshModifiers ( &event ) ;

    switch ( event.type )
    {
      case ClientMessage   : (*exitCB)() ; break ;
      case DestroyNotify   : (*exitCB)() ; break ;

      case ConfigureNotify :
        if ( currHandle == event.xconfigure.window &&
              ( size[0] != event.xconfigure.width ||
                size[1] != event.xconfigure.height ) )
        {
          size[0] = event.xconfigure.width ;
          size[1] = event.xconfigure.height ;

          glXMakeCurrent ( currDisplay, currHandle, currContext ) ;
          glXWaitX    () ;

          if ( resizeCB != NULL )
            (*resizeCB) ( size[0], size[1] ) ;
        }
	break;

      case MappingNotify:
	XRefreshKeyboardMapping ( (XMappingEvent *) &event ) ;
	break;

      case EnterNotify     :
      case LeaveNotify     :
      case VisibilityNotify:
      case Expose          : break ;

      case MotionNotify    :
        if ( mpCB )
          (*mpCB) ( event.xmotion.x, event.xmotion.y ) ;
        break ;

      case ButtonPress     :
        updown = PW_DOWN ;
        /* FALLTHROUGH */
      case ButtonRelease   :
        {
          int button = -1 ;

          switch ( event.xbutton.button )
          {
            case 1 : button = PW_LEFT_BUTTON   ; break ;
            case 2 : button = PW_MIDDLE_BUTTON ; break ;
            case 3 : button = PW_RIGHT_BUTTON  ; break ;
          }

          if ( button != -1 && msCB )
            (*msCB) ( button, updown, event.xbutton.x, event.xbutton.y ) ;
        }
        break ;

      case KeyPress        :
        updown = PW_DOWN ;
        /* FALLTHROUGH */
      case KeyRelease      :
        {
          XComposeStatus composeStatus ;
          char           asciiCode [ 32 ] ;
          KeySym         keySym ;

          int len = XLookupString( &event.xkey, asciiCode, sizeof(asciiCode),
                                   &keySym, &composeStatus ) ;
          int result = -1 ;

          if( len > 0 )
            result = asciiCode[ 0 ] ;
          else
          {
            switch( keySym )
            {
              case XK_F1:     result = PW_KEY_F1;     break;
              case XK_F2:     result = PW_KEY_F2;     break;
              case XK_F3:     result = PW_KEY_F3;     break;
              case XK_F4:     result = PW_KEY_F4;     break;
              case XK_F5:     result = PW_KEY_F5;     break;
              case XK_F6:     result = PW_KEY_F6;     break;
              case XK_F7:     result = PW_KEY_F7;     break;
              case XK_F8:     result = PW_KEY_F8;     break;
              case XK_F9:     result = PW_KEY_F9;     break;
              case XK_F10:    result = PW_KEY_F10;    break;
              case XK_F11:    result = PW_KEY_F11;    break;
              case XK_F12:    result = PW_KEY_F12;    break;

              case XK_Left:   result = PW_KEY_LEFT;   break;
              case XK_Right:  result = PW_KEY_RIGHT;  break;
              case XK_Up:     result = PW_KEY_UP;     break;
              case XK_Down:   result = PW_KEY_DOWN;   break;

              case XK_KP_Prior:
              case XK_Prior:  result = PW_KEY_PAGE_UP; break;
              case XK_KP_Next:
              case XK_Next:   result = PW_KEY_PAGE_DOWN; break;
              case XK_KP_Home:
              case XK_Home:   result = PW_KEY_HOME;   break;
              case XK_KP_End:
              case XK_End:    result = PW_KEY_END;    break;
              case XK_KP_Insert:
              case XK_Insert: result = PW_KEY_INSERT; break;
            }
          }

          if ( result != -1 && kbCB != NULL )
            (*kbCB) ( result, updown, event.xkey.x, event.xkey.y ) ;
        }
        break ;
    }
  }

  glXMakeCurrent ( currDisplay, currHandle, currContext ) ;
}


void pwSwapBuffers ()
{
  glFlush () ;
  glXSwapBuffers ( currDisplay, currHandle ) ;
  getEvents () ;
}


#ifdef NEED_GAMMA
void pwSetGamma ( float g )
{
  XF86VidModeGamma gamma ;
  gamma.red = gamma.green = gamma.blue = g ;
  XF86VidModeSetGamma ( currDisplay, currScreen, & gamma ) ;
}
#endif


void pwCleanup ()
{
  glXDestroyContext ( currDisplay, currContext ) ;
  XDestroyWindow    ( currDisplay, currHandle  ) ;
  XFlush            ( currDisplay ) ;
}


#endif


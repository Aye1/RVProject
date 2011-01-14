/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 1998,2002  Steve Baker

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Library General Public
     License as published by the Free Software Foundation; either
     version 2 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Library General Public License for more details.

     You should have received a copy of the GNU Library General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

     For further information visit http://plib.sourceforge.net

     $Id: pu.cxx,v 1.67 2004/02/16 22:09:34 stromberg Exp $
*/

#include "puLocal.h"

#if defined(UL_AGL)
#  include <agl.h>
#elif defined(UL_CGL)
#  include <OpenGL/CGLCurrent.h>
#elif defined(UL_GLX)
#  include <GL/glx.h>
#endif


int puRefresh = TRUE ;


static puGetWindowCallback     cbGetWindow;
static puSetWindowCallback     cbSetWindow;
static puGetWindowSizeCallback cbGetWindowSize;
static puSetWindowSizeCallback cbSetWindowSize;


void puSetWindowFuncs ( puGetWindowCallback   getWindow,
			puSetWindowCallback   setWindow,
			puGetWindowSizeCallback getWindowSize,
			puSetWindowSizeCallback setWindowSize )
{
    cbGetWindow     = getWindow;
    cbSetWindow     = setWindow;
    cbGetWindowSize = getWindowSize;
    cbSetWindowSize = setWindowSize;
}


int puGetWindow ()
{
    if ( !cbGetWindow )
	ulSetError ( UL_FATAL, "puGetWindow: Callbacks not initialized." ) ;
    return ( *cbGetWindow ) () ;
}

void puSetWindow ( int window )
{
    if ( !cbSetWindow )
	ulSetError ( UL_WARNING, "puSetWindow: Cannot set window (null callback)." ) ;
    else
	( *cbSetWindow ) ( window ) ;
}

void puGetWindowSize ( int *width, int *height )
{
    if ( !cbGetWindowSize )
	ulSetError ( UL_FATAL, "puGetWindowSize: Callbacks not initialized." ) ;
    ( *cbGetWindowSize ) ( width, height ) ;
}

void puSetWindowSize ( int width, int height )
{
    if ( !cbSetWindowSize )
	ulSetError ( UL_WARNING, "puSetWindowSize: Cannot change size (null callback)." ) ;
    else
	( *cbSetWindowSize ) ( width, height ) ;
}

int puGetWindowWidth ()
{
    int w, h ;
    puGetWindowSize ( &w, &h ) ;
    return w ;
}

int puGetWindowHeight ()
{
    int w, h ;
    puGetWindowSize ( &w, &h ) ;
    return h ;
}



static bool glIsValidContext ( void )
{
#if defined(UL_WGL)
  return ( wglGetCurrentContext () != NULL ) ;
#elif defined(UL_AGL)
  return ( aglGetCurrentContext () != NULL ) ;
#elif defined(UL_CGL)
  return ( CGLGetCurrentContext () != NULL ) ;
#elif defined(UL_GLX)
  return ( glXGetCurrentContext () != NULL ) ;
#else
  return true ;
#endif
}


static int _puCursor_enable = FALSE ;
static int _puCursor_x      = 0 ;
static int _puCursor_y      = 0 ;
static float _puCursor_bgcolour [4] = { 1.0f, 1.0f, 1.0f, 1.0f } ; 
static float _puCursor_fgcolour [4] = { 0.0f, 0.0f, 0.0f, 1.0f } ;  

void   puHideCursor ( void ) { _puCursor_enable = FALSE ; }
void   puShowCursor ( void ) { _puCursor_enable = TRUE  ; }
int    puCursorIsHidden ( void ) { return ! _puCursor_enable ; }

void puCursor ( int x, int y )
{
  _puCursor_x = x ;
  _puCursor_y = y ;
}


static void puDrawCursor ( int x, int y )
{
  glColor4fv ( _puCursor_bgcolour ) ;  

  glBegin    ( GL_TRIANGLES ) ;
  glVertex2i ( x, y ) ;
  glVertex2i ( x + 13, y -  4 ) ;
  glVertex2i ( x +  4, y - 13 ) ;

  glVertex2i ( x +  8, y -  3 ) ;
  glVertex2i ( x + 17, y - 12 ) ;
  glVertex2i ( x + 12, y - 17 ) ;

  glVertex2i ( x + 12, y - 17 ) ;
  glVertex2i ( x +  3, y -  8 ) ;
  glVertex2i ( x +  8, y -  3 ) ;
  glEnd      () ;

  glColor4fv ( _puCursor_fgcolour ) ;  

  glBegin    ( GL_TRIANGLES ) ;
  glVertex2i ( x+1, y-1 ) ;
  glVertex2i ( x + 11, y -  4 ) ;
  glVertex2i ( x +  4, y - 11 ) ;

  glVertex2i ( x +  8, y -  5 ) ;
  glVertex2i ( x + 15, y - 12 ) ;
  glVertex2i ( x + 12, y - 15 ) ;

  glVertex2i ( x + 12, y - 15 ) ;
  glVertex2i ( x +  5, y -  8 ) ;
  glVertex2i ( x +  8, y -  5 ) ;
  glEnd      () ;
}


// Pointer to linked list of objects to delete
// as a result of keyboarding or mouse clicking

static puObject *objects_to_delete = NULL;


void puDeleteObject ( puObject *ob )
{
  if ( ob == NULL )
    return ;

  puGroup *parent = ob->getParent () ;

  /* Add object to linked list to be deleted */
  if ( objects_to_delete == NULL )
    objects_to_delete = ob ;
  else
  {
    /* Ensure that objects are deleted in the order of puDeleteObject calls */

    puObject *last ;

    for ( last = objects_to_delete ;
          last -> getNextObject() != NULL ;
          last = last -> getNextObject() )
      /* Find last object. */ ;

    last -> setNextObject ( ob ) ;
  }

  /* Remove from parent interface */
 
  if ( parent != ob && parent != NULL )
     parent -> remove ( ob ) ;  /* Sets object's next and previous pointers to null as well */
}


void puCleanUpJunk ( void )
{
  puObject * local_objects_to_delete = objects_to_delete ;
  objects_to_delete = NULL ;
  /* Step through the linked list of objects to delete, removing them. */
  while ( local_objects_to_delete != NULL )
  {
    puObject *next_ob = local_objects_to_delete -> getNextObject() ;
    delete local_objects_to_delete ;
    local_objects_to_delete = next_ob ;

    /* If we've reached the end of the list, start over (in case we've deleted a group and
     * it has put new widgets on the delete list)
     */
    if ( local_objects_to_delete == NULL )
    {
      local_objects_to_delete = objects_to_delete ;
      objects_to_delete = NULL ;
    }
  }
}


static puObject *active_widget ;   /* Widget which is currently receiving user input */
static char *input_paste_buffer ;  /* Cut/Copy/Paste buffer for input widgets */

static int firsttime = TRUE ;

void puRealInit ( void )
{
  if ( firsttime )
  {
    if ( ! glIsValidContext () )
    {
      ulSetError ( UL_FATAL,
        "puInit called without a valid OpenGL context.");
    }

    new puInterface ( 0, 0 ) ;

    active_widget = NULL ;
    input_paste_buffer = NULL ;

    firsttime = FALSE ;
  }
}

void puExit ( void )
{
  if ( firsttime )
    ulSetError ( UL_FATAL, "puExit called without a previous call to puInit." ) ;

  delete puGetBaseLiveInterface () ;
  firsttime = TRUE ;
}

static void puSetOpenGLState ( void )
{
  int w, h ;

  puGetWindowSize ( &w, &h ) ;

  glPushAttrib ( GL_ENABLE_BIT | GL_VIEWPORT_BIT  | GL_TRANSFORM_BIT | GL_LIGHTING_BIT ) ;

  glDisable      ( GL_LIGHTING   ) ;
  glDisable      ( GL_FOG        ) ;
  glDisable      ( GL_TEXTURE_2D ) ;
  glDisable      ( GL_DEPTH_TEST ) ;
  glDisable      ( GL_CULL_FACE  ) ;

  glViewport ( 0, 0, w, h ) ;

  glMatrixMode   ( GL_PROJECTION ) ;
  glPushMatrix   () ;
  glLoadIdentity () ;
  glOrtho        ( 0, w, 0, h, -1, 1 ) ;
  glMatrixMode   ( GL_MODELVIEW ) ;
  glPushMatrix   () ;
  glLoadIdentity () ;
}

static void puRestoreOpenGLState ( void )
{
  glMatrixMode   ( GL_PROJECTION ) ;
  glPopMatrix    () ;
  glMatrixMode   ( GL_MODELVIEW ) ;
  glPopMatrix    () ;
  glPopAttrib    () ;
}


void  puDisplay ( void )
{
  puCleanUpJunk () ;

  puSetOpenGLState () ;
  puGetUltimateLiveInterface () -> draw ( 0, 0 ) ;

  int h = puGetWindowHeight () ;

  if ( _puCursor_enable )
    puDrawCursor ( _puCursor_x,
                   h - _puCursor_y ) ;

  puRestoreOpenGLState () ;

  puRefresh = FALSE ;
}


void  puDisplay ( int window_number )  /* Deprecated */
{
  if ( window_number == puGetWindow () )
    puDisplay () ;
}


int puKeyboard ( int key, int updown, int, int )
{
  return puKeyboard ( key, updown ) ;
}


int puKeyboard ( int key, int updown )
{
  int return_value = puGetBaseLiveInterface () -> checkKey ( key, updown ) ;

  puCleanUpJunk () ;

  return return_value ;
}


static int last_buttons = 0 ;
static int pu_mouse_x = 0 ;
static int pu_mouse_y = 0 ;
static int pu_mouse_offset_x = 0 ;
static int pu_mouse_offset_y = 0 ;

int puGetPressedButton ()
{
  return last_buttons ;
}

int puMouse ( int button, int updown, int x, int y )
{
  puCursor ( x, y ) ;

  int h = puGetWindowHeight () ;

  if ( updown == PU_DOWN )
    last_buttons |=  ( 1 << button ) ;
  else
    last_buttons &= ~( 1 << button ) ;

  pu_mouse_x = x ;
  pu_mouse_y = h - y ;
  int return_value =  puGetBaseLiveInterface () -> checkHit ( button,
    updown, pu_mouse_x, pu_mouse_y ) ;

  puCleanUpJunk () ;

  puObject *active = puActiveWidget () ;

  if ( ( last_buttons == 0 ) && ( active != NULL ) )
  {
    int x_offset, y_offset ;
    active -> getAbsolutePosition ( &x_offset, &y_offset ) ;

    x_offset -= active -> getABox () -> min [0] ;
    y_offset -= active -> getABox () -> min [1] ;

    if ( ! active -> isHit ( pu_mouse_x - x_offset, pu_mouse_y - y_offset ) )
    {
      active -> invokeDownCallback () ;
      puDeactivateWidget () ;
    }
  }

  return return_value ;
}


int puMouse ( int x, int y )
{
  puCursor ( x, y ) ;

  int button =
    (last_buttons & (1<<PU_LEFT_BUTTON  )) ?  PU_LEFT_BUTTON   :
    (last_buttons & (1<<PU_MIDDLE_BUTTON)) ?  PU_MIDDLE_BUTTON :
    (last_buttons & (1<<PU_RIGHT_BUTTON )) ?  PU_RIGHT_BUTTON  : PU_NOBUTTON ;

  int h = puGetWindowHeight () ;

  pu_mouse_x = x ;
  pu_mouse_y = h - y ;

  /*
    When you drag over an ACTIVE widget, you don't
    affect any other widgets until you release the
    mouse button.
  */
  if ( puActiveWidget () )
  {
    puActiveWidget()->doHit(button, PU_DRAG, pu_mouse_x - pu_mouse_offset_x,
                                             pu_mouse_y - pu_mouse_offset_y) ;
    return TRUE ;
  }

  int return_value = puGetBaseLiveInterface () -> checkHit ( button,
    PU_DRAG, pu_mouse_x, pu_mouse_y ) ;

  puCleanUpJunk () ;

  return return_value ;
}

void puMoveToLast (puObject *ob)
{
  puGroup *parent = ob -> getParent () ;

  /* If no parent interface, return. */

  if ( ! parent ) return;

  /* Remove "ob" from present place in the "dlist" list */

  parent -> remove (ob) ;

  /* Place at the end of the list */

  parent -> add (ob) ;

  /*
    Now repeat the process for the parent interface so that the interface will
    be drawn last of all interfaces.
  */

  puMoveToLast ( parent );
}

void puDeactivateWidget ( void )  {  active_widget = NULL ; }
void puSetActiveWidget ( puObject *w, int x, int y )
{
  active_widget = w ;
  pu_mouse_offset_x = pu_mouse_x - x ;
  pu_mouse_offset_y = pu_mouse_y - y ;
}

puObject *puActiveWidget ( void ) {   return active_widget ; }

void puSetPasteBuffer ( const char *ch )
{
  delete [] input_paste_buffer ;
  input_paste_buffer = ulStrDup ( ch ) ;
}

char *puGetPasteBuffer ( void )  {  return input_paste_buffer ;  }

int  puNeedRefresh ( void )  {  return puRefresh ;  }
void puPostRefresh ( void )  {  puRefresh = TRUE ;  }


void puSetResizeMode ( int mode )
{
    static int last = 0;
    if ( last == 0 && mode != 0 )
	ulSetError ( UL_WARNING, "puSetResizeMode is deprecated!" ) ;
    last = mode ;
}

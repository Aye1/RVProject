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

     $Id: jsLinux.cxx,v 1.3 2003/04/10 12:36:27 puggles Exp $
*/

#include "js.h"

#if defined(UL_LINUX) && defined(JS_NEW)

#include <sys/param.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/* check the joystick driver version */

void jsJoystick::open ()
{
  name [0] = '\0' ;

  for ( int i = 0 ; i < _JS_MAX_AXES ; i++ )
    tmp_axes [ i ] = 0.0f ;

  tmp_buttons = 0 ;

  fd = ::open ( fname, O_RDONLY ) ;

  error = ( fd < 0 ) ;

  if ( error )
    return ;

  /*
    Set the correct number of axes for the linux driver
  */

  /* Melchior Franz's fixes for big-endian Linuxes since writing 
   *  to the upper byte of an uninitialized word doesn't work. 
   *  9 April 2003 
   */
  unsigned char u ;
  ioctl ( fd, JSIOCGAXES   , &u ) ; 
  num_axes = u ;
  ioctl ( fd, JSIOCGBUTTONS, &u ) ;
  num_buttons = u ;
  ioctl ( fd, JSIOCGNAME ( sizeof(name) ), name ) ;
  fcntl ( fd, F_SETFL      , O_NONBLOCK   ) ;

  if ( num_axes > _JS_MAX_AXES )
    num_axes = _JS_MAX_AXES ;

  for ( int i = 0 ; i < _JS_MAX_AXES ; i++ )
  {
    max       [ i ] = 32767.0f ;
    center    [ i ] = 0.0f ;
    min       [ i ] = -32767.0f ;
    dead_band [ i ] = 0.0f ;
    saturate  [ i ] = 1.0f ;
  }
}



void jsJoystick::close ()
{
  if ( ! error )
    ::close ( fd ) ;
}


jsJoystick::jsJoystick ( int ident )
{
  id = ident ;

  sprintf ( fname, "/dev/input/js%d", ident ) ;

  if ( access ( fname, F_OK ) != 0 )
    sprintf ( fname, "/dev/js%d", ident ) ;

  open () ;
}


void jsJoystick::rawRead ( int *buttons, float *axes )
{
  if ( error )
  {
    if ( buttons )
      *buttons = 0 ;

    if ( axes )
      for ( int i = 0 ; i < num_axes ; i++ )
        axes[i] = 1500.0f ;

    return ;
  }

  while (1)
  {
    int status = ::read ( fd, &js, sizeof(js_event) ) ;

    if ( status != sizeof(js_event) )
    {
      /* use the old values */

      if ( buttons != NULL ) *buttons = tmp_buttons ;
      if ( axes    != NULL )
        memcpy ( axes, tmp_axes, sizeof(float) * num_axes ) ;

      if ( errno == EAGAIN )
        return ;

      perror( fname ) ;
      setError () ;
      return ;
    }

    switch ( js.type & ~JS_EVENT_INIT )
    {
      case JS_EVENT_BUTTON :
        if ( js.value == 0 ) /* clear the flag */
          tmp_buttons &= ~(1 << js.number) ;
        else
          tmp_buttons |=  (1 << js.number) ;
        break ;

      case JS_EVENT_AXIS:
        if ( js.number < num_axes )
        {
          tmp_axes [ js.number ] = (float) js.value ;

          if ( axes )
            memcpy ( axes, tmp_axes, sizeof(float) * num_axes ) ;
        }
        break ;

      default:
        ulSetError ( UL_WARNING, "PLIB_JS: Unrecognised /dev/js return!?!" ) ;

        /* use the old values */

        if ( buttons != NULL ) *buttons = tmp_buttons ;
        if ( axes    != NULL )
          memcpy ( axes, tmp_axes, sizeof(float) * num_axes ) ;

        return ;
    }

    if ( buttons != NULL )
      *buttons = tmp_buttons ;
  }
}

#endif


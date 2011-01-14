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

     $Id: js.h,v 1.30 2003/11/11 22:54:48 stromberg Exp $
*/

#ifndef __INCLUDED_JS_H__
#define __INCLUDED_JS_H__ 1

#include "ul.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // -dw- for memcpy

#define _JS_MAX_AXES 16
#define _JS_MAX_BUTTONS 32

#ifdef UL_MACINTOSH
#  include <InputSprocket.h>
#endif

#ifdef UL_MAC_OSX
#  include <mach/mach.h>
#  include <IOKit/IOkitLib.h>
#  include <IOKit/hid/IOHIDLib.h>
#endif

#ifdef UL_LINUX 
#    include <sys/ioctl.h>
#    include <linux/joystick.h>

/* check the joystick driver version */

#if defined(JS_VERSION) && JS_VERSION >= 0x010000
#  define JS_NEW
#endif
#endif

#define JS_TRUE  1
#define JS_FALSE 0

#ifndef JS_RETURN

 /*
   We'll put these values in and that should
   allow the code to at least compile when there is
   no support. The JS open routine should error out
   and shut off all the code downstream anyway
 */

  struct JS_DATA_TYPE
  {
    int buttons ;
    int x ;
    int y ;
  } ;

#  define JS_RETURN (sizeof(struct JS_DATA_TYPE))
#endif

class jsJoystick
{
  int          id ;

#if defined(UL_MACINTOSH)

#define  isp_num_axis   9
#define  isp_num_needs  41
  ISpElementReference isp_elem  [ isp_num_needs ] ;
  ISpNeed             isp_needs [ isp_num_needs ] ;

#elif defined(UL_BSD)
  struct os_specific_s *os ;
#elif defined(UL_MAC_OSX)

  IOHIDDeviceInterface ** hidDev;
  static const int kNumDevices;
  static int numDevices;
  static io_object_t ioDevices[kNumDevices];

  static void findDevices(mach_port_t);
  static CFDictionaryRef getCFProperties(io_object_t);

  void enumerateElements(CFTypeRef element);
  /// callback for CFArrayApply
  static void elementEnumerator( const void *element, void* vjs);
  void parseElement(CFDictionaryRef element);

  void addAxisElement(CFDictionaryRef axis);
  void addButtonElement(CFDictionaryRef button);
  void addHatElement(CFDictionaryRef hat);

  IOHIDElementCookie buttonCookies[41];
  IOHIDElementCookie axisCookies[_JS_MAX_AXES];
  long minReport[_JS_MAX_AXES],
       maxReport[_JS_MAX_AXES];

#elif defined(UL_WIN32)
  JOYCAPS      jsCaps ;
  JOYINFOEX    js     ;
  UINT         js_id  ;
  bool getOEMProductName ( char *buf, int buf_sz ) ;
#else

#ifdef JS_NEW
  js_event     js          ;
  int          tmp_buttons ;
  float        tmp_axes [ _JS_MAX_AXES ] ;
# else
  JS_DATA_TYPE js ;
# endif

  char         fname [ 128 ] ;
  int          fd ;
#endif

  int          error        ;
  char         name [ 128 ] ;
  int          num_axes     ;
  int          num_buttons  ;

  float dead_band [ _JS_MAX_AXES ] ;
  float saturate  [ _JS_MAX_AXES ] ;
  float center    [ _JS_MAX_AXES ] ;
  float max       [ _JS_MAX_AXES ] ;
  float min       [ _JS_MAX_AXES ] ;


  void open () ;
  void close () ;

  float fudge_axis ( float value, int axis ) const ;

public:

  jsJoystick ( int ident = 0 ) ;
  ~jsJoystick () { close () ; }

  const char* getName () const { return name ;     }
  int   getNumAxes    () const { return num_axes ; }
  int   notWorking    () const { return error ;    }
  void  setError      () { error = JS_TRUE ; }

  float getDeadBand ( int axis ) const       { return dead_band [ axis ] ; }
  void  setDeadBand ( int axis, float db )   { dead_band [ axis ] = db   ; }

  float getSaturation ( int axis ) const     { return saturate [ axis ]  ; }
  void  setSaturation ( int axis, float st ) { saturate [ axis ] = st    ; }

  void setMinRange ( float *axes ) { memcpy ( min   , axes, num_axes * sizeof(float) ) ; }
  void setMaxRange ( float *axes ) { memcpy ( max   , axes, num_axes * sizeof(float) ) ; }
  void setCenter   ( float *axes ) { memcpy ( center, axes, num_axes * sizeof(float) ) ; }

  void getMinRange ( float *axes ) const { memcpy ( axes, min   , num_axes * sizeof(float) ) ; }
  void getMaxRange ( float *axes ) const { memcpy ( axes, max   , num_axes * sizeof(float) ) ; }
  void getCenter   ( float *axes ) const { memcpy ( axes, center, num_axes * sizeof(float) ) ; }

  void read    ( int *buttons, float *axes ) ;
  void rawRead ( int *buttons, float *axes ) ;
} ;

extern void jsInit () ;

#endif


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

     $Id: jsMacOS.cxx,v 1.2 2002/11/30 00:41:49 sjbaker Exp $
*/

#include "js.h"

#ifdef UL_MACINTOSH

#define _JS_MAX_AXES 9


void jsJoystick::open ()
{
  name [0] = '\0' ;

  /*
    FIXME: get joystick name in Mac
  */

  OSStatus err ;

  err = ISpStartup () ;

  if ( err == noErr )
  {
#define ISP_CHECK_ERR(x) if ( x != noErr ) { setError () ; return ; }

    setError () ;

    // initialize the needs structure
    ISpNeed temp_isp_needs[isp_num_needs] =
    {
      { "\pX-Axis",    128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pY-Axis",    128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pZ-Axis",    128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pR-Axis",    128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pAxis   4",  128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pAxis   5",  128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pAxis   6",  128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pAxis   7",  128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },
      { "\pAxis   8",  128, 0, 0, kISpElementKind_Axis,   kISpElementLabel_None, 0, 0, 0, 0 },

      { "\pButton 0",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 1",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 2",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 3",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 4",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 5",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 6",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 7",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 8",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 9",  128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 10", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 11", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 12", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 13", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 14", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 15", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 16", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 17", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 18", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 19", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 20", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 21", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 22", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 23", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 24", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 25", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 26", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 27", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 28", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 29", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 30", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
      { "\pButton 31", 128, 0, 0, kISpElementKind_Button, kISpElementLabel_Btn_Select, 0, 0, 0, 0 },
    } ;

    memcpy ( isp_needs, temp_isp_needs, sizeof(temp_isp_needs) ) ;


    // next two calls allow keyboard and mouse to emulate other input
    // devices (gamepads, joysticks, etc)

    /*
      err = ISpDevices_ActivateClass ( kISpDeviceClass_Keyboard ) ;
      ISP_CHECK_ERR(err)


      err = ISpDevices_ActivateClass ( kISpDeviceClass_Mouse ) ;
      ISP_CHECK_ERR(err)
    */

    err = ISpElement_NewVirtualFromNeeds ( isp_num_needs, isp_needs, isp_elem, 0 ) ;
    ISP_CHECK_ERR(err)

    err = ISpInit ( isp_num_needs, isp_needs, isp_elem, 'PLIB', nil, 0, 128, 0 ) ;
    ISP_CHECK_ERR(err)

    num_buttons = isp_num_needs - isp_num_axis ;
    num_axes    = isp_num_axis ;

    for ( int i = 0 ; i < num_axes ; i++ )
    {
      dead_band [ i ] = 0 ;
      saturate  [ i ] = 1 ;
      center    [ i ] = kISpAxisMiddle ;
      max       [ i ] = kISpAxisMaximum ;
      min       [ i ] = kISpAxisMinimum ;
    }

    error = false ;
  }
  else
  {
    setError () ;
    num_buttons = num_axes = 0 ;
  }
}


void jsJoystick::close ()
{
  ISpSuspend  () ;
  ISpStop     () ;
  ISpShutdown () ;
}


jsJoystick::jsJoystick ( int ident )
{
  id = ident ;
  sprintf ( fname, "/dev/js%d", ident ) ; /* FIXME */
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

  int i ;
  int err ;
  UInt32 state ;

  if ( buttons != NULL )
  {
    *buttons = 0;

    for ( i = 0 ; i < num_buttons ; i++ )
    {
      err = ISpElement_GetSimpleState ( isp_elem [ i + isp_num_axis ], &state) ;
      ISP_CHECK_ERR(err)

       *buttons |= state << i ;
    }
  }

  if ( axes != NULL )
  {
     for ( i = 0 ; i < num_axes ; i++ )
     {
       err = ISpElement_GetSimpleState ( isp_elem [ i ], &state ) ;
       ISP_CHECK_ERR(err)

       axes [i] = (float) state ;
     }
  }
}

#endif

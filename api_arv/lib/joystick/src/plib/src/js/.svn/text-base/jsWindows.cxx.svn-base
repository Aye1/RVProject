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

     $Id: jsWindows.cxx,v 1.1 2002/11/29 19:39:48 sjbaker Exp $
*/

#include "js.h"

#ifdef UL_WIN32

#define _JS_MAX_AXES 8  /* X,Y,Z,R,U,V,POV_X,POV_Y */


// Inspired by
// http://msdn.microsoft.com/archive/en-us/dnargame/html/msdn_sidewind3d.asp

bool jsJoystick::getOEMProductName ( char *buf, int buf_sz )
{
  if ( error )
    return false ;

  union
  {
    char key   [ 256 ] ;
    char value [ 256 ] ;
  } ;
  char OEMKey [ 256 ] ;

  HKEY  hKey ;
  DWORD dwcb ;
  LONG  lr ;

  // Open .. MediaResources\CurrentJoystickSettings
  sprintf ( key, "%s\\%s\\%s",
            REGSTR_PATH_JOYCONFIG, jsCaps.szRegKey,
            REGSTR_KEY_JOYCURR ) ;

  lr = RegOpenKeyEx ( HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &hKey) ;

  if ( lr != ERROR_SUCCESS ) return false ;

  // Get OEM Key name
  dwcb = sizeof(OEMKey) ;

  // JOYSTICKID1-16 is zero-based; registry entries for VJOYD are 1-based.
  sprintf ( value, "Joystick%d%s", js_id + 1, REGSTR_VAL_JOYOEMNAME ) ;

  lr = RegQueryValueEx ( hKey, value, 0, 0, (LPBYTE) OEMKey, &dwcb);
  RegCloseKey ( hKey ) ;

  if ( lr != ERROR_SUCCESS ) return false ;

  // Open OEM Key from ...MediaProperties
  sprintf ( key, "%s\\%s", REGSTR_PATH_JOYOEM, OEMKey ) ;

  lr = RegOpenKeyEx ( HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &hKey ) ;

  if ( lr != ERROR_SUCCESS ) return false ;

  // Get OEM Name
  dwcb = buf_sz ;

  lr = RegQueryValueEx ( hKey, REGSTR_VAL_JOYOEMNAME, 0, 0, (LPBYTE) buf,
                           &dwcb ) ;
  RegCloseKey ( hKey ) ;

  if ( lr != ERROR_SUCCESS ) return false ;

  return true ;
}


void jsJoystick::open ()
{
  name [0] = '\0' ;

  js . dwFlags = JOY_RETURNALL ;
  js . dwSize  = sizeof ( js ) ;

  memset ( &jsCaps, 0, sizeof(jsCaps) ) ;

  error = ( joyGetDevCaps( js_id, &jsCaps, sizeof(jsCaps) )
                 != JOYERR_NOERROR ) ;
  if ( jsCaps.wNumAxes == 0 )
  {
    num_axes = 0 ;
    setError () ;
  }
  else
  {
    // Device name from jsCaps is often "Microsoft PC-joystick driver",
    // at least for USB.  Try to get the real name from the registry.
    if ( ! getOEMProductName ( name, sizeof(name) ) )
    {
      ulSetError ( UL_WARNING,
                   "JS: Failed to read joystick name from registry" ) ;

      strncpy ( name, jsCaps.szPname, sizeof(name) ) ;
    }

    // Windows joystick drivers may provide any combination of
    // X,Y,Z,R,U,V,POV - not necessarily the first n of these.
    if ( jsCaps.wCaps & JOYCAPS_HASPOV )
    {
      num_axes = _JS_MAX_AXES ;
      min [ 7 ] = -1.0 ; max [ 7 ] = 1.0 ;  // POV Y
      min [ 6 ] = -1.0 ; max [ 6 ] = 1.0 ;  // POV X
    }
    else
      num_axes = 6 ;

    min [ 5 ] = (float) jsCaps.wVmin ; max [ 5 ] = (float) jsCaps.wVmax ;
    min [ 4 ] = (float) jsCaps.wUmin ; max [ 4 ] = (float) jsCaps.wUmax ;
    min [ 3 ] = (float) jsCaps.wRmin ; max [ 3 ] = (float) jsCaps.wRmax ;
    min [ 2 ] = (float) jsCaps.wZmin ; max [ 2 ] = (float) jsCaps.wZmax ;
    min [ 1 ] = (float) jsCaps.wYmin ; max [ 1 ] = (float) jsCaps.wYmax ;
    min [ 0 ] = (float) jsCaps.wXmin ; max [ 0 ] = (float) jsCaps.wXmax ;
  }

  for ( int i = 0 ; i < num_axes ; i++ )
  {
    center    [ i ] = ( max[i] + min[i] ) / 2.0f ;
    dead_band [ i ] = 0.0f ;
    saturate  [ i ] = 1.0f ;
  }
}


void jsJoystick::close ()
{
}



jsJoystick::jsJoystick ( int ident )
{
  id = ident ;

  switch ( ident )
  {
    case 0  : js_id = JOYSTICKID1 ; open () ; break ;
    case 1  : js_id = JOYSTICKID2 ; open () ; break;
    default :    num_axes = 0 ; setError () ; break ;
  }
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

  MMRESULT status = joyGetPosEx ( js_id, &js ) ;

  if ( status != JOYERR_NOERROR )
  {
    setError() ;
    return ;
  }

  if ( buttons != NULL )
    *buttons = (int) js.dwButtons ;

  if ( axes != NULL )
  {
    /* WARNING - Fall through case clauses!! */

    switch ( num_axes )
    {
      case 8:
        // Generate two POV axes from the POV hat angle.
        // Low 16 bits of js.dwPOV gives heading (clockwise from ahead) in
        //   hundredths of a degree, or 0xFFFF when idle.

        if ( ( js.dwPOV & 0xFFFF ) == 0xFFFF )
        {
          axes [ 6 ] = 0.0 ;
          axes [ 7 ] = 0.0 ;
        }
        else
        {
          // This is the contentious bit: how to convert angle to X/Y.
          //    wk: I know of no define for PI that we could use here:
          //    SG_PI would pull in sg, M_PI is undefined for MSVC
          // But the accuracy of the value of PI is very unimportant at
          // this point.

          float s = (float) sin ( ( js.dwPOV & 0xFFFF ) * ( 0.01 * 3.1415926535f / 180 ) ) ;
          float c = (float) cos ( ( js.dwPOV & 0xFFFF ) * ( 0.01 * 3.1415926535f / 180 ) ) ;

          // Convert to coordinates on a square so that North-East
          // is (1,1) not (.7,.7), etc.
          // s and c cannot both be zero so we won't divide by zero.
          if ( fabs ( s ) < fabs ( c ) )
          {
            axes [ 6 ] = ( c < 0.0 ) ? -s/c  : s/c  ;
            axes [ 7 ] = ( c < 0.0 ) ? -1.0f : 1.0f ;
          }
          else
          {
            axes [ 6 ] = ( s < 0.0 ) ? -1.0f : 1.0f ;
            axes [ 7 ] = ( s < 0.0 ) ? -c/s  : c/s  ;
          }
        }

      case 6: axes[5] = (float) js . dwVpos ;
      case 5: axes[4] = (float) js . dwUpos ;
      case 4: axes[3] = (float) js . dwRpos ;
      case 3: axes[2] = (float) js . dwZpos ;
      case 2: axes[1] = (float) js . dwYpos ;
      case 1: axes[0] = (float) js . dwXpos ;
              break;

      default:
        ulSetError ( UL_WARNING, "PLIB_JS: Wrong num_axes. Joystick input is now invalid" ) ;
    }
  }
}

#endif


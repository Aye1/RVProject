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

     $Id: psl.cxx,v 1.6 2002/09/27 21:19:59 ude Exp $
*/


#include "pslLocal.h"

int _pslInitialised = FALSE ;
static char *_pslScriptPath  = NULL  ;

void pslInit () { _pslInitialised = TRUE ; }

void pslScriptPath ( const char *path )
{
  delete [] _pslScriptPath ;
  _pslScriptPath = ulStrDup ( path ) ;
}


char* _pslMakeScriptPath ( char* path, const char* fname )
{
  if ( fname != NULL && fname [ 0 ] != '\0' )
  {
    if ( ! ulIsAbsolutePathName ( fname ) &&
       _pslScriptPath != NULL && _pslScriptPath[0] != '\0' )
    {
      strcpy ( path, _pslScriptPath ) ;
      strcat ( path, "/" ) ;
      strcat ( path, fname ) ;
    }
    else
      strcpy ( path, fname ) ;
 
    /* Convert backward slashes to forward slashes */

    for ( char* ptr = path ; *ptr ; ptr ++ )
      if ( *ptr == '\\' )
        *ptr = '/' ;
  }
  else
    path [0] = 0 ;

  return path ;
}




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

     $Id: puInputBase.cxx,v 1.1 2002/09/27 23:57:33 ude Exp $
*/


#include "puLocal.h"

UL_RTTI_DEF(puInputBase)

void puInputBase::normalizeCursors ( void )
{
  int sl = strlen ( widget -> getStringValue () ) ;

  /* Clamp the positions to the limits of the text.  */

  if ( cursor_position       <  0 ) cursor_position       =  0 ;
  if ( select_start_position <  0 ) select_start_position =  0 ;
  if ( select_end_position   <  0 ) select_end_position   =  0 ;
  if ( cursor_position       > sl ) cursor_position       = sl ;
  if ( select_start_position > sl ) select_start_position = sl ;
  if ( select_end_position   > sl ) select_end_position   = sl ;

  /* Swap the ends of the select window if they get crossed over */

  if ( select_end_position < select_start_position )
  {
    int tmp = select_end_position ;
    select_end_position = select_start_position ;
    select_start_position = tmp ;
  }
}

void puInputBase::removeSelectRegion ( void )
{
  char *text = widget -> getStringValue () ;
  int text_len = strlen ( text ) ;
  char *p = new char [ text_len + 1 -
                           ( select_end_position - select_start_position ) ] ;

  memcpy ( p, text, select_start_position ) ;
  memcpy ( p + select_start_position,
           text + select_end_position,
           text_len - select_end_position + 1 ) ;

  widget -> setValue ( p ) ;
  delete [] p ;

  cursor_position = select_end_position = select_start_position ;
}


void puInputBase::addValidData ( const char *data )
{
  int valid_len    = valid_data != NULL ? strlen ( valid_data ) : 0 ;
  int data_len     = data       != NULL ? strlen ( data       ) : 0 ;
  int new_data_len = valid_len + data_len ;

  char *new_data = new char [ new_data_len + 1 ] ;

  if ( valid_len != 0 )
    memcpy ( new_data, valid_data, valid_len ) ;
  if ( data_len  != 0 )
    memcpy ( new_data + valid_len, data, data_len ) ;

  new_data [ new_data_len ] = '\0' ;
  delete [] valid_data ;
  valid_data = new_data ;
}


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

     $Id: puFont.cxx,v 1.23 2004/02/16 13:49:03 stromberg Exp $
*/


#include "puLocal.h"


puFont PUFONT_8_BY_13        ( fntGetBitmapFont( FNT_BITMAP_8_BY_13        ) ) ;
puFont PUFONT_9_BY_15        ( fntGetBitmapFont( FNT_BITMAP_9_BY_15        ) ) ;
puFont PUFONT_TIMES_ROMAN_10 ( fntGetBitmapFont( FNT_BITMAP_TIMES_ROMAN_10 ) ) ;
puFont PUFONT_TIMES_ROMAN_24 ( fntGetBitmapFont( FNT_BITMAP_TIMES_ROMAN_24 ) ) ;
puFont PUFONT_HELVETICA_10   ( fntGetBitmapFont( FNT_BITMAP_HELVETICA_10   ) ) ;
puFont PUFONT_HELVETICA_12   ( fntGetBitmapFont( FNT_BITMAP_HELVETICA_12   ) ) ;
puFont PUFONT_HELVETICA_18   ( fntGetBitmapFont( FNT_BITMAP_HELVETICA_18   ) ) ;


puFont::puFont ()
{
  fnt_font_handle  = fntGetBitmapFont( FNT_BITMAP_8_BY_13 );
  pointsize = 13 ;
  slant = 0 ;
}


float puFont::getFloatStringWidth ( const char *str ) const
{
  if ( str == NULL )
    return 0.0f ;

  if ( fnt_font_handle != NULL )
  {
    float r, l ;
    fnt_font_handle -> getBBox ( str, pointsize, slant, &l, &r, NULL, NULL ) ;
    return ( r - l ) ;
  }

  return 0.0f ;
}


static int count_lines ( const char *str )
{
  int k = 0 ;

  if ( str != NULL )
  {
    for ( k = 1 ; *str != '\0' ; str++ )
      if ( *str == '\n' )
        k++ ;
  }

  return k ;
}


int puFont::getStringHeight ( const char *s ) const
{
  /* Height *excluding* descender */

  if ( s == NULL )
    return 0 ;

  if ( fnt_font_handle != NULL )
    return int ( pointsize * ( 1.333f * count_lines ( s ) - 0.333f ) ) ;

  return 0 ;
}


int puFont::getStringDescender ( void ) const
{
  if ( fnt_font_handle != NULL )
  {
    float b ;
    fnt_font_handle -> getBBox ( "y", pointsize, slant, NULL, NULL, &b, NULL ) ;
    return (int) -b ;
  }

  return 0 ;
}


void puFont::drawString ( const char *str, int x, int y )
{
  if ( str == NULL )
    return ;

  if ( fnt_font_handle != NULL )
  {
    sgVec3 curpos ;
    sgSetVec3 ( curpos, (float)x, (float)y, 0.0f ) ;

    glPushAttrib( GL_COLOR_BUFFER_BIT ); // NHV
      glEnable    ( GL_ALPHA_TEST   ) ;
      glEnable    ( GL_BLEND        ) ;
      glAlphaFunc ( GL_GREATER, 0.1f ) ;
      glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

      fnt_font_handle -> begin () ;
        fnt_font_handle -> puts ( curpos, pointsize, slant, str ) ;
      fnt_font_handle -> end () ;

    glPopAttrib () ;
    glDisable ( GL_TEXTURE_2D ) ;
    return ;
  }
}


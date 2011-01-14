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

     $Id: ssgaScreenDump.cxx,v 1.3 2004/02/16 02:26:28 sjbaker Exp $
*/


#include "ssgAux.h"
#include <string.h>


#include "ul.h"

static void writeByte ( FILE *fd, unsigned char x )
{
  fwrite ( & x, sizeof(unsigned char), 1, fd ) ;
}


static void writeShort ( FILE *fd, unsigned short x )
{
  x = ulEndianBig16 ( x ) ;
  fwrite ( & x, sizeof(unsigned short), 1, fd ) ;
}


static void writeInt ( FILE *fd, unsigned int x )
{
  x = ulEndianBig32 ( x ) ;
  fwrite ( & x, sizeof(unsigned int), 1, fd ) ;
}


void ssgaScreenDump ( char *filename, int xsize, int ysize, int frontBuffer )
{
  FILE *fd = fopen ( filename, "wb" ) ;

  if ( fd == NULL )
  {
    fprintf ( stderr, "Failed to open '%s' for writing screendump.\n", 
                       filename ) ;
    return ;
  }

  unsigned char *row    = new unsigned char [ xsize ] ;
  unsigned char *buffer = ssgaScreenDump ( xsize, ysize, frontBuffer ) ;

  char  type  =   0 /* RGB_IMG_VERBATIM */ ;
  short dim   =   3 ;
  short zsize =   3 ;
  char  bpp   =   1 ;
  int   min   =   0 ;
  int   max   = 255 ;
  short magic = 0x01DA /* RGB_IMG_MAGIC */ ;
  int   colormap = 0 ;
  int   i ;

  writeShort ( fd, magic ) ;
  writeByte  ( fd, type  ) ;
  writeByte  ( fd, bpp   ) ;
  writeShort ( fd, dim   ) ;
  writeShort ( fd, xsize ) ;
  writeShort ( fd, ysize ) ;
  writeShort ( fd, zsize ) ;
  writeInt   ( fd, min   ) ;
  writeInt   ( fd, max   ) ;
  writeInt   ( fd, 0 ) ;  /* Dummy field */

  for ( i = 0 ; i < 80 ; i++ )
    writeByte ( fd, '\0' ) ;         /* Name field */

  writeInt ( fd, colormap ) ;

  for ( i = 0 ; i < 404 ; i++ )
    writeByte ( fd, 0 ) ;         /* Dummy field */

  for ( int z = 0 ; z < 3 ; z++ )
    for ( int y = 0 ; y < ysize ; y++ )
    {
      for ( i = 0 ; i < xsize ; i++ )
        row [ i ] = buffer [ ( y * xsize + i ) * 3 + z ] ;

      fseek ( fd, ( z * ysize + y ) * xsize + 512, SEEK_SET ) ;
      fwrite ( row, 1, xsize, fd ) ;
    }

  fclose ( fd ) ;

  delete row ;
  delete buffer ;
}


unsigned char *ssgaScreenDump ( int xsize, int ysize, int frontBuffer )
{
  unsigned char *buffer = new unsigned char [ xsize * ysize * 3 ] ;

  if ( frontBuffer )
    glReadBuffer ( GL_FRONT ) ;

  glReadPixels( 0, 0, xsize, ysize, GL_RGB, GL_UNSIGNED_BYTE,
                                                       (void *) buffer ) ;
  if ( frontBuffer )
    glReadBuffer ( GL_BACK ) ;

  return buffer ;
}



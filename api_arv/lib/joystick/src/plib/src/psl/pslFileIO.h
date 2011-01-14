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

     $Id: pslFileIO.h,v 1.4 2003/01/06 05:10:14 sjbaker Exp $
*/

char *_pslGetFname        () ;
int   _pslGetLineNo       () ;

void  _pslPopDefaultFile  () ;
void  _pslPushDefaultFile ( FILE *fd, const char *fname ) ;
void  _pslPushDefaultFile ( const char *memptr, const char *fname ) ;
void  _pslPushDefaultFile ( const char *fname ) ;

void  _pslUnGetChar       ( int c ) ;
int   _pslGetChar         () ;



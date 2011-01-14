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

     $Id: pslSymbol.h,v 1.4 2002/09/06 19:17:23 ude Exp $
*/


class pslSymbol
{
public:
  char      *symbol   ;
  pslAddress address  ;
  int        locality ;

  pslSymbol ()
  {
    symbol   = NULL ;
    address  = 0 ;
    locality = 0 ;
  }

  void set ( const char *s, pslAddress v, int loc )
  {
    symbol   = ulStrDup ( s ) ;
    address  = v ;
    locality = loc ;
  }

  ~pslSymbol () { delete [] symbol ; }
} ;



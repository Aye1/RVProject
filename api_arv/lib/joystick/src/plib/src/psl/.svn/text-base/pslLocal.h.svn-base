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

     $Id: pslLocal.h,v 1.17 2002/09/27 15:45:06 sjbaker Exp $
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "psl.h"
#include "ul.h"

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/* Limits */

#define MAX_ARGS         64
#define MAX_VARIABLE    256
#define MAX_LABEL       256
#define MAX_TOKEN      1024 
#define MAX_CODE      65536
#define MAX_STACK       256 
#define MAX_NESTING      32 
#define MAX_UNGET        64
#define MAX_UNGET_CHAR 1024
#define MAX_SYMBOL  (MAX_VARIABLE + MAX_LABEL)
#define MAX_INCLUDE_DEPTH  8

typedef unsigned short pslAddress  ;

extern int _pslInitialised ;

#include "pslFileIO.h"
#include "pslOpcodes.h"
#include "pslSymbol.h"
#include "pslCompiler.h"
#include "pslContext.h"

char* _pslMakeScriptPath ( char* path, const char* fname ) ;


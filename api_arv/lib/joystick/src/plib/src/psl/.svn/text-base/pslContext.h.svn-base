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

     $Id: pslContext.h,v 1.15 2002/09/22 20:49:05 ude Exp $
*/


class pslContext
{
  pslOpcode          *code       ;
  const pslExtension *extensions ;
  pslProgram         *program    ;

  pslVariable  variable [ MAX_VARIABLE ] ;
  pslValue     stack    [ MAX_STACK    ] ; 
  int          sp ;
  pslAddress   pc ;

  int num_errors   ;
  int num_warnings ;
  int line_no      ;
 
  void bumpErrors   () { num_errors++   ; }
  void bumpWarnings () { num_warnings++ ; }
 
  char *getProgName () const { return program -> getProgName () ; }
 
  void error   ( const char *fmt, ... ) ;
  void warning ( const char *fmt, ... ) ;                                       

  pslVariable *popLValue () ;
  pslVariable *peekLValue () ;

public:

  pslContext ( pslProgram *p )
  {
    code       = p -> getCode       () ;
    extensions = p -> getExtensions () ;
    program    = p ;

    reset () ;
  }

  ~pslContext () {} ;

  int getLineNo () const { return line_no ; }

  void printStack ( FILE *fd ) const ;

  void pushInt    ( int              x ) { stack [ sp++ ] . set ( x ) ; }
  void pushFloat  ( float            x ) { stack [ sp++ ] . set ( x ) ; }
  void pushString ( const char      *x ) { stack [ sp++ ] . set ( x ) ; }
  void pushNumber ( const pslNumber *x ) { stack [ sp++ ] . set ( x ) ; }

  int    peekInt    () const { return stack [ sp-1 ] . getInt    () ; }
  float  peekFloat  () const { return stack [ sp-1 ] . getFloat  () ; }
  char  *peekString () const { return stack [ sp-1 ] . getString () ; }

  void   popVoid    ( int n = 1 ) { sp -= n ; }
  int    popInt     () { return stack [ --sp ] . getInt    () ; }
  float  popFloat   () { return stack [ --sp ] . getFloat  () ; }
  char  *popString  () { return stack [ --sp ] . getString () ; }

  void   popNumber  ( pslNumber *v ) { v -> set ( & stack [ --sp ] ) ; }

  pslResult step  () ;

  pslResult trace ()
  {
    if ( program -> getStackTraceFlag () ) printStack ( stdout ) ;
    program -> getCompiler () -> printInstruction ( stdout, pc ) ;
    fflush ( stdout ) ;
    return step () ;
  }

  void reset ()
  {
    for ( int i = 0 ; i < MAX_VARIABLE ; i++ )
      variable [ i ] . reset () ;

    line_no = -1 ;

    sp = 0 ;
    pc = 0 ;
  }
} ;



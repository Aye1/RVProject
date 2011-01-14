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

     $Id: pslCodeGen.cxx,v 1.29 2002/09/22 18:25:16 sjbaker Exp $
*/


#include "pslLocal.h"


void pslCompiler::genCodeByte ( pslOpcode op )
{
  if ( next_code >= MAX_CODE - 1 )
    error ( "Program too big!" ) ;
  else
    code [ next_code++ ] = op ;
}


void pslCompiler::genCodeAddr ( pslAddress a )
{
  genCodeByte ( a & 0xFF ) ;
  genCodeByte ( ( a >> 8 ) & 0xFF ) ;
}


void pslCompiler::genLineNumber ( int l )
{
  genCodeByte ( OPCODE_LINE_NUMBER ) ;
  genCodeByte ( l & 0xFF ) ;
  genCodeByte ( ( l >> 8 ) & 0xFF ) ;
}


void pslCompiler::genCharConstant ( char c )
{
  /* A bit wasteful but... */

  genCodeByte ( OPCODE_PUSH_INT_CONSTANT ) ;
  genCodeByte ( c ) ;
  genCodeByte ( 0 ) ;
  genCodeByte ( 0 ) ;
  genCodeByte ( 0 ) ;
}


void pslCompiler::genConstant ( const char *c )
{
  int isInteger = TRUE ;

  for ( const char *p = c ; *p != '\0' ; p++ )
    if ( *p == '.' || *p == 'f' || *p == 'F' )
    {
      isInteger = FALSE ;
      break ;
    }

  if ( isInteger )
    genIntConstant ( c ) ;
  else
    genFloatConstant ( c ) ;
}


void pslCompiler::genStringConstant ( const char *c )
{
  genCodeByte ( OPCODE_PUSH_STRING_CONSTANT ) ;

  for ( int i = 0 ; c [ i ] != '\0' ; i++ )
    genCodeByte ( (unsigned char)( c [ i ]) ) ;

  genCodeByte ( '\0' ) ;
}


void pslCompiler::genIntConstant ( int i )
{
  char *ii = (char *) & i ;

  genCodeByte ( OPCODE_PUSH_INT_CONSTANT ) ;
  genCodeByte ( ii [ 0 ] ) ;
  genCodeByte ( ii [ 1 ] ) ;
  genCodeByte ( ii [ 2 ] ) ;
  genCodeByte ( ii [ 3 ] ) ;
}

void pslCompiler::genIntConstant ( const char *c )
{
  int i = (int) strtol ( c, NULL, 0 ) ; 
  genIntConstant ( i ) ;
}

void pslCompiler::genFloatConstant ( const char *c )
{
  float f = (float) atof ( c ) ; 
  char *ff = (char *) & f ;

  genCodeByte ( OPCODE_PUSH_FLOAT_CONSTANT ) ;
  genCodeByte ( ff [ 0 ] ) ;
  genCodeByte ( ff [ 1 ] ) ;
  genCodeByte ( ff [ 2 ] ) ;
  genCodeByte ( ff [ 3 ] ) ;
}

void pslCompiler::genGetParameter ( pslAddress var, int argpos )
{
  genCodeByte ( OPCODE_GET_PARAMETER ) ;
  genCodeByte ( var ) ;
  genCodeByte ( argpos ) ;
}

int pslCompiler::genMakeIntArray ( const char *c )
{
  int a = getVarSymbol ( c ) ;

  genCodeByte ( OPCODE_SET_INT_ARRAY ) ;
  genCodeByte ( a ) ;
  return a ;
} 

int pslCompiler::genMakeFloatArray ( const char *c )
{
  int a = getVarSymbol ( c ) ;

  genCodeByte ( OPCODE_SET_FLOAT_ARRAY ) ;
  genCodeByte ( a ) ;
  return a ;
} 

int pslCompiler::genMakeStringArray ( const char *c )
{
  int a = getVarSymbol ( c ) ;

  genCodeByte ( OPCODE_SET_STRING_ARRAY ) ;
  genCodeByte ( a ) ;
  return a ;
} 

int pslCompiler::genMakeIntVariable ( const char *c )
{
  int a = getVarSymbol ( c ) ;

  genCodeByte ( OPCODE_SET_INT_VARIABLE ) ;
  genCodeByte ( a ) ;
  return a ;
} 

int pslCompiler::genMakeFloatVariable ( const char *c )
{
  int a = getVarSymbol ( c ) ;

  genCodeByte ( OPCODE_SET_FLOAT_VARIABLE ) ;
  genCodeByte ( a ) ;
  return a ;
} 

int pslCompiler::genMakeStringVariable ( const char *c )
{
  int a = getVarSymbol ( c ) ;

  genCodeByte ( OPCODE_SET_STRING_VARIABLE ) ;
  genCodeByte ( a ) ;
  return a ;
} 


void pslCompiler::genVariable ( const char *c, int array_ref )
{
  int a = getVarSymbol ( c ) ;

  genIntConstant ( array_ref ) ;
  genIntConstant ( a ) ;
} 

void pslCompiler::genFetch ()
{
  genCodeByte ( OPCODE_FETCH ) ;
} 

void pslCompiler::genIncrementFetch ()
{
  genCodeByte ( OPCODE_INCREMENT_FETCH ) ;
} 

void pslCompiler::genDecrementFetch ()
{
  genCodeByte ( OPCODE_DECREMENT_FETCH ) ;
} 

void pslCompiler::genIncrementLValue ()
{
  genCodeByte ( OPCODE_INCREMENT_LVALUE ) ;
} 

void pslCompiler::genDecrementLValue ()
{
  genCodeByte ( OPCODE_DECREMENT_LVALUE ) ;
} 

void pslCompiler::genAssignment ()
{
  genCodeByte ( OPCODE_POP_VARIABLE ) ;
} 

void pslCompiler::genAddAssignment ()
{
  genCodeByte ( OPCODE_POP_ADD_VARIABLE ) ;
} 


void pslCompiler::genSubAssignment ()
{
  genCodeByte ( OPCODE_POP_SUB_VARIABLE ) ;
} 


void pslCompiler::genMulAssignment ()
{
  genCodeByte ( OPCODE_POP_MUL_VARIABLE ) ;
} 


void pslCompiler::genModAssignment ()
{
  genCodeByte ( OPCODE_POP_MOD_VARIABLE ) ;
} 


void pslCompiler::genDivAssignment ()
{
  genCodeByte ( OPCODE_POP_DIV_VARIABLE ) ;
} 


void pslCompiler::genAndAssignment ()
{
  genCodeByte ( OPCODE_POP_AND_VARIABLE ) ;
} 


void pslCompiler::genOrAssignment ()
{
  genCodeByte ( OPCODE_POP_OR_VARIABLE ) ;
} 


void pslCompiler::genXorAssignment ()
{
  genCodeByte ( OPCODE_POP_XOR_VARIABLE ) ;
} 


void pslCompiler::genSHLAssignment ()
{
  genCodeByte ( OPCODE_POP_SHL_VARIABLE ) ;
} 


void pslCompiler::genSHRAssignment ()
{
  genCodeByte ( OPCODE_POP_SHR_VARIABLE ) ;
} 


void pslCompiler::genCall ( const char *c, int argc )
{
  int ext = getExtensionSymbol ( c ) ;

  if ( ext < 0 )
  {
    genIntConstant ( argc ) ;
    genCodeByte ( OPCODE_CALL ) ;

    int a = getCodeSymbol ( c, next_code ) ;

    genCodeAddr ( a ) ;
    genCodeByte ( argc ) ;
  }
  else
  {
    genCodeByte ( OPCODE_CALLEXT ) ;
    genCodeByte ( ext ) ;
    genCodeByte ( argc ) ;
  }
} 

void pslCompiler::genExchange     () { genCodeByte ( OPCODE_EXCHANGE   ) ; } 
void pslCompiler::genReturn       () { genCodeByte ( OPCODE_RETURN     ) ; } 
void pslCompiler::genPop          () { genCodeByte ( OPCODE_POP        ) ; } 
void pslCompiler::genSubtract     () { genCodeByte ( OPCODE_SUB        ) ; } 
void pslCompiler::genAdd          () { genCodeByte ( OPCODE_ADD        ) ; } 
void pslCompiler::genDivide       () { genCodeByte ( OPCODE_DIV        ) ; } 
void pslCompiler::genMultiply     () { genCodeByte ( OPCODE_MULT       ) ; } 
void pslCompiler::genModulo       () { genCodeByte ( OPCODE_MOD        ) ; } 
void pslCompiler::genNegate       () { genCodeByte ( OPCODE_NEG        ) ; } 
void pslCompiler::genNot          () { genCodeByte ( OPCODE_NOT        ) ; } 
void pslCompiler::genTwiddle      () { genCodeByte ( OPCODE_TWIDDLE    ) ; } 
void pslCompiler::genOrOr         () { genCodeByte ( OPCODE_OROR       ) ; } 
void pslCompiler::genAndAnd       () { genCodeByte ( OPCODE_ANDAND     ) ; } 
void pslCompiler::genOr           () { genCodeByte ( OPCODE_OR         ) ; } 
void pslCompiler::genAnd          () { genCodeByte ( OPCODE_AND        ) ; } 
void pslCompiler::genXor          () { genCodeByte ( OPCODE_XOR        ) ; } 
void pslCompiler::genShiftLeft    () { genCodeByte ( OPCODE_SHIFTLEFT  ) ; } 
void pslCompiler::genShiftRight   () { genCodeByte ( OPCODE_SHIFTRIGHT ) ; } 

void pslCompiler::genLess         () { genCodeByte ( OPCODE_LESS       ) ; } 
void pslCompiler::genLessEqual    () { genCodeByte ( OPCODE_LESSEQUAL  ) ; } 
void pslCompiler::genGreater      () { genCodeByte ( OPCODE_GREATER    ) ; } 
void pslCompiler::genGreaterEqual () { genCodeByte ( OPCODE_GREATEREQUAL); } 
void pslCompiler::genNotEqual     () { genCodeByte ( OPCODE_NOTEQUAL   ) ; } 
void pslCompiler::genEqual        () { genCodeByte ( OPCODE_EQUAL      ) ; } 

void pslCompiler::genStackDup () { genCodeByte ( OPCODE_STACK_DUPLICATE ) ; } 

int pslCompiler::genPeekJumpIfTrue  ( int l )
{
  genCodeByte ( OPCODE_PEEK_JUMP_TRUE ) ;

  int res = next_code ;

  genCodeAddr ( l ) ;

  return res ;
}

int pslCompiler::genPeekJumpIfFalse  ( int l )
{
  genCodeByte ( OPCODE_PEEK_JUMP_FALSE ) ;

  int res = next_code ;

  genCodeAddr ( l ) ;

  return res ;
}

int pslCompiler::genJumpIfTrue  ( int l )
{
  genCodeByte ( OPCODE_JUMP_TRUE ) ;

  int res = next_code ;

  genCodeAddr ( l ) ;

  return res ;
}

int pslCompiler::genJumpIfFalse  ( int l )
{
  genCodeByte ( OPCODE_JUMP_FALSE ) ;

  int res = next_code ;

  genCodeAddr ( l ) ;

  return res ;
}

int pslCompiler::genJump ( int l )
{
  genCodeByte ( OPCODE_JUMP ) ;

  int res = next_code ;

  genCodeAddr ( l ) ;

  return res ;
}


int pslCompiler::genPauseStatement()
{ 
  genCodeByte ( OPCODE_PAUSE ) ;
  return TRUE ;
} 



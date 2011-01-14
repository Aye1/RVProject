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

     $Id: pslExpression.cxx,v 1.18 2002/09/22 18:25:16 sjbaker Exp $
*/


#include "pslLocal.h"



int pslCompiler::genLValue ()
{
  /*
    Expect:

          variable                   ...or...
          arrayname [ expression ]   ...or...
          structure .  lvalue        ...or...
          structure -> lvalue
  */

  char c [ MAX_TOKEN ] ;
  char n [ MAX_TOKEN ] ;
  getToken ( c ) ;
  getToken ( n ) ;

  int isArray  = ( n[0] == '[' ) ;
  int isStruct = ( strcmp ( n, "."  ) == 0 ||
                   strcmp ( n, "->" ) == 0 ) ;

  if ( ! isArray && ! isStruct )
  {
    ungetToken   ( n ) ;
    genVariable ( c, FALSE ) ;
    return TRUE ;
  }

  if ( isStruct )
    return error ( "Structures are not supported yet." ) ;

  /* An array + index */

  if ( ! genExpression () )
    return error ( "Missing expression for array index." ) ;

  getToken ( n ) ;

  if ( n[0] != ']' )
    return error ( "Missing ']' after array index." ) ;

  genVariable ( c, TRUE ) ;
  return TRUE ;
}




int pslCompiler::genPrimitive ()
{
  /* Expect:

         'x'                       ...or...
         "xxxxxx"                  ...or...
         number                    ...or...
         ( expression )            ...or...
         + primitive               ...or...
         - primitive               ...or...
         ! primitive               ...or...
         ~ primitive               ...or...
         ++ lvalue                 ...or...
         -- lvalue                 ...or...
         function ( expression )   ...or...
         lvalue                    ...or...
         lvalue ++                 ...or...
         lvalue --                 ...or...
         lvalue = expression       ...or...
         lvalue += expression      ...or...
         lvalue -= expression
         (etc)
  */

  char c [ MAX_TOKEN ] ;
  getToken ( c ) ;

  /* Constant ? */

  if ( c [ 0 ] == '\''  ) { genCharConstant   (   c [ 1 ] ) ; return TRUE ; } 
  if ( c [ 0 ] == '"'   ) { genStringConstant ( & c [ 1 ] ) ; return TRUE ; }
  if ((c [ 0 ] == '.' && c [ 1 ] != '\0' ) ||
       isdigit ( c[0] ) ) { genConstant       (   c       ) ; return TRUE ; }


  /* Bracketed expression */

  if ( strcmp ( c, "(" ) == 0 )
  {
    if ( ! genExpression () )
    {
      ungetToken ( c ) ;
      return error ( "Missing expression after '('" ) ;
    }

    getToken ( c ) ;

    if ( strcmp ( c, ")" ) != 0 )
    {
      ungetToken ( c ) ;
      return error ( "Missing ')' (found '%s')", c );
    }

    return TRUE ;
  }

  /* Various unary operators */

  if ( strcmp ( c, "+" ) == 0 )    /* Skip over redundant unary '+' symbol */
  {
    if ( genPrimitive () ) {   /* Nothing */    return TRUE  ; }
                       else { ungetToken ( c ) ; return FALSE ; }
  }

  if ( strcmp ( c, "-" ) == 0 )    /* Unary '-' symbol */
  {
    if ( genPrimitive () ) { genNegate ()    ; return TRUE  ; }
                       else { ungetToken ( c ) ; return FALSE ; }
  }

  if ( strcmp ( c, "!" ) == 0 )    /* Unary '!' symbol */
  {
    if ( genPrimitive () ) { genNot ()       ; return TRUE  ; }
                       else { ungetToken ( c ) ; return FALSE ; }
  }

  if ( strcmp ( c, "~" ) == 0 )    /* Unary '~' symbol */
  {
    if ( genPrimitive () ) { genTwiddle ()   ; return TRUE  ; }
                       else { ungetToken ( c ) ; return FALSE ; }
  }

  /* Pre-increment and pre-decrement */

  if ( strcmp ( c, "++" ) == 0 )
  {
    genLValue () ;
    genIncrementLValue () ;
    genFetch () ;
    return TRUE ;
  }

  if ( strcmp ( c, "--" ) == 0 )
  {
    genLValue () ;
    genDecrementLValue () ;
    genFetch () ;
    return TRUE ;
  }

  /* Something illegal ?!? */

  if ( ! isalpha ( c [ 0 ] ) && c [ 0 ] != '_' )
  {
    ungetToken ( c ) ;
    return FALSE ;
  }

  /*
    An LValue or a function call...
  */

  char n [ MAX_TOKEN ] ;
  getToken ( n ) ;

  if ( n[0] == '(' )
  {
    ungetToken ( n ) ;
    genFunctionCall ( c ) ;
    return TRUE ;
  }

  /*
    An LValue.
  */

  ungetToken ( n ) ;
  ungetToken ( c ) ;

  if ( ! genLValue () )
    return error ( "Illegal expression." ) ;

  getToken ( n ) ;

  if ( strcmp ( n, "++" ) == 0 )
     { genIncrementFetch () ; return TRUE ; }

  if ( strcmp ( n, "--" ) == 0 )
     { genDecrementFetch () ; return TRUE ; }

  if ( strcmp ( n, "="   ) == 0 )
     { if ( genExpression () ) { genAssignment    () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "+="  ) == 0 )
     { if ( genExpression () ) { genAddAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "-="  ) == 0 )
     { if ( genExpression () ) { genSubAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "*="  ) == 0 )
     { if ( genExpression () ) { genMulAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "%="  ) == 0 )
     { if ( genExpression () ) { genModAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "/="  ) == 0 )
     { if ( genExpression () ) { genDivAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "&="  ) == 0 )
     { if ( genExpression () ) { genAndAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "|="  ) == 0 )
     { if ( genExpression () ) { genOrAssignment  () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "^="  ) == 0 )
     { if ( genExpression () ) { genXorAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, "<<=" ) == 0 )
     { if ( genExpression () ) { genSHLAssignment () ; return TRUE ; }
       else return FALSE ; }

  if ( strcmp ( n, ">>=" ) == 0 )
     { if ( genExpression () ) { genSHRAssignment () ; return TRUE ; }
       else return FALSE ; }

  ungetToken ( n ) ;
  genFetch  () ;
  return TRUE ;
}



int pslCompiler::genMultExpression ()
{
  /* Expect:

          primitive              ...or...
          primitive * primitive
          primitive / primitive
          primitive % primitive
  */

  if ( ! genPrimitive () )
    return FALSE ;

  while ( TRUE )
  {
    char c [ MAX_TOKEN ] ;

    getToken ( c ) ;

    if ( strcmp ( c, "*" ) != 0 &&
         strcmp ( c, "/" ) != 0 &&
         strcmp ( c, "%" ) != 0 )
    {
      ungetToken ( c ) ;
      return TRUE ;
    }

    if ( ! genPrimitive () )
      return FALSE ;

    if ( strcmp ( c, "*" ) == 0 )
      genMultiply () ;
    else
    if ( strcmp ( c, "/" ) == 0 )
      genDivide () ;
    else
      genModulo () ;
  }
}




int pslCompiler::genAddExpression ()
{
  /* Expect:

          multExp              ...or...
          multExp + multExp
          multExp - multExp
  */

  if ( ! genMultExpression () )
    return FALSE ;

  while ( TRUE )
  {
    char c [ MAX_TOKEN ] ;

    getToken ( c ) ;

    if ( strcmp ( c, "+" ) != 0 &&
         strcmp ( c, "-" ) != 0 )
    {
      ungetToken ( c ) ;
      return TRUE ;
    }

    if ( ! genMultExpression () )
      return FALSE ;

    if ( strcmp ( c, "+" ) == 0 )
      genAdd () ;
    else
      genSubtract () ;
  }
}




int pslCompiler::genShiftExpression ()
{
  /* Expect:

          addExp              ...or...
          addExp << addExp
          addExp >> addExp
  */

  if ( ! genAddExpression () )
    return FALSE ;

  while ( TRUE )
  {
    char c [ MAX_TOKEN ] ;

    getToken ( c ) ;

    if ( strcmp ( c, "<<" ) != 0 &&
         strcmp ( c, ">>" ) != 0 )
    {
      ungetToken ( c ) ;
      return TRUE ;
    }

    if ( ! genAddExpression () )
      return FALSE ;

    if ( strcmp ( c, "<<" ) == 0 )
      genShiftLeft () ;
    else
      genShiftRight () ;
  }
}



int pslCompiler::genBitwiseExpression ()
{
  /* Expect:

          shiftExp              ...or...
          shiftExp & shiftExp
          shiftExp | shiftExp
          shiftExp ^ shiftExp
  */

  if ( ! genShiftExpression () )
    return FALSE ;

  while ( TRUE )
  {
    char c [ MAX_TOKEN ] ;

    getToken ( c ) ;

    if ( strcmp ( c, "|" ) != 0 &&
         strcmp ( c, "&" ) != 0 &&
         strcmp ( c, "^" ) != 0 )
    {
      ungetToken ( c ) ;
      return TRUE ;
    }

    if ( ! genShiftExpression () )
      return FALSE ;

    if ( strcmp ( c, "|" ) == 0 )
      genOr () ;
    else
    if ( strcmp ( c, "&" ) == 0 )
      genAnd () ;
    else
      genXor () ;
  }
}




int pslCompiler::genRelExpression ()
{
  /* Expect:

          bitwiseExp              ...or...
          bitwiseExp <  bitwiseExp
          bitwiseExp >  bitwiseExp
          bitwiseExp == bitwiseExp
          bitwiseExp <= bitwiseExp
          bitwiseExp >= bitwiseExp
          bitwiseExp != bitwiseExp
  */

  if ( ! genBitwiseExpression () )
    return FALSE ;

  while ( TRUE )
  {
    char c [ MAX_TOKEN ] ;

    getToken ( c ) ;

    if ( strcmp ( c, "<"  ) != 0 &&
         strcmp ( c, ">"  ) != 0 &&
         strcmp ( c, "<=" ) != 0 &&
         strcmp ( c, ">=" ) != 0 &&
         strcmp ( c, "!=" ) != 0 &&
         strcmp ( c, "==" ) != 0 )
    {
      ungetToken ( c ) ;
      return TRUE ;
    }

    if ( ! genBitwiseExpression () )
      return FALSE ;

    if ( strcmp ( c, "<"  ) == 0 ) genLess         () ; else
    if ( strcmp ( c, ">"  ) == 0 ) genGreater      () ; else
    if ( strcmp ( c, "<=" ) == 0 ) genLessEqual    () ; else
    if ( strcmp ( c, ">=" ) == 0 ) genGreaterEqual () ; else
    if ( strcmp ( c, "!=" ) == 0 ) genNotEqual     () ; else
    if ( strcmp ( c, "==" ) == 0 ) genEqual        () ;
  }
}



int pslCompiler::genBoolExpression ()
{
  /* Expect:

          relExp              ...or...
          relExp && boolExp   ...or...
          relExp || boolExp
  */

  if ( ! genRelExpression () )
    return FALSE ;

  char c [ MAX_TOKEN ] ;
  int  shortcut ;

  getToken ( c ) ;

  if ( strcmp ( c, "&&"  ) == 0 )
    shortcut = genPeekJumpIfFalse ( 0 ) ;
  else
  if ( strcmp ( c, "||"  ) == 0 )
    shortcut = genPeekJumpIfTrue ( 0 ) ;
  else
  {
    ungetToken ( c ) ;
    return TRUE ;
  }

  if ( ! genBoolExpression () )
    return error ( "Missing expression following '&&' or '||'" ) ;

  code [ shortcut   ] =   next_code        & 0xFF ;
  code [ shortcut+1 ] = ( next_code >> 8 ) & 0xFF ;

  return TRUE ;
}


int pslCompiler::genExpression ()
{
  /* All expressions can be bool expressions */
  return genBoolExpression () ;
}



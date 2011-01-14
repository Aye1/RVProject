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

     $Id: pslCompiler.cxx,v 1.29 2003/01/06 05:10:13 sjbaker Exp $
*/


#include "pslLocal.h"
#include "ul.h"

 
int pslCompiler::compile ( const char *fname )
{
  char filename [ 1024 ] ;
  _pslMakeScriptPath ( filename, fname ) ;                        

  FILE *fd = fopen ( filename, "ra" ) ;
 
  if ( fd == NULL )
  {
    perror ( "PSL:" ) ;
    ulSetError ( UL_WARNING, "PSL: Failed while opening '%s' for reading.",
                                                                  filename );
    return FALSE ;
  }
 
  return compile ( fd, fname ) ;
}
 
 
int pslCompiler::compile ( const char *memptr, const char *fname )
{
  init () ;
 
  _pslPushDefaultFile ( memptr, (fname == NULL) ? progName : fname ) ;
  genProgram     () ;
  _pslPopDefaultFile  () ;
 
  return cleanup () ;
}


int pslCompiler::compile ( FILE *fd, const char *fname )
{
  init () ;
 
  _pslPushDefaultFile ( fd, (fname == NULL) ? progName : fname ) ;
  genProgram     () ;
  _pslPopDefaultFile  () ;
 
  return cleanup () ;
}


int pslCompiler::cleanup ()
{
  const char *dump_env = getenv ( "PSL_DUMP" ) ;

  if ( num_errors != 0 || num_warnings != 0 )
    fprintf ( stderr, "PSL: '%s' Compiled with %d Warnings, %d Fatal Errors\n",
             progName, num_warnings, num_errors ) ;
 
  /* If there are errors, prevent the program from running. */

  if ( num_errors != 0 )
  {
    if ( dump_env != NULL &&
         ulStrEqual ( dump_env, "on_error" ) )
      dump () ;

    next_code = 0 ;
    genCodeByte ( OPCODE_HALT ) ;
  }
  else
  if ( dump_env != NULL &&
       ulStrEqual ( dump_env, "always" ) )
    dump () ;

  return num_errors ;
}



int pslCompiler::genReturnStatement ()
{
  char c [ MAX_TOKEN ] ;

  getToken ( c ) ;

  if ( c [ 0 ] == ';' )   /* Return without data == "return 0" */
  {
    ungetToken   ( c ) ;
    genConstant ( "0.0" ) ;
  }
  else
  {
    ungetToken     ( c ) ;
    genExpression () ;
  }

  genReturn () ;
  return TRUE ;
}



/* Administer the break/continue jump addresses */

void pslCompiler::pushBreakToLabel ()
{
  if ( next_break >= MAX_LABEL-1 )
    error ( "Too many nested 'break' contexts" ) ;
  else
    breakToAddressStack [ next_break++ ] = next_tmp_label++ ;
}

void pslCompiler::pushNoContinue ()
{
  continueToAddressStack [ next_continue++ ] = -1 ;
}

int pslCompiler::pushContinueToLabel ()
{
  if ( next_continue >= MAX_LABEL-1 )
    error ( "Too many nested 'continue' contexts" ) ;
  else
    continueToAddressStack [ next_continue++ ] = next_tmp_label++ ;

  return next_tmp_label-1 ;
}

void pslCompiler::setContinueToLabel ( int which )
{
  char s [ 10 ] ;
  sprintf ( s, "L%d", which ) ;
  setCodeSymbol ( s, next_code ) ;
}

void pslCompiler::popBreakToLabel ()
{
  char s [ 10 ] ;
  sprintf ( s, "L%d", breakToAddressStack[next_break-1] ) ;
  setCodeSymbol ( s, next_code ) ;
  next_break-- ;
}

void pslCompiler::popContinueToLabel ()
{
  next_continue-- ;
}



/* Implement actual break and continue statements. */

int pslCompiler::genBreakStatement ()
{
  if ( next_break <= 0 )
    return error ( "'break' statement is not inside a 'switch' or a loop." ) ;

  char s [ 10 ] ;
  sprintf ( s, "L%d", breakToAddressStack [ next_break-1 ] ) ;
  genJump ( getCodeSymbol ( s, next_code+1 ) ) ;
  return TRUE ;
}


int pslCompiler::genContinueStatement ()
{
  if ( next_break <= 0 )
    return error ( "'continue' statement is not inside a loop." ) ;

  if ( continueToAddressStack [ next_continue-1 ] < 0 )
    return error ( "'continue' statement not allowed inside a 'switch'." ) ;

  char s [ 10 ] ;
  sprintf ( s, "L%d", continueToAddressStack [ next_continue-1 ] ) ;
  genJump ( getCodeSymbol ( s, next_code+1 ) ) ;
  return TRUE ;
}





int pslCompiler::genSwitchStatement ()
{
  if ( ! genExpression () )
    return error ( "Missing control expression for 'switch'" ) ;

  char c [ MAX_TOKEN ] ;

  getToken ( c ) ;    /* Hopefully, the word 'while' */
  
  if ( c [ 0 ] != '{' )
    return error ( "Missing '{' after 'switch'" ) ;

  int jumpToNextCase = genJump ( 0 ) ;
  int jumpAfterTest  = 0 ;

  pushBreakToLabel () ;
  pushNoContinue   () ;

  while ( TRUE )
  {
    getToken ( c ) ;

    if ( strcmp ( c, "case" ) == 0 )
    {
      jumpAfterTest = genJump ( 0 ) ;

      code [ jumpToNextCase   ] =   next_code        & 0xFF ;
      code [ jumpToNextCase+1 ] = ( next_code >> 8 ) & 0xFF ;

      genStackDup () ;

      if ( ! genExpression () )
        error ( "Missing expression after 'case'." ) ;

      getToken ( c ) ;

      if ( c[0] != ':' )
        error ( "Missing ':' after 'case' expression." ) ;

      genEqual () ;

      jumpToNextCase = genJumpIfFalse ( 0 ) ;

      code [ jumpAfterTest   ] = next_code & 0xFF ;
      code [ jumpAfterTest+1 ] = ( next_code >> 8 ) & 0xFF ;
    }
    else
    if ( strcmp ( c, "default" ) == 0 )
    {
      code [ jumpToNextCase   ] =   next_code        & 0xFF ;
      code [ jumpToNextCase+1 ] = ( next_code >> 8 ) & 0xFF ;

      getToken ( c ) ;

      if ( c[0] != ':' )
        error ( "Missing ':' after 'default'." ) ;
    }
    else
    if ( strcmp ( c, "}" ) == 0 )
    {
      ungetToken ( ";" ) ;
      break ;
    }
    else
    {
      ungetToken ( c ) ;

      if ( ! genStatement () )
        error ( "Missing statement within switch." ) ;

      getToken ( c ) ;

      if ( c [ 0 ] != ';' )
        error ( "Missing semicolon." ) ;
    }
  }

  popBreakToLabel    () ;
  popContinueToLabel () ;
  genPop () ;
  return TRUE ;
}



int pslCompiler::genDoWhileStatement ()
{
  /* Remember place to jump back to */

  int start_loc = next_code ;

  pushBreakToLabel    () ;
  setContinueToLabel ( pushContinueToLabel () ) ;

  if ( ! genStatement () )
    return error ( "Missing statement for 'do/while'" ) ;

  char c [ MAX_TOKEN ] ;

  getToken ( c ) ;    /* The final ';' of the action */

  getToken ( c ) ;    /* Hopefully, the word 'while' */
  
  if ( strcmp ( c, "while" ) != 0 )
    return error ( "Missing 'while' for 'do/while'" ) ;

  if ( ! genExpression () )
    return error ( "Missing expression for 'while' in a 'do/while'" ) ;

  genJumpIfTrue ( start_loc ) ;

  popBreakToLabel    () ;
  popContinueToLabel () ;
  return TRUE ;
}


int pslCompiler::genForStatement ()
{
  char c [ MAX_TOKEN ] ;

  pushLocality        () ;
  pushBreakToLabel    () ;
  int ct_lab = pushContinueToLabel () ;

  getToken ( c ) ;    /* The initial '(' of the action */

  if ( c [ 0 ] != '(' )
  {
    popLocality () ;
    return error ( "Missing '(' for 'for' loop" ) ;
  }

  if ( ! genStatement () )
  {
    popLocality () ;
    return error ( "Missing initialiser for 'if'" ) ;
  }

  getToken ( c ) ;    /* The ';' after the initialiser */

  if ( c [ 0 ] != ';' )
  {
    popLocality () ;
    return error ( "Missing ';' after 'for' loop initialisation" ) ;
  }

  /* Remember place to jump back to */

  int start_loc = next_code ;

  /* The test */

  if ( ! genExpression () )
    return error ( "Missing test for 'for' loop" ) ;

  getToken ( c ) ;    /* The ';' after the initialiser */

  if ( c [ 0 ] != ';' )
  {
    popLocality () ;
    return error ( "Missing ';' after 'for' loop test" ) ;
  }

  char saved [ MAX_UNGET ][ MAX_TOKEN ] ;
  int next_saved    = 0 ;
  int paren_counter = 0 ;

  do
  {
    getToken ( saved [ next_saved ] ) ;

    if ( saved [ next_saved ][ 0 ] == '(' ) paren_counter++ ;
    if ( saved [ next_saved ][ 0 ] == ')' ) paren_counter-- ;

    if ( next_saved >= MAX_UNGET-1 )
    {
      popLocality () ;
      return error ( "Too many tokens in 'increment' part of 'for' loop" ) ;
    }

    next_saved++ ;

  } while ( paren_counter >= 0 ) ;
 
  next_saved-- ;  /* Throw away the ')' */

  int label_loc = genJumpIfFalse ( 0 ) ;

  if ( ! genStatement () )
  {
    popLocality () ;
    return error ( "Missing action body for 'for' loop" ) ;
  }
 
  setContinueToLabel ( ct_lab ) ;

  getToken ( c ) ;   /* Throw away the ';' */

  /* Put the increment test back onto the token stream */

  ungetToken ( ";" ) ;    

  for ( int i = next_saved-1 ; i >= 0 ; i-- )
    ungetToken ( saved[i] ) ;    

  if ( ! genStatement () )
  {
    popLocality () ;
    return error ( "Missing 'increment' part of 'for' loop" ) ;
  }

  genJump ( start_loc ) ;

  code [ label_loc   ] = next_code & 0xFF ;
  code [ label_loc+1 ] = ( next_code >> 8 ) & 0xFF ;

  popBreakToLabel    () ;
  popContinueToLabel () ;
  popLocality        () ;
  return TRUE ;
}


int pslCompiler::genWhileStatement ()
{
  /* Remember place to jump back to */

  pushBreakToLabel    () ;
  setContinueToLabel ( pushContinueToLabel () ) ;

  int start_loc = next_code ;

  if ( ! genExpression () )
    return error ( "Missing expression for 'while'" ) ;

  int label_loc = genJumpIfFalse ( 0 ) ;

  if ( ! genStatement () )
    return error ( "Missing statement for 'while'" ) ;

  genJump ( start_loc ) ;

  code [ label_loc   ] = next_code & 0xFF ;
  code [ label_loc+1 ] = ( next_code >> 8 ) & 0xFF ;

  popBreakToLabel    () ;
  popContinueToLabel () ;

  return TRUE ;
}


int pslCompiler::genIfStatement ()
{
  if ( ! genExpression () )
    return error ( "Missing expression for 'if'" ) ;

  int else_loc = genJumpIfFalse ( 0 ) ;

  if ( ! genStatement () )
    return error ( "Missing statement for 'if'" ) ;

  char c [ MAX_TOKEN ] ;

  getToken ( c ) ;

  if ( c [ 0 ] != ';' )
  {
    ungetToken ( c ) ;
    return error ( "Missing ';' or 'else' after 'if' statement" ) ;
  }

  getToken ( c ) ;

  if ( strcmp ( c, "else" ) != 0 )
  {
    code [ else_loc   ] = next_code & 0xFF ;
    code [ else_loc+1 ] = ( next_code >> 8 ) & 0xFF ;

    ungetToken ( c ) ;
    ungetToken ( ";" ) ;
    return TRUE ;
  }

  int label_loc = genJump ( 0 ) ;

  code [ else_loc   ] = next_code & 0xFF ;
  code [ else_loc+1 ] = ( next_code >> 8 ) & 0xFF ;

  if ( ! genStatement () )
    return error ( "Missing statement for 'else'" ) ;

  code [ label_loc   ] = next_code & 0xFF ;
  code [ label_loc+1 ] = ( next_code >> 8 ) & 0xFF ;

  return TRUE ;
}


int pslCompiler::genFunctionCall ( const char *var )
{
  char c [ MAX_TOKEN ] ;

  getToken ( c ) ;

  /*
    'var' should be the name of a function,
    'c'   should be an '('
  */

  if ( c[0] != '(' )
    return error ( "Missing '(' in call to '%s'", var ) ;

  getToken ( c ) ;

  int argc = 0 ;

  while ( c[0] != ')' )
  { 
    ungetToken ( c ) ;
    genExpression () ;
    argc++ ;
    getToken ( c ) ;

    if ( c[0] == ')' )
      break ;

    if ( c[0] != ',' )
      return error ( "Missing ')' or ',' in call to '%s'", var ) ;

    getToken ( c ) ;
  }

  genCall ( var, argc ) ;
  return TRUE ;
}


int pslCompiler::genCompoundStatement ()
{
  char c [ MAX_TOKEN ] ;

  pushLocality () ;

  while ( genStatement () )
  {
    getToken ( c ) ;

    if ( c[0] != ';' )
    {
      popLocality () ;
      return warning ( "Unexpected '%s' in Compound statement", c ) ;
    }
  }

  getToken ( c ) ;

  if ( c[0] == '}' )
  {
    popLocality () ;
    ungetToken ( ";" ) ;
    return TRUE ;
  }

  popLocality () ;
  ungetToken ( c ) ;
  return warning ( "Unexpected '%s' in Compound statement", c ) ;
}


int pslCompiler::genStatement ()
{
  char c [ MAX_TOKEN ] ;

  if ( generate_line_numbers )
    genLineNumber ( _pslGetLineNo () ) ;

  getToken ( c ) ;

  if ( strcmp ( c, "static"   ) == 0 ) return genStaticVarDecl      () ;
  if ( strcmp ( c, "string"   ) == 0 ) return genLocalVarDecl ( PSL_STRING) ;
  if ( strcmp ( c, "int"      ) == 0 ) return genLocalVarDecl ( PSL_INT   ) ;
  if ( strcmp ( c, "float"    ) == 0 ) return genLocalVarDecl ( PSL_FLOAT ) ;
  if ( strcmp ( c, "return"   ) == 0 ) return genReturnStatement    () ;
  if ( strcmp ( c, "break"    ) == 0 ) return genBreakStatement     () ;
  if ( strcmp ( c, "continue" ) == 0 ) return genContinueStatement  () ;
  if ( strcmp ( c, "pause"    ) == 0 ) return genPauseStatement     () ;
  if ( strcmp ( c, "for"      ) == 0 ) return genForStatement       () ;
  if ( strcmp ( c, "do"       ) == 0 ) return genDoWhileStatement   () ;
  if ( strcmp ( c, "switch"   ) == 0 ) return genSwitchStatement    () ;
  if ( strcmp ( c, "while"    ) == 0 ) return genWhileStatement     () ;
  if ( strcmp ( c, "if"       ) == 0 ) return genIfStatement        () ;
  if ( strcmp ( c, "case"     ) == 0 || strcmp ( c, "default" ) == 0 )
    return error ( "'%s' encountered - not inside 'switch' statement", c ) ;

  if ( c [ 0 ] == '{' ) return genCompoundStatement () ;

  ungetToken ( c ) ; 

  if ( genExpression () )
  {
    genPop () ;   /* Discard result */
    return TRUE ;
  }

  return FALSE ;
}


void pslCompiler::genProgram ()
{
  char c [ MAX_TOKEN ] ;

  /* Compile the program */

  while ( TRUE )
  {
    getToken ( c ) ;

    if ( c[0] == '\0' )
      break ;

    ungetToken ( c ) ;

    genGlobalDeclaration () ;
  }

  /* Have the program call 'main' and then halt */

  genIntConstant ( 0 ) ;  /* No arguments to main *YET*  */

  genCodeByte ( OPCODE_CALL ) ;
  genCodeAddr ( getCodeSymbol ( "main", next_code ) ) ;
  genCodeByte ( 0 ) ;  /* Argc */
  genCodeByte ( OPCODE_HALT ) ;

  checkUnresolvedSymbols () ;
}



int pslCompiler::genLocalVarDecl ( pslType t )
{
  char c  [ MAX_TOKEN ] ;
  char s  [ MAX_TOKEN ] ;

  getToken ( s ) ;

  setVarSymbol ( s ) ;

  getToken ( c ) ;

  if ( c[0] == '[' )
  {
    genExpression () ;

    switch ( t )
    {
      case PSL_FLOAT  : genMakeFloatArray  ( s ) ; break ;
      case PSL_STRING : genMakeStringArray ( s ) ; break ;
      default :         genMakeIntArray    ( s ) ; break ;
    }

    getToken ( c ) ;

    if ( c[0] != ']' )
      return error ( "Missing ']' after array declaration" ) ; 

    return TRUE ;
  }

  switch ( t )
  {
    case PSL_FLOAT  : genMakeFloatVariable  ( s ) ; break ;
    case PSL_STRING : genMakeStringVariable ( s ) ; break ;
    default :         genMakeIntVariable    ( s ) ; break ;
  }

  if ( strcmp ( c, "=" ) == 0 )
  {
    genVariable ( s, FALSE ) ;
    genExpression () ;
    genAssignment () ;
    genPop        () ;
    return TRUE ;
  }

  ungetToken ( c ) ;
  return TRUE ;
}



int pslCompiler::genStaticVarDecl ()
{
  return error ( "Static Local Variables are Not Supported Yet." ) ;
}



int pslCompiler::genGlobalVarDecl ( const char *s, pslType t )
{
  char c  [ MAX_TOKEN ] ;

  setVarSymbol ( s ) ;

  getToken ( c ) ;

  if ( c[0] == '[' )
  {
    genExpression () ;

    switch ( t )
    {
      case PSL_FLOAT  : genMakeFloatArray  ( s ) ; break ;
      case PSL_STRING : genMakeStringArray ( s ) ; break ;
      default :         genMakeIntArray    ( s ) ; break ;
    }

    getToken ( c ) ;

    if ( c[0] != ']' )
      return error ( "Missing ']' after array declaration" ) ; 
  }
  else
  {
    switch ( t )
    {
      case PSL_FLOAT  : genMakeFloatVariable  ( s ) ; break ;
      case PSL_STRING : genMakeStringVariable ( s ) ; break ;
      default :         genMakeIntVariable    ( s ) ; break ;
    }
 
    if ( strcmp ( c, "=" ) == 0 )
    {
      genVariable ( s, FALSE ) ;
      genExpression () ;
      genAssignment () ;
      genPop        () ;
      getToken ( c ) ;
    }
  }
 
  if ( c[0] != ';' )
    return error ( "Missing ';' after declaration of '%s'", s ) ;

  return TRUE ;
}



int pslCompiler::genGlobalDeclaration ()
{
  char c  [ MAX_TOKEN ] ;
  char fn [ MAX_TOKEN ] ;

  getToken ( c ) ;

  if ( strcmp ( c, "static" ) == 0 ||
       strcmp ( c, "extern" ) == 0 )
  {
    /* Something complicated should probably happen here! */
    getToken ( c ) ;
  }

  pslType t ;

  if ( strcmp ( c, "void"   ) == 0 ) t = PSL_VOID   ; else
    if ( strcmp ( c, "int"    ) == 0 ) t = PSL_INT    ; else
      if ( strcmp ( c, "float"  ) == 0 ) t = PSL_FLOAT  ; else
        if ( strcmp ( c, "string" ) == 0 ) t = PSL_STRING ; else
          return error ( "Expected declaration - but got '%s'", c ) ;

  getToken ( fn ) ;

  getToken ( c ) ;

  if ( c[0] == '(' )
  {
    ungetToken ( c ) ;
    return genFunctionDeclaration ( fn ) ;
  }

  if ( c[0] == '[' || strcmp ( c, "=" ) == 0 || c[0] == ';' )
  {
    ungetToken ( c ) ;
    return genGlobalVarDecl ( fn, t ) ;
  }

  return error ( "Expected a declaration - but got '%s'", c);
}


int pslCompiler::genFunctionDeclaration ( const char *fn )
{
  char c  [ MAX_TOKEN ] ;

  pslAddress jump_target = genJump ( 0 ) ;

  setCodeSymbol ( fn, next_code ) ;

  getToken ( c ) ;

  if ( c[0] != '(' )
    return error ( "Missing '(' in declaration of '%s'", fn ) ;

  pushLocality () ;

  int argpos = 0 ;

  while ( 1 )
  {
    getToken ( c ) ;

    if ( c [ 0 ] == ')' || c [ 0 ] == '\0' )
      break ;

    char s [ MAX_TOKEN ] ;

    getToken ( s ) ;

    pslAddress a = setVarSymbol ( s ) ;

    if ( strcmp ( c, "int"    ) == 0 ) genMakeIntVariable    ( s ) ; else
    if ( strcmp ( c, "float"  ) == 0 ) genMakeFloatVariable  ( s ) ; else
    if ( strcmp ( c, "string" ) == 0 ) genMakeStringVariable ( s ) ; else
    {
      popLocality () ;
      return error ( "Missing ')' in declaration of '%s'", fn ) ;
    }
 
    genGetParameter ( a, argpos++ ) ;

    getToken ( c ) ;

    if ( c[0] == ',' )
      continue ;

    if ( c[0] == ')' )
      break ;

    popLocality () ;
    return error ( "Missing ',' or ')' in declaration of '%s'", fn ) ;
  }

  if ( c[0] != ')' )
  {
    popLocality () ;
    return error ( "Missing ')' in declaration of '%s'", fn ) ;
  }

  getToken ( c ) ;

  if ( c [ 0 ] != '{' )
  {
    popLocality () ;
    return error ( "Missing '{' in function '%s'", fn ) ;
  }

  if ( ! genCompoundStatement () )
  {
    popLocality () ;
    return error ( "Missing '}' in function '%s'", fn ) ;
  }

  getToken ( c ) ;

  /* If we fall off the end of the function, we still need a return value */

  genConstant ( "0.0" ) ;
  genReturn   () ;

  code [  jump_target  ] =  next_code       & 0xFF ;
  code [ jump_target+1 ] = (next_code >> 8) & 0xFF ;

  popLocality () ;
  return TRUE ;
}




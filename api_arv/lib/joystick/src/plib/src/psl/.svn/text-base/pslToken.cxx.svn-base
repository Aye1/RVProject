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

     $Id: pslToken.cxx,v 1.21 2002/09/15 17:45:53 sjbaker Exp $
*/


#include "pslLocal.h"

static char ungotten_token [ MAX_UNGET ][ MAX_TOKEN ] ;
static int  unget_token_stack_depth = 0 ;

void pslCompiler::skipToEOL ()
{
  int c ;

  do
  {
    c = getChar () ;
  } while ( c != '\n' && c != -1 ) ;
}



int pslCompiler::getChar ()
{
  int c ;

  /* Skip characters so long as the 'skipping' flag is set */

  do
  {
    /*
      If we got a newline then we have to test to see whether
      a '#' preprocessor directive is on this line.
    */
 
    c = _pslGetChar () ;

    if ( c == '\n' )
    {
      int d = _pslGetChar () ;
 
      if ( d == '#' )
      {
        skipOverride = TRUE ;
        c = doPreProcessorCommand () ;
        skipOverride = FALSE ;

        if ( ! skipping () )
          return c ;
      }
      else      
        _pslUnGetChar ( d ) ;
    }
  } while ( skipping () && c != -1 ) ;

  /*
    All done - return the character.
  */
 
  return c ;
}



int pslCompiler::searchDefines ( const char *token ) const
{
  if ( ( token[0] < 'a' || token[0] > 'z' ) &&
       ( token[0] < 'A' || token[0] > 'Z' ) &&
         token[0] != '_' )
    return -1 ;

  for ( int i = 0 ; i < next_define ; i++ )
    if ( define_token [ i ] != NULL &&
         strcmp ( token, define_token [ i ] ) == 0 )
      return i ;

  return -1 ;
}


void pslCompiler::doUndefStatement  ()
{
  char token [ MAX_TOKEN ] ;

  /*
    It's tempting to just getToken to get the undefined token
    but that doesn't work because it get's define-processed.
  */

  getToken ( token, FALSE ) ;

  if ( ( token[0] < 'a' || token[0] > 'z' ) &&
       ( token[0] < 'A' || token[0] > 'Z' ) &&
         token[0] != '_' )
  {
    error ( "#undef token is not a legal identifier" ) ;
    skipToEOL () ;
    return ;
  } 

  skipToEOL () ;

  int def = searchDefines ( token ) ;

  if ( def == -1 ) return ;  /* Not an error to have undefined undef's */
 
  if ( ! skipping () )
  {
    delete define_token       [ def ] ;
    delete define_replacement [ def ] ;
    define_token       [ def ] = NULL ;
    define_replacement [ def ] = NULL ;
  }
}


void pslCompiler::doIfdefStatement  ()
{
  char token [ MAX_TOKEN ] ;
  
  getToken ( token, FALSE ) ;

  if ( next_skippingLevel >= 30 )
  {
    error ( "Too many levels of #ifdef/#ifndef processing." ) ;
    skipToEOL () ;
    return ;
  }

  if ( searchDefines ( token ) == -1 )
    skippingFlag |= ( 1 << next_skippingLevel++ ) ;
  else
    skippingFlag &= ~( 1 << next_skippingLevel++ ) ;
}


void pslCompiler::doIfndefStatement ()
{
  char token [ MAX_TOKEN ] ;
  
  getToken ( token, FALSE ) ;

  if ( next_skippingLevel >= 30 )
  {
    error ( "Too many levels of #ifdef/#ifndef processing." ) ;
    skipToEOL () ;
    return ;
  }

  if ( searchDefines ( token ) == -1 )
    skippingFlag &= ~( 1 << next_skippingLevel++ ) ;
  else
    skippingFlag |= ( 1 << next_skippingLevel++ ) ;

  skipToEOL () ;
}


void pslCompiler::doElseStatement ()
{
  if ( next_skippingLevel <= 1 )
    error ( "#else without prior matching #ifdef/#ifndef." ) ;

  skipToEOL () ;
  skippingFlag ^= ( 1 << (next_skippingLevel-1) ) ;
}



void pslCompiler::doEndifStatement ()
{
  if ( next_skippingLevel <= 1 )
    error ( "#endif without prior matching #ifdef/#ifndef." ) ;

  skipToEOL () ;
  skippingFlag &= ~( 1 << (--next_skippingLevel) ) ;
}


void pslCompiler::doDefineStatement ()
{
  char token [ MAX_TOKEN ] ;
  char subst [ 1024 ] ;
  char *p ;

  getToken ( token, FALSE ) ;

  if ( ( token[0] < 'a' || token[0] > 'z' ) &&
       ( token[0] < 'A' || token[0] > 'Z' ) &&
         token[0] != '_' )
  {
    error ( "#define token is not a legal identifier" ) ;
    skipToEOL () ;
    return ;
  } 

  int c ;

  p = subst ;

  do
  {
    c = getChar () ;

    *(p++) = c ;
  } while ( c != -1 && c != '\n' ) ;

  *(p-1) = '\0' ;

  if ( ! skipping () )
  {
    if ( searchDefines ( token ) != -1 )
      error ( "Attempt to re-#define %s", token ) ;
    else
    if ( next_define >= MAX_SYMBOL - 1 )
      error ( "Too many #define's\n" ) ;
    else
    {
      define_token       [ next_define ] = ulStrDup ( token ) ;
      define_replacement [ next_define ] = ulStrDup ( subst ) ;
      next_define++ ;
    }
  }
}

void pslCompiler::doIncludeStatement ()
{
  char token [ MAX_TOKEN ] ;
  char *p ;

  getToken ( token ) ;

  if ( token[0] == '"' )
    p = token + 1 ;
  else
  if ( token[0] == '<' )
  {
    p = token ;

    do
    {
      *p = getChar () ;

    } while ( *(p++) != '>' ) ;

    *(p-1) = '\0' ;

    p = token ;
  }
  else
  {
    error ( "Illegal character after '#include'" ) ;
    skipToEOL () ;
    return ;
  }

  /*
    Skip to the end of this line of text BEFORE we hand
    control over to the next file.
  */

  skipToEOL () ;

  if ( ! skipping () )
    _pslPushDefaultFile ( p ) ;
}



int pslCompiler::doPreProcessorCommand ()
{
  char token [ MAX_TOKEN ] ;

  getToken ( token ) ;

  if ( strcmp ( token, "include" ) == 0 )
  {
    doIncludeStatement () ;
    return getChar () ;
  }

  if ( strcmp ( token, "undef"  ) == 0 )
  {
    doUndefStatement () ;
    return '\n' ;
  }

  if ( strcmp ( token, "define"  ) == 0 )
  {
    doDefineStatement () ;
    return '\n' ;
  }

  if ( strcmp ( token, "ifndef"   ) == 0 )
  {
    doIfndefStatement () ;
    return getChar () ;
  }

  if ( strcmp ( token, "ifdef"   ) == 0 )
  {
    doIfdefStatement () ;
    return getChar () ;
  }

  if ( strcmp ( token, "endif"   ) == 0 )
  {
    doEndifStatement () ;
    return getChar () ;
  }

  if ( strcmp ( token, "else"    ) == 0 )
  {
    doElseStatement () ;
    return getChar () ;
  }

  error ( "Unrecognised preprocessor directive '%s'", token ) ;

  skipToEOL () ;
  return '\n' ;
}


void pslCompiler::getToken ( char *res, int define_sub )
{
  /* WARNING -- RECURSIVE -- WARNING -- RECURSIVE -- WARNING -- RECURSIVE */

  if ( unget_token_stack_depth > 0 )
  {
    strcpy ( res, ungotten_token [ --unget_token_stack_depth ] ) ;
    return ;
  }

  int c ;

  do
  {
    c = getChar () ;

    if ( c < 0 )
    {
      res [ 0 ] = '\0' ;
      return ;
    }

    if ( c == '/' )
    {
      int d = getChar () ;

      if ( d == '/' ) /* C++ style comment */
      {
        do
        {
          d = getChar () ;
        } while ( d != '\n' && d != -1 ) ;

        c = ' ' ;
      }
      else
      if ( d == '*' ) /* C style comment */
      {
        /*
          YUK! This is *so* horrible to get right.
          Just think about this case! ==> **/

        do
        {
          /* Search for a star or an EOF */

          do
          {
            d = getChar () ;
          } while ( d != '*' && d != -1 ) ;

          c = getChar () ;

          /* If you get two stars in a row - unget the second one */

          if ( c == '*' )
            _pslUnGetChar ( '*' ) ;

        } while ( c != '/' ) ;

        c = ' ' ;
      }
      else
        _pslUnGetChar ( d ) ;
    }
  } while ( isspace ( c ) ) ;

  int tp = 0 ;

  if ( c == '"' )
  {
    int isBkSlash = FALSE ;

    do
    {
      if ( c == '\\' )
      {
        if ( isBkSlash )
        {
          isBkSlash = FALSE ;
          res [ tp++ ] = c ;
        }
        else
          isBkSlash = TRUE ;
      }
      else    
      if ( isBkSlash )
      {
        switch ( c )
        {
          case '0' : res [ tp++ ] = '\0' ; break ;
          case 'r' : res [ tp++ ] = '\r' ; break ;
          case 't' : res [ tp++ ] = '\t' ; break ;
          case 'n' : res [ tp++ ] = '\n' ; break ;
          case 'f' : res [ tp++ ] = '\f' ; break ;
          case 'b' : res [ tp++ ] = '\b' ; break ;
          case 'a' : res [ tp++ ] = '\a' ; break ;
          default  : res [ tp++ ] =   c  ; break ;
        }

        isBkSlash = FALSE ;
      }
      else
        res [ tp++ ] = c ;

      c = getChar () ;

      if ( tp >= MAX_TOKEN - 1 )
      {
        error ( "Input string is bigger than %d characters!",
                                                       MAX_TOKEN - 1 ) ;
        tp-- ;
      }
    } while ( ( isBkSlash || c != '"' ) && c != -1 ) ;

    if ( c == -1 )
      error ( "Missing \\\" character" ) ;
   
    /* The trailing quotes character is not included into the string */
    res [ tp ] = '\0' ;
    return ;
  }

  if ( c == '\'' )
  {
    res [ tp++ ] = '\'' ;

    c = getChar () ;

    if ( c == '\\' )
    {
      c = getChar () ;

      switch ( c )
      {
	case '0' : res [ tp++ ] = '\0' ; break ;
	case 'r' : res [ tp++ ] = '\r' ; break ;
	case 't' : res [ tp++ ] = '\t' ; break ;
	case 'n' : res [ tp++ ] = '\n' ; break ;
	case 'f' : res [ tp++ ] = '\f' ; break ;
	case 'b' : res [ tp++ ] = '\b' ; break ;
	case 'a' : res [ tp++ ] = '\a' ; break ;
	default  : res [ tp++ ] =   c  ; break ;
      }
    }
    else
      res [ tp++ ] = c ;

    c = getChar () ;

    if ( c != '\'' )
      error ( "Missing \\' character" ) ;
   
    /* The trailing quote character is not included into the string */
    res [ tp ] = '\0' ;
    return ;
  }

  if ( isalnum ( c ) || c == '.' || c == '_' )  /* variables and numbers */
  {
    while ( isalnum ( c ) || c == '.' || c == '_' )
    {
      res [ tp++ ] = c ;
      c = getChar () ;

      if ( tp >= MAX_TOKEN - 1 )
      {
        error ( "Input string is bigger than %d characters!",
                                                     MAX_TOKEN - 1 ) ;
        tp-- ;
      }
    }

    _pslUnGetChar ( c ) ;
  }
  else

  /* Deal with:
       <<, >>, <<=, >>=,
       /=, +=, -=, *=, %=, &=, |=,
       <=, >=, ==, !=, ++, --
  */

  if ( c == '*' || c == '/' || c == '%' ||
       c == '<' || c == '>' || c == '=' ||
       c == '!' || c == '&' || c == '|' ||
       c == '+' || c == '-' )
  {
    res [ tp++ ] = c ;

    int c2 = getChar () ;

    if ( c2 == '=' || ( c == '>' && c2 == '>' ) ||
                      ( c == '<' && c2 == '<' ) ||
                      ( c == '&' && c2 == '&' ) ||
                      ( c == '|' && c2 == '|' ) ||
                      ( c == '+' && c2 == '+' ) ||
                      ( c == '-' && c2 == '-' ) )
    {
      res [ tp++ ] = c2 ;

      if ( ( c == '<' && c2 == '<' ) ||
           ( c == '>' && c2 == '>' ) )
      {
        int c3 = getChar () ;

        if ( c3 == '=' )
          res [ tp++ ] = c3 ;
        else
          _pslUnGetChar ( c3 ) ;
      }
    }
    else
      _pslUnGetChar ( c2 ) ;
  }
  else
  {
    res [ 0 ] = c ;
    tp = 1 ;
  }

  res [ tp ] = '\0' ;

  /*
    Don't do define substituting if told not to
  */

  if ( ! define_sub )
    return ;

  /* Do #define expansion.  */

  int def = searchDefines ( res ) ;

  if ( def == -1 )
    return ;

  /*
    If there is a replacement for this token, un-get it so it's the
    next thing we'll read.
  */

  for ( int i = strlen ( define_replacement [ def ] ) - 1 ; i >= 0 ; i-- )
    _pslUnGetChar ( define_replacement [ def ][ i ] ) ;

  /*
    Then have another try at reading the token.
  */

  getToken ( res ) ;
}


void pslCompiler::ungetToken ( const char *s )
{
  if ( unget_token_stack_depth >= MAX_UNGET-1 )
  {
    error ( "Too many ungetTokens! This must be an *UGLY* PSL program!" ) ;
    exit ( -1 ) ;
  }

  strcpy ( ungotten_token[unget_token_stack_depth++], s ) ;
}



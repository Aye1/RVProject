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

     $Id: pslDump.cxx,v 1.22 2002/09/22 18:25:16 sjbaker Exp $
*/


#include "pslLocal.h"


struct OpcodeDecode
{
  const char *s ;
  unsigned char opcode ;
  int numArgBytes ;
} ;



static const OpcodeDecode opcodeDecode [] =
{
  { "**BAD_INSTRUCTION**", OPCODE_BAD        , 0 },

  { "SOURCE LINE NUMBER:", OPCODE_LINE_NUMBER, 2 },

  /* Stack operations */

  { "PUSH_INT_CONSTANT"  , OPCODE_PUSH_INT_CONSTANT  , sizeof(int)   },
  { "PUSH_FLOAT_CONSTANT", OPCODE_PUSH_FLOAT_CONSTANT, sizeof(float) },
  { "PUSH_STRING_CONSTANT",OPCODE_PUSH_STRING_CONSTANT, 0 },
  { "PUSH_VARIABLE"      , OPCODE_PUSH_VARIABLE       , 1 },

  { "STACK_DUPLICATE"    , OPCODE_STACK_DUPLICATE    , 0 },

  { "POP_VARIABLE"       , OPCODE_POP_VARIABLE       , 0 },
  { "POP"                , OPCODE_POP                , 0 },

  { "FETCH"              , OPCODE_FETCH              , 0 },
  { "INCREMENT_FETCH"    , OPCODE_INCREMENT_FETCH    , 0 },
  { "DECREMENT_FETCH"    , OPCODE_DECREMENT_FETCH    , 0 },
  { "INCREMENT_LVALUE"   , OPCODE_INCREMENT_LVALUE   , 0 },
  { "DECREMENT_LVALUE"   , OPCODE_DECREMENT_LVALUE   , 0 },

  /* Variable creation */

  { "SET_INT_ARRAY"      , OPCODE_SET_INT_ARRAY      , 1 },
  { "SET_FLOAT_ARRAY"    , OPCODE_SET_FLOAT_ARRAY    , 1 },
  { "SET_STRING_ARRAY"   , OPCODE_SET_STRING_ARRAY   , 1 },
  { "SET_INT_VARIABLE"   , OPCODE_SET_INT_VARIABLE   , 1 },
  { "SET_FLOAT_VARIABLE" , OPCODE_SET_FLOAT_VARIABLE , 1 },
  { "SET_STRING_VARIABLE", OPCODE_SET_STRING_VARIABLE, 1 },
  { "GET_PARAMETER"      , OPCODE_GET_PARAMETER      , 2 },

  /* Flow Control */

  { "CALLEXT",         OPCODE_CALLEXT     , 2 },
  { "CALL",            OPCODE_CALL        , 3 },
  { "RETURN",          OPCODE_RETURN      , 0 },
  { "PEEK_JUMP_FALSE", OPCODE_PEEK_JUMP_FALSE, 2 },
  { "PEEK_JUMP_TRUE",  OPCODE_PEEK_JUMP_TRUE , 2 },
  { "JUMP_FALSE",      OPCODE_JUMP_FALSE  , 2 },
  { "JUMP_TRUE",       OPCODE_JUMP_TRUE   , 2 },
  { "JUMP",            OPCODE_JUMP        , 2 },
  { "PAUSE",           OPCODE_PAUSE       , 0 },
  { "HALT",            OPCODE_HALT        , 0 },

  /* Math operators */

  { "SUB",             OPCODE_SUB         , 0 },
  { "ADD",             OPCODE_ADD         , 0 },
  { "DIV",             OPCODE_DIV         , 0 },
  { "MULT",            OPCODE_MULT        , 0 },
  { "MOD",             OPCODE_MOD         , 0 },
  { "NEG",             OPCODE_NEG         , 0 },
  { "EXCHG",           OPCODE_EXCHANGE    , 0 },

  /* Bitwise operators */

  { "NOT",             OPCODE_NOT         , 0 },
  { "TWIDDLE",         OPCODE_TWIDDLE     , 0 },

  { "OROR",            OPCODE_OROR        , 0 },
  { "ANDAND",          OPCODE_ANDAND      , 0 },

  { "OR",              OPCODE_OR          , 0 },
  { "AND",             OPCODE_AND         , 0 },
  { "XOR",             OPCODE_XOR         , 0 },

  { "SHIFT_LEFT",      OPCODE_SHIFTLEFT   , 0 },
  { "SHIFT_RIGHT",     OPCODE_SHIFTRIGHT  , 0 },

  /* Boolean operators */

  { "LESS",            OPCODE_LESS        , 0 },
  { "LESSEQUAL",       OPCODE_LESSEQUAL   , 0 },
  { "GREATER",         OPCODE_GREATER     , 0 },
  { "GREATEREQUAL",    OPCODE_GREATEREQUAL, 0 },
  { "NOTEQUAL",        OPCODE_NOTEQUAL    , 0 },
  { "EQUAL",           OPCODE_EQUAL       , 0 },

  { "POP_ADD_VARIABLE", OPCODE_POP_ADD_VARIABLE, 0 },
  { "POP_SUB_VARIABLE", OPCODE_POP_SUB_VARIABLE, 0 },
  { "POP_MUL_VARIABLE", OPCODE_POP_MUL_VARIABLE, 0 },
  { "POP_MOD_VARIABLE", OPCODE_POP_MOD_VARIABLE, 0 },
  { "POP_DIV_VARIABLE", OPCODE_POP_DIV_VARIABLE, 0 },
  { "POP_AND_VARIABLE", OPCODE_POP_AND_VARIABLE, 0 },
  { "POP_OR_VARIABLE" , OPCODE_POP_OR_VARIABLE , 0 },
  { "POP_XOR_VARIABLE", OPCODE_POP_XOR_VARIABLE, 0 },
  { "POP_SHL_VARIABLE", OPCODE_POP_SHL_VARIABLE, 0 },
  { "POP_SHR_VARIABLE", OPCODE_POP_SHR_VARIABLE, 0 },

  { NULL, 0, 0 }
} ;


int pslCompiler::printOpcode ( FILE *fd, int addr ) const
{
  fprintf ( fd, "%3d: 0x%02x ", addr, (unsigned int)( code [ addr ] ) ) ;

  unsigned char op = code [ addr ] ;

  for ( int i = 0 ; opcodeDecode [ i ] . s != NULL ; i++ )
    if ( opcodeDecode [ i ] . opcode == op )
    {
      for ( int j = 0 ; j < 4 ; j++ )
        if ( j < opcodeDecode [ i ] . numArgBytes )
          fprintf ( fd, "0x%02x ", code [ addr + j + 1 ] ) ;
        else
          fprintf ( fd, "     " ) ;

      fprintf ( fd, " %s", opcodeDecode [ i ] . s ) ;
      return 1 + opcodeDecode [ i ] . numArgBytes ;
    }

  fprintf ( fd, "**** UNRECOGNISED OPCODE **** " ) ;
  return 1 ;
}


void pslCompiler::dump () const
{
  int i ;

  printf ( "\n" ) ;
  printf ( "Bytecode:\n" ) ;

  for ( i = 0 ; i < next_code ; )
    i += printInstruction ( stdout, i ) ;

  printf ( "\n" ) ;
  printf ( "Global Variables:\n" ) ;

  for ( i = 0 ; i < MAX_SYMBOL ; i++ )
    if ( symtab [ i ] . symbol != NULL )
    {
      printf ( "\t%5s => %4d", symtab[i].symbol,
                               symtab[i].address ) ;

      if ( i & 1 )
        printf ( "\n" ) ;
      else
        printf ( "  " ) ;
    }

  printf ( "\n" ) ;
  printf ( "Functions:\n" ) ;

  for ( i = 0 ; i < MAX_SYMBOL ; i++ )
    if ( code_symtab [ i ] . symbol != NULL )
    {
      printf ( "\t%5s => %4d", code_symtab[i].symbol,
                               code_symtab[i].address ) ;

      if ( i & 1 )
        printf ( "\n" ) ;
      else
        printf ( "  " ) ;
    }

  printf ( "\n" ) ;

  if ( num_errors > 0 )
    printf ( "PROGRAM FAILED TO COMPILE WITH %d WARNINGS AND %d ERRORS\n", 
                                 num_warnings, num_errors ) ;
  else
  if ( num_warnings > 0 )
    printf ( "PROGRAM COMPILED WITH %d WARNINGS\n", num_warnings ) ;
  else
    printf ( "PROGRAM COMPILED OK\n" ) ;

  printf ( "\n" ) ;
}


int pslCompiler::printInstruction ( FILE *fd, int addr ) const
{
  int skip = printOpcode ( fd, addr ) ;

  switch ( code [ addr ] )
  {
    case OPCODE_LINE_NUMBER :
      fprintf ( fd, "%d =====================",
                              code[addr+1] + ( code[addr+2] << 8 ) ) ;
      break ;

    case OPCODE_SET_INT_ARRAY         :
    case OPCODE_SET_FLOAT_ARRAY       :
    case OPCODE_SET_STRING_ARRAY      :
    case OPCODE_SET_INT_VARIABLE      :
    case OPCODE_SET_FLOAT_VARIABLE    :
    case OPCODE_SET_STRING_VARIABLE   :
    case OPCODE_PUSH_VARIABLE         :
      fprintf ( fd, "\t\t[%d]", code [ addr+1 ] ) ;
      break ;

    case OPCODE_GET_PARAMETER :
      fprintf ( fd, "\t\t[%d],off=%d", code [ addr+1 ], code [ addr+2 ] ) ;
      break ;

    case OPCODE_PUSH_INT_CONSTANT :
      {
	int f ;
	memcpy ( &f, &code[addr+1], sizeof(int) ) ;
	fprintf ( fd, "\t%d", f ) ;
      }
      break ;

    case OPCODE_PUSH_FLOAT_CONSTANT :
      {
	float f ;
	memcpy ( &f, &code[addr+1], sizeof(float) ) ;
	fprintf ( fd, "\t%f", f ) ;
      }
      break ;

    case OPCODE_PUSH_STRING_CONSTANT :
      {
	fprintf ( fd, "\t\"%s\"",  &code[addr+1] ) ;
        skip += strlen ( (char *) &code[addr+1] ) + 1 ;
      }
      break ;

    case OPCODE_CALLEXT :
      fprintf ( fd, "\t\t\t%s,nargs=%d",
                             extensions[code[addr+1]].symbol, code[addr+2] );
      break ;

    case OPCODE_CALL :
      fprintf ( fd, "\t\t\t%d,nargs=%d", code[addr+1] + ( code[addr+2] << 8 ),
				  code[addr+3] ) ;
      break ;

    case OPCODE_PEEK_JUMP_FALSE :
    case OPCODE_PEEK_JUMP_TRUE  :
    case OPCODE_JUMP_FALSE      :
    case OPCODE_JUMP_TRUE       :
      fprintf ( fd, "\t\t%d", code[addr+1] + ( code[addr+2] << 8 ) ) ;
      break ;

    case OPCODE_JUMP :
      fprintf ( fd, "\t\t\t%d", code[addr+1] + ( code[addr+2] << 8 ) ) ;
      break ;

    default : break ;
  }

  fprintf ( fd, "\n" ) ;
  return skip ;
}



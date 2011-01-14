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

     $Id: pslCompiler.h,v 1.33 2003/01/06 05:10:13 sjbaker Exp $
*/


struct pslFwdRef
{
  char       *symbol ;
  pslAddress  where ;

  void set ( const char *s, pslAddress w )
  {
    symbol = ulStrDup ( s ) ;
    where = w ;
  }

  pslAddress getWhere () const { return where ; }

  int matches ( const char *s ) const
  {
    return symbol != NULL && strcmp ( s, symbol ) == 0 ;
  }

   pslFwdRef () { symbol = NULL ; }
  ~pslFwdRef () { delete [] symbol ; }
} ;



class pslProgram ;


class pslCompiler
{
  /* File I/O and preprocessor */

  int  getChar               () ;

  void doIncludeStatement    () ;
  void doDefineStatement     () ;
  void doIfdefStatement      () ;
  void doUndefStatement      () ;
  void doIfndefStatement     () ;
  void doElseStatement       () ;
  void doEndifStatement      () ;

  int  doPreProcessorCommand () ;
  void getToken              ( char *s, int define_sub = TRUE ) ;
  void ungetToken            ( const char *s ) ;
  void skipToEOL             () ;


  /* Write data into Code space */

  void genCodeByte ( unsigned char b ) ;
  void genCodeAddr ( pslAddress a ) ;
  int  printOpcode  ( FILE *fd, int addr ) const ;

  void genLineNumber ( int l ) ;

  /* Write single byte-coded instructions into code space.  */

  void genStackDup     () ;
  void genPop          () ;
  void genSubtract     () ;
  void genAdd          () ;
  void genDivide       () ;
  void genMultiply     () ;
  void genModulo       () ;
  void genNegate       () ;
  void genNot          () ;
  void genTwiddle      () ;
  void genOrOr         () ;
  void genAndAnd       () ;
  void genOr           () ;
  void genAnd          () ;
  void genXor          () ;
  void genShiftLeft    () ;
  void genShiftRight   () ;
  void genLess         () ;
  void genLessEqual    () ;
  void genGreater      () ;
  void genGreaterEqual () ;
  void genNotEqual     () ;
  void genEqual        () ;
  void genExchange     () ;

  int  genPeekJumpIfFalse  ( int l ) ;
  int  genPeekJumpIfTrue   ( int l ) ;
  int  genJumpIfFalse      ( int l ) ;
  int  genJumpIfTrue       ( int l ) ;
  int  genJump             ( int l ) ;

  void genGetParameter     ( pslAddress var, int argpos ) ;

  int  genMakeIntVariable    ( const char *s ) ;
  int  genMakeFloatVariable  ( const char *s ) ;
  int  genMakeStringVariable ( const char *s ) ;

  int  genMakeIntArray     ( const char *s ) ;
  int  genMakeFloatArray   ( const char *s ) ;
  int  genMakeStringArray  ( const char *s ) ;

  void genConstant         ( const char *s ) ;
  void genIntConstant      ( const char *s ) ;
  void genFloatConstant    ( const char *s ) ;
  void genStringConstant   ( const char *s ) ;
  void genCharConstant     ( char c ) ;
  void genIntConstant      ( int i ) ;

  void genVoidConstant     () ;

  void genFetch            () ;

  void genIncrementLValue  () ;
  void genDecrementLValue  () ;
  void genIncrementFetch   () ;
  void genDecrementFetch   () ;

  void genVariable         ( const char *s, int dimension ) ;
  void genAssignment       () ;
  void genAddAssignment    () ;
  void genSubAssignment    () ;
  void genMulAssignment    () ;
  void genModAssignment    () ;
  void genDivAssignment    () ;
  void genAndAssignment    () ;
  void genOrAssignment     () ;
  void genXorAssignment    () ;
  void genSHLAssignment    () ;
  void genSHRAssignment    () ;

  void genCall             ( const char *s, int argc ) ;
  void genReturn           () ;

  /* Expression parsers & code generators.  */

  int genLValue            () ;
  int genPrimitive         () ;
  int genBitwiseExpression () ;
  int genMultExpression    () ;
  int genAddExpression     () ;
  int genShiftExpression   () ;
  int genRelExpression     () ;
  int genBoolExpression    () ;
  int genExpression        () ;

  /* Statement-level parsers & code generators. */

  int  genBreakStatement   () ;
  int  genContinueStatement() ;
  int  genReturnStatement  () ;
  int  genPauseStatement   () ;
  int  genSwitchStatement  () ;
  int  genWhileStatement   () ;
  int  genDoWhileStatement () ;
  int  genForStatement     () ;
  int  genIfStatement      () ;
  int  genFunctionCall     ( const char *s ) ;
  int  genCompoundStatement() ;
  int  genStatement        () ;

  int  genLocalVarDecl     ( pslType t ) ;
  int  genGlobalVarDecl    ( const char *fn, pslType t ) ;
  int  genStaticVarDecl    () ;

  /* Top level constructs */

  int  genFunctionDeclaration ( const char *fn ) ;
  int  genGlobalDeclaration   () ;
  void genProgram             () ;

  /* The symbol tables for variables, code and define's */

  int next_label ;
  int next_code_symbol ;
  int next_define ;

  int generate_line_numbers ;

  char *define_token       [ MAX_SYMBOL ] ;
  char *define_replacement [ MAX_SYMBOL ] ;

  int searchDefines ( const char *s ) const ;

  int skipOverride ;
  int skippingFlag  ;
  int next_skippingLevel ;

  int skipping () const
  {
    return ! skipOverride && skippingFlag != 0 ;
  }

  pslSymbol         symtab [ MAX_SYMBOL ] ;
  pslSymbol    code_symtab [ MAX_SYMBOL ] ;

  pslAddress getVarSymbol       ( const char *s ) ;
  pslAddress setVarSymbol       ( const char *s ) ;

  pslAddress getCodeSymbol      ( const char *s, pslAddress fixupLoc ) ;
  void       setCodeSymbol      ( const char *s, pslAddress v ) ;

  int        getExtensionSymbol ( const char *s ) const ;

  const pslExtension *extensions ;

  /* Forward references to code symbols that are not yet defined */

  int next_fwdref ;
  pslFwdRef    forward_ref [ MAX_SYMBOL ] ;

  int locality_stack [ MAX_NESTING ] ;
  int locality_sp ;

  void fixup                  ( const char *s, pslAddress v ) ;
  void addFwdRef              ( const char *s, pslAddress where ) ;
  void checkUnresolvedSymbols () ;

  int  cleanup () ;  /* Report errors, etc after a compile */

  void pushLocality ()
  {
    if ( locality_sp >= MAX_NESTING-1 )
      error ( "Too many nested {}'s" ) ;
    else
      locality_stack [ locality_sp++ ] = next_var ;
  }

  void popLocality  ()
  {
    if ( locality_sp <= 0 )
      error ( "Locality stack underflow !!" ) ;

    /* Delete any local symbols */

    for ( int i = locality_stack [ locality_sp-1 ] ;
              i < next_var ; i++ )
    {
      delete [] symtab [ i ] . symbol ;
      symtab [ i ] . symbol = NULL ;
    }

    /* Put the next_var pointer back where it belongs */

    next_var = locality_stack [ --locality_sp ] ;
  }

  /* Ikky stuff to remember where break and continue should jump */

  int breakToAddressStack    [ MAX_LABEL ] ;
  int continueToAddressStack [ MAX_LABEL ] ;
  int next_break     ;
  int next_tmp_label ;
  int next_continue  ;

  void pushBreakToLabel    () ;
  int  pushContinueToLabel () ;
  void  setContinueToLabel ( int which ) ;
  void pushNoContinue      () ;

  void popBreakToLabel     () ;
  void popContinueToLabel  () ;

  /* Error and warning handlers */

  int num_errors   ;
  int num_warnings ;

  void bumpErrors   () { num_errors++   ; }
  void bumpWarnings () { num_warnings++ ; }

  int error   ( const char *fmt, ... ) ;
  int warning ( const char *fmt, ... ) ;

  /* Remember the name of the program for debug purposes */

  char *progName ;

  char *getProgName () const { return progName ; }

  /* Major storage for symbols and byte-codes */

  int next_var   ;
  int next_code  ;
  pslOpcode    *code       ;
  pslContext   *context    ;
  pslProgram   *program    ;

public:

  pslCompiler ( pslProgram   *prog,
                pslOpcode    *_code,
                const pslExtension *_extn,
                const char   *_progName )
  {
    program  = prog ;
    progName = ulStrDup ( _progName ) ;

    generate_line_numbers = FALSE ;
    code       = _code ;
    extensions = _extn ;

    for ( int i = 0 ; i < MAX_SYMBOL ; i++ )
    {
      define_token       [ i ] = NULL ;
      define_replacement [ i ] = NULL ;
      symtab      [ i ] . symbol = NULL ;
      forward_ref [ i ] . symbol = NULL ;
      code_symtab [ i ] . symbol = NULL ;
    }

    init () ;
  }

  ~pslCompiler ()
  {
    for ( int i = 0 ; i < MAX_SYMBOL ; i++ )
    {
      delete [] define_token       [ i ] ;
      delete [] define_replacement [ i ] ;
      delete [] symtab      [ i ] . symbol ; symtab      [ i ] . symbol = NULL ;
      delete [] code_symtab [ i ] . symbol ; code_symtab [ i ] . symbol = NULL ;
      delete [] forward_ref [ i ] . symbol ; forward_ref [ i ] . symbol = NULL ;
    }
  }

  const pslExtension *getExtensions () const { return extensions ; }

  int printInstruction ( FILE *fd, int addr ) const ;


  void generateLineNumbers ()
  {
    generate_line_numbers = TRUE ;
  }


  void init () 
  {
    int i ;

    for ( i = 0 ; i < MAX_CODE   ; i++ ) code   [ i ] = OPCODE_HALT ; 

    for ( i = 0 ; i < MAX_SYMBOL ; i++ )
    {
      delete [] define_token       [ i ] ; define_token       [ i ] = NULL ;
      delete [] define_replacement [ i ] ; define_replacement [ i ] = NULL ;
      delete [] symtab      [ i ] . symbol ; symtab      [ i ] . symbol = NULL ;
      delete [] code_symtab [ i ] . symbol ; code_symtab [ i ] . symbol = NULL ;
      delete [] forward_ref [ i ] . symbol ; forward_ref [ i ] . symbol = NULL ;
    }

    num_errors = num_warnings = 0 ;

    locality_sp = 0 ;

    next_define   = 0 ;
    next_continue = 0 ;
    next_break    = 0 ;
    next_tmp_label= 0 ;
    next_fwdref   = 0 ;
    next_label    = 0 ;
    next_code_symbol = 0 ;
    next_code     = 0 ;
    next_var      = 0 ;
    skippingFlag  = 0 ;
    next_skippingLevel = 1 ;
  }

  void dump () const ;
  int  compile ( const char *fname ) ;
  int  compile ( FILE *fd, const char *fname = NULL ) ;
  int  compile ( const char *memptr, const char *prog_name ) ;
} ;




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

     $Id: ssgSaveFLT.cxx,v 1.4 2003/01/04 02:55:55 sjbaker Exp $
*/

/*******************************************************
 **  ssgSaveFLT.cxx
 **
 **  Written by Steve Baker
 **  Last updated: 2002-10-15
 **
 **  This was written to be a part of Stephen J Bakers
 **  PLIB (http://plib.sourceforge.net)
 *******************************************************/

#include <stdio.h>
#include "ssgLocal.h"


struct FLT_texture
{
  char fname [ 200 ] ;   /* 200 is the OpenFlight limit - don't increase it! */
} ;


struct FLT_vertex
{
  sgVec3 vert ;
  sgVec3 norm ;
  sgVec4 col  ;
  sgVec2 tex  ;
} ;


static ulList *texList = NULL ;
static ulList *vtxList = NULL ;
static FILE   *save_fd = NULL ;
static int max_bank = 1 ;
static int curr_bank = 0 ;

static int addToTexturePool ( char *tex )
{
  int max =  texList -> getNumEntities () ;

  for ( int p = strlen(tex) ; p >= 0 ; p-- )
    if ( tex[p] == '/' )
    {
      tex = & tex[p+1] ;
      break ;
    }

  for ( int i = 0 ; i < max ; i++ )
  {
    FLT_texture *target = (FLT_texture *)( texList -> getEntity ( i ) ) ;

    if ( strcmp ( target->fname, tex ) == 0 )
      return i ;
  }

  FLT_texture *add = new FLT_texture ;

  strncpy ( add->fname, tex, 199 ) ;

  texList -> addEntity ( add ) ;
  return max ;
}


static int addToVertexPool ( FLT_vertex *vert )
{
  int max =  vtxList -> getNumEntities () ;

  for ( int i = 0 ; i < max ; i++ )
  {
    FLT_vertex *target = (FLT_vertex *)( vtxList -> getEntity ( i ) ) ;

    if ( memcmp ( target, vert, sizeof ( FLT_vertex ) ) == 0 )
      return i ;
  }

  FLT_vertex *add = new FLT_vertex ;

  *add = *vert ;

  vtxList -> addEntity ( add ) ;
  return max ;
}


static int writeChar ( char val )
{
  fwrite ( & val, sizeof(char), 1, save_fd ) ;
  return sizeof(char) ;
}


static int writeShort ( short val )
{
  writeChar ( (val >> 8) & 0xFF ) ;
  writeChar ( val & 0xFF ) ;
  return sizeof(short) ;
}


static int writeInt( int val )
{
  writeChar ( (val >> 24) & 0xFF ) ;
  writeChar ( (val >> 16) & 0xFF ) ;
  writeChar ( (val >> 8 ) & 0xFF ) ;
  writeChar ( val & 0xFF ) ;
  return sizeof(int) ;
}


static void swabInt ( int *x )
{
  unsigned int t = (unsigned int) *x ;

  *x = ((t & 0x000000FF) << 24) |
       ((t & 0x0000FF00) <<  8) |
       ((t & 0x00FF0000) >>  8) |
       ((t & 0xFF000000) >> 24) ;
}


static void swabFloat ( float *x )
{
  int *a = (int *) x ;

  swabInt ( a ) ;
}


static void swabDouble ( double *x )
{
  int *a = (int *) x ;
  int *b = a+1 ;

  swabInt ( a ) ;
  swabInt ( b ) ;

  *a = *a ^ *b ;
  *b = *a ^ *b ;
  *a = *a ^ *b ;
}



static int writeFloat ( float val )
{
  if ( ulIsLittleEndian )
    swabFloat ( & val ) ;

  fwrite ( & val, sizeof(float), 1, save_fd ) ;
  return sizeof(float) ;
}


static int writeDouble( double val )
{
  if ( ulIsLittleEndian )
    swabDouble ( & val ) ;

  fwrite ( & val, sizeof(double), 1, save_fd ) ;
  return sizeof(double) ;
}


static int writeString( char *str, int slen )
{
  fwrite ( str, sizeof(char), slen, save_fd ) ;
  return slen ;
}



static void writeHeader ()
{
  int  len = 0;

  len += writeShort ( 1 ) ;
  len += writeShort ( 2+2+8+4+4+32+2+2+2+2+2+1+1+4+4+
                      4+4+4+4+4+4+4+4+4+4+4+4+4+2+2+
                      4+8+8+8+8+2+2+4+4+2+2+2+2+4+8+
                      8+8+8+8+8+8+8+2+2+2+2+2+2+2+2+
                      4+2+2+2+8+8+2+2 ) ;

  len += writeString( "       ", 8 ) ;  /* ID */
  len += writeInt   ( 1570 ) ;  /* Format Rev level 15.70 */
  len += writeInt   ( 0 ) ;  /* Edit rev level */
  len += writeString( "Unknown Date.                  ", 32 ) ; /* Date/Time */
  len += writeShort ( 0 ) ; /* Next Group Node */
  len += writeShort ( 0 ) ; /* Next LOD Node */
  len += writeShort ( 0 ) ; /* Next Object Node */
  len += writeShort ( 0 ) ; /* Next Face node */
  len += writeShort ( 1 ) ; /* Unit multiplier */
  len += writeChar  ( 0 ) ;  /* Unit type Meters */
  len += writeChar  ( 0 ) ;  /* TexWhite */
  len += writeInt   ( 0 ) ;  /* Flags */

  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Projection Type */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */
  len += writeInt   ( 0 ) ;  /* Reserved */

  len += writeShort ( 0 ) ;  /* Next DOF node */
  len += writeShort ( 1 ) ;  /* Vertex storage type (DOUBLE!) */
  len += writeInt   ( 100 ) ;  /* Database Origin */

  len += writeDouble( 0 ) ; /* SW DB X */
  len += writeDouble( 0 ) ; /* SW DB Y */
  len += writeDouble( 0 ) ; /* Delta X */
  len += writeDouble( 0 ) ; /* Delta Y */

  len += writeShort ( 0 ) ; /* Next Sound node */
  len += writeShort ( 0 ) ; /* Next Path node */

  len += writeInt   ( 0 ) ; /* Reserved */
  len += writeInt   ( 0 ) ; /* Reserved */

  len += writeShort ( 0 ) ; /* Next Clip node */
  len += writeShort ( 0 ) ; /* Next Text node */
  len += writeShort ( 0 ) ; /* Next BSP node */
  len += writeShort ( 0 ) ; /* Next Switch node */

  len += writeInt   ( 0 ) ; /* Reserved */

  len += writeDouble( 0 ) ; /* SW Corner lat */
  len += writeDouble( 0 ) ; /* SW Corner lon */
  len += writeDouble( 0 ) ; /* NE Corner lat */
  len += writeDouble( 0 ) ; /* NE Corner lon */
  len += writeDouble( 0 ) ; /* Org Lat */
  len += writeDouble( 0 ) ; /* Org Lon */
  len += writeDouble( 0 ) ; /* Lambert upper lat */
  len += writeDouble( 0 ) ; /* Lambert upper lon */
  len += writeShort ( 0 ) ; /* Next Light source */
  len += writeShort ( 0 ) ; /* Next Light Point */
  len += writeShort ( 0 ) ; /* Next Road */
  len += writeShort ( 0 ) ; /* Next CAT */
  len += writeShort ( 0 ) ; /* Reserved */
  len += writeShort ( 0 ) ; /* Reserved */
  len += writeShort ( 0 ) ; /* Reserved */
  len += writeShort ( 0 ) ; /* Reserved */
  len += writeInt   ( 0 ) ; /* Earth Ellipsoidal model (==WGS84) */
  len += writeShort ( 0 ) ; /* Next Adaptive node */
  len += writeShort ( 0 ) ; /* Next Curve node */
  len += writeShort ( 0 ) ; /* Reserved */
  len += writeDouble( 0 ) ; /* Delta Z to place */
  len += writeDouble( 0 ) ; /* Radius Z to place */
  len += writeShort ( 0 ) ; /* Next Mesh node */
  len += writeShort ( 0 ) ; /* Reserved */
}


static void writePush () { writeShort ( 10 ) ; writeShort (  4 ) ; }
static void writePop  () { writeShort ( 11 ) ; writeShort (  4 ) ; }

static void writeObject ()
{
  writeShort ( 4 ) ;
  writeShort ( 2+2+8+4+2+2+2+2+2+2 ) ;

  static int next_name = 0 ;
  char name [ 8 ] ;
  sprintf ( name, "O%d", next_name++ ) ;
  writeString ( name, 8 ) ;  /* Name */
  writeInt   ( 0x00000000 ) ;     /* Flags */
  writeShort ( 0 ) ;     /* Relative Priority */
  writeShort ( 0 ) ;     /* Transparency (1-Alpha) */
  writeShort ( 0 ) ;    /* Special ID 1 */
  writeShort ( 0 ) ;    /* Special ID 2 */
  writeShort ( 0 ) ;    /* Significance */
  writeShort ( 0 ) ;    /* Layer */
}



static void writeFace ( ssgLeaf *leaf, int isLine )
{
  writeShort ( 5 ) ;
  writeShort ( 2+2+8+4+2+1+1+2+2+1+1+2+2+2+2+2+4+2+1+1+
               4+1+1+2+4+4+4+2+2+4+4+2+2 ) ;

  static int next_name = 0 ;
  char name [ 8 ] ;
  sprintf ( name, "F%d", next_name++ ) ;
  writeString ( name, 8 ) ;  /* Name */
  writeInt   ( 0 ) ;    /* IR color code */
  writeShort ( 0 ) ;    /* Relative Priority */
  writeChar  ( isLine ? 2 : 0 ) ;    /* Draw Type */
  writeChar  ( 0 ) ;    /* Texture-White */
  writeShort ( 0 ) ;    /* Color name index */
  writeShort ( 0 ) ;    /* Alternate Color name index */
  writeChar  ( 0 ) ;
  writeChar  ( 0 ) ;    /* Billboard */
  writeShort (-1 ) ;    /* Detail Texture */

  ssgSimpleState *s = (ssgSimpleState *)( leaf -> getState () ) ;

  if ( s != NULL && s -> getTextureFilename () != NULL )
    writeShort ( addToTexturePool ( s -> getTextureFilename () ) ) ;
  else
    writeShort (-1 ) ; /* No Texture */

  writeShort (-1 ) ;    /* Material */
  writeShort ( 0 ) ;/* XXXX */ /* SMC */
  writeShort ( 0 ) ;    /* DFAD FID */
  writeInt   ( 0 ) ;    /* IR Material Code */
  writeShort ( 0 ) ;    /* Transparent */
  writeChar  ( 0 ) ;    /* LOD generation control */
  writeChar  ( 0 ) ;    /* Line Style */
  writeInt   ( 0x60000000 ) ; /* Flags == No Colour */
  writeChar  ( 3 ) ;    /* Light mode == Use Vertex colour & Normal */
  writeChar  ( 0 ) ;
  writeShort ( 0 ) ;
  writeInt   ( 0 ) ;
  writeInt   ( 0xFFFFFFFF ) ;  /* Packed Color */
  writeInt   ( 0xFFFFFFFF ) ;  /* Packed Alt Color */
  writeShort ( 0 ) ;   /* Texture mapping index */
  writeShort ( 0 ) ;
  writeInt   ( 0 ) ;   /* Primary Color Index */
  writeInt   ( 0 ) ;   /* Secondary Color Index */
  writeShort ( 0 ) ;
  writeShort ( 0 ) ;
}

static void writeLeaf ( ssgLeaf *leaf )
{
  int i ;

  writeObject () ;
  writePush   () ;

  for ( i = 0 ; i < leaf -> getNumLines () ; i++ )
  {
    writeFace ( leaf, TRUE ) ;

    writePush  () ;
    writeShort ( 72 ) ;  /* Vertex List */
    writeShort ( 2+2+4*2 ) ;

    short vv [ 2 ] ;

    leaf -> getLine ( i, &vv[0], &vv[1] ) ;

    for ( int j = 0 ; j < 2 ; j++ )
    {
      FLT_vertex vert ;
      sgCopyVec3 ( vert.vert, leaf -> getVertex   ( vv[j] ) ) ;
      sgCopyVec3 ( vert.norm, leaf -> getNormal   ( vv[j] ) ) ;
      sgCopyVec4 ( vert.col , leaf -> getColour   ( vv[j] ) ) ;
      sgCopyVec2 ( vert.tex , leaf -> getTexCoord ( vv[j] ) ) ;

      writeInt ( addToVertexPool ( & vert ) * 60 + 8 ) ;
    }

    writePop  () ;
  }

  for ( i = 0 ; i < leaf -> getNumTriangles () ; i++ )
  {
    writeFace ( leaf, FALSE ) ;

    writePush  () ;
    writeShort ( 72 ) ;  /* Vertex List */
    writeShort ( 2+2+4*3 ) ;

    short vv [ 3 ] ;

    leaf -> getTriangle ( i, &vv[0], &vv[1], &vv[2] ) ;

    for ( int j = 0 ; j < 3 ; j++ )
    {
      FLT_vertex vert ;
      sgCopyVec3 ( vert.vert, leaf -> getVertex   ( vv[j] ) ) ;
      sgCopyVec3 ( vert.norm, leaf -> getNormal   ( vv[j] ) ) ;
      sgCopyVec4 ( vert.col , leaf -> getColour   ( vv[j] ) ) ;
      sgCopyVec2 ( vert.tex , leaf -> getTexCoord ( vv[j] ) ) ;

      writeInt ( addToVertexPool ( & vert ) * 60 + 8 ) ;
    }

    writePop  () ;
  }

  writePop    () ;
}


static void writeAnimationGroup ()
{
  writeShort ( 2 ) ;
  writeShort ( 2+2+8+2+2+4+2+2+2+1+1+4 ) ;
  writeString ( "Animate", 8 ) ;  /* Name */
  writeShort ( 0 ) ;              /* Relative Priority */
  writeShort ( 0 ) ;
  writeInt   ( 0x40000000 ) ; /* Flags = Animate forwards */
  writeShort ( 100 ) ;/* Special ID 1 */
  writeShort ( 0 ) ;  /* Special ID 2 */
  writeShort ( 0 ) ;  /* Significance */
  writeChar  ( 0 ) ;  /* Layer */
  writeChar  ( 0 ) ;
  writeInt   ( 0 ) ;
}


static void writeVanillaGroup ()
{
  writeShort ( 2 ) ;
  writeShort ( 2+2+8+2+2+4+2+2+2+1+1+4 ) ;

  static int next_name = 0 ;
  char name [ 8 ] ;
  sprintf ( name, "Q%d", next_name++ ) ;
  writeString ( name, 8 ) ;  /* Name */
  writeShort ( 0 ) ;              /* Relative Priority */
  writeShort ( 0 ) ;
  writeInt   ( 0 ) ;  /* Flags */
  writeShort ( 0 ) ;  /* Special ID 1 */
  writeShort ( 0 ) ;  /* Special ID 2 */
  writeShort ( 0 ) ;  /* Significance */
  writeChar  ( 0 ) ;  /* Layer */
  writeChar  ( 0 ) ;
  writeInt   ( 0 ) ;
}


static void writeGroup ( ssgBranch *bra )
{
  writeShort ( 2 ) ;
  writeShort ( 2+2+8+2+2+4+2+2+2+1+1+4 ) ;

  static int next_name = 0 ;
  char name [ 8 ] ;
  sprintf ( name, "G%d", next_name++ ) ;
  writeString ( name, 8 ) ;  /* Name */
  writeShort ( 0 ) ;              /* Relative Priority */
  writeShort ( 0 ) ;
  writeInt   ( 0 ) ;  /* Flags */
  writeShort ( 0 ) ;  /* Special ID 1 */
  writeShort ( 0 ) ;  /* Special ID 2 */
  writeShort ( 0 ) ;  /* Significance */
  writeChar  ( 0 ) ;  /* Layer */
  writeChar  ( 0 ) ;
  writeInt   ( 0 ) ;
}


static void writeEntity ( ssgEntity *ent )
{
  if ( ent == NULL )
    return ;

  if ( ! ent -> isAKindOf ( ssgTypeBranch () ) )
  {
    if ( ent -> isAKindOf ( ssgTypeTween () ) )
      ((ssgTween *)ent) -> setBank ( curr_bank ) ;

    writeLeaf ( (ssgLeaf *)ent ) ;

    if ( ent -> isAKindOf ( ssgTypeTween () ) )
      ((ssgTween *)ent) -> setBank ( 0 ) ;

    return ;
  }

  if ( ((ssgBranch *)ent) -> getNumKids() == 0 )
    return ;

  writeGroup ( (ssgBranch *)ent ) ;
  writePush () ;

  for ( int i = 0 ; i < ((ssgBranch *)ent) -> getNumKids() ; i++ )
    writeEntity ( ((ssgBranch *)ent) -> getKid ( i ) ) ;

  writePop () ;
}


static void collectPalettesFromLeaf ( ssgLeaf *leaf )
{
  ssgSimpleState *s = (ssgSimpleState *)( leaf -> getState () ) ;

  if ( s != NULL && s -> getTextureFilename () != NULL )
    addToTexturePool ( s -> getTextureFilename () ) ;

  if ( leaf -> isAKindOf ( ssgTypeTween () ) )
  {
    ssgTween *t = (ssgTween *) leaf ;

    if ( t -> getNumBanks () > max_bank ) max_bank = t -> getNumBanks () ;

    for ( int i = 0 ; i < t->getNumBanks() ; i++ )
    {
      t -> setBank ( i ) ;

      for ( int i = 0 ; i < t -> getNumVertices () ; i++ )
      {
        FLT_vertex vert ;
        sgCopyVec3 ( vert.vert, t -> getVertex   ( i ) ) ;
        sgCopyVec3 ( vert.norm, t -> getNormal   ( i ) ) ;
        sgCopyVec4 ( vert.col , t -> getColour   ( i ) ) ;
        sgCopyVec2 ( vert.tex , t -> getTexCoord ( i ) ) ;
        addToVertexPool ( & vert ) ;
      }
    }

    t -> setBank ( 0 ) ;
  }

  for ( int i = 0 ; i < leaf -> getNumVertices () ; i++ )
  {
    FLT_vertex vert ;
    sgCopyVec3 ( vert.vert, leaf -> getVertex   ( i ) ) ;
    sgCopyVec3 ( vert.norm, leaf -> getNormal   ( i ) ) ;
    sgCopyVec4 ( vert.col , leaf -> getColour   ( i ) ) ;
    sgCopyVec2 ( vert.tex , leaf -> getTexCoord ( i ) ) ;
    addToVertexPool ( & vert ) ;
  }
}

static void collectPalettes ( ssgEntity *ent )
{
  if ( ent == NULL )
    return ;

  if ( ! ent -> isAKindOf ( ssgTypeBranch () ) )
  {
    collectPalettesFromLeaf ( (ssgLeaf *)ent ) ;
    return ;
  }

  for ( int i = 0 ; i < ((ssgBranch *)ent) -> getNumKids() ; i++ )
    collectPalettes ( ((ssgBranch *)ent) -> getKid ( i ) ) ;
}

static void writePalettes ( ssgEntity *root )
{
  int i ;

  max_bank = 1 ;

  collectPalettes ( root ) ;

  for ( i = 0 ; i < texList -> getNumEntities () ; i++ )
  {
    writeShort  ( 64 ) ;  /* Texture Palette */
    writeShort  ( 2+2+200+4+4+4 ) ;
    writeString ( ((FLT_texture *)(texList -> getEntity ( i )))->fname, 200 ) ;
    writeInt    ( i ) ;
    writeInt    ( i % 16 ) ;
    writeInt    ( i / 16 ) ;
  }

  writeShort  ( 67 ) ;  /* Vertex Palette header. */
  writeShort  ( 2+2+4 ) ;
  writeInt    ( 2+2+4 +
                ( 2+2+2+2+8+8+8+4*3+4*2+4+4 ) * vtxList -> getNumEntities () ) ;

  for ( i = 0 ; i < vtxList -> getNumEntities () ; i++ )
  {
    FLT_vertex *vv = (FLT_vertex *)(vtxList -> getEntity ( i )) ;

    writeShort  ( 70 ) ;  /* Vertex Palette with Normal and Texture */
    writeShort  ( 2+2+2+2+8+8+8+4*3+4*2+4+4 ) ;
    writeShort  ( 0 ) ;  /* Colour name index */
    writeShort  ( 0x1000 ) ;  /* Flags == Use Packed Colour */
    writeDouble ( vv -> vert [ 0 ] ) ;
    writeDouble ( vv -> vert [ 1 ] ) ;
    writeDouble ( vv -> vert [ 2 ] ) ;
    writeFloat  ( vv -> norm [ 0 ] ) ;
    writeFloat  ( vv -> norm [ 1 ] ) ;
    writeFloat  ( vv -> norm [ 2 ] ) ;
    writeFloat  ( vv -> tex [ 0 ] ) ;
    writeFloat  ( vv -> tex [ 1 ] ) ;
    writeInt    ( ( (int) ( vv -> col [ 3 ] * 255.0f ) << 24 ) +
                  ( (int) ( vv -> col [ 2 ] * 255.0f ) << 16 ) +
                  ( (int) ( vv -> col [ 1 ] * 255.0f ) <<  8 ) +
                  ( (int) ( vv -> col [ 0 ] * 255.0f ) <<  0 ) ) ;
    writeInt    ( 0 ) ;
  }
}



int ssgSaveFLT ( const char *fname, ssgEntity *root )
{
  if ( ( save_fd = fopen ( fname, "wb") ) == NULL )
  {
    fprintf ( stderr, "Could not open file '%s' for writing.\n", fname ) ;
    return FALSE ;
  }

  writeHeader () ;

  if ( texList == NULL )  texList = new ulList (  20 ) ;
  if ( vtxList == NULL )  vtxList = new ulList ( 200 ) ;

  writePalettes ( root ) ;

  writePush () ;
  writeVanillaGroup () ;
  writePush () ;

  if ( max_bank > 1 )
  {
    writeAnimationGroup () ;
    writePush () ;
  }

  for ( int i = 0 ; i < max_bank ; i++ )
  {
    curr_bank = i ;
    writeEntity ( root ) ;
  }

  if ( max_bank > 1 )
    writePop () ;

  writePop () ;
  writePop () ;

  fclose ( save_fd ) ;

  /* Cleanup */

// This block is commented out because it
// seems to screw things up elsewhere.
// However, it needs to be there to avoid
// a memory leak.
// !! DEBUG ME!!
#ifdef PREVENT_MEMORY_LEAK
  int max, i ;

  max = texList -> getNumEntities () ;

  for ( i = 0 ; i < max ; i++ )
    delete (FLT_texture *) texList -> getEntity ( i ) ;
  delete texList ;

  max = vtxList -> getNumEntities () ;

  for ( i = 0 ; i < max ; i++ )
    delete (FLT_vertex *) vtxList -> getEntity ( i ) ;
  delete vtxList ;
#endif

  return TRUE ;
}



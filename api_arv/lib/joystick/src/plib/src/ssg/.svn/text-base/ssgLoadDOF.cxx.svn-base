/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 1998,2003  Steve Baker
 
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

     $Id: ssgLoadDOF.cxx,v 1.1 2003/01/29 19:33:41 sjbaker Exp $
*/


#include "ssgLocal.h"

static ssgLoaderOptions* current_options = NULL ;

struct _NameValuePair
{
  char *name ;
  int  value ;
} ;

#define TOKEN_DOF1 1
#define TOKEN_EDOF 2
#define TOKEN_MATS 3
#define TOKEN_GEOB 4
#define TOKEN_MAT0 5
#define TOKEN_GOB1 6
#define TOKEN_MHDR 7
#define TOKEN_MCOL 8
#define TOKEN_MUVW 9
#define TOKEN_MTRA 10
#define TOKEN_MTEX 11
#define TOKEN_MSUB 12
#define TOKEN_MEND 13
#define TOKEN_GHDR 14
#define TOKEN_INDI 15
#define TOKEN_VERT 16
#define TOKEN_TVER 17
#define TOKEN_NORM 18
#define TOKEN_BRST 19
#define TOKEN_GEND 20
#define TOKEN_ILLEGAL  998
#define TOKEN_EOF      999


static _NameValuePair tags [] =
{
  { "DOF1", TOKEN_DOF1 },
  { "EDOF", TOKEN_EDOF },
  { "MATS", TOKEN_MATS },
  { "GEOB", TOKEN_GEOB },
  { "MAT0", TOKEN_MAT0 },
  { "GOB1", TOKEN_GOB1 },
  { "MHDR", TOKEN_MHDR },
  { "MCOL", TOKEN_MCOL },
  { "MUVW", TOKEN_MUVW },
  { "MTRA", TOKEN_MTRA },
  { "MTEX", TOKEN_MTEX },
  { "MSUB", TOKEN_MSUB },
  { "MEND", TOKEN_MEND },
  { "GHDR", TOKEN_GHDR },
  { "INDI", TOKEN_INDI },
  { "VERT", TOKEN_VERT },
  { "TVER", TOKEN_TVER },
  { "NORM", TOKEN_NORM },
  { "BRST", TOKEN_BRST },
  { "GEND", TOKEN_GEND },
  { NULL, 0 }
} ;


struct dofMaterial
{
  char *name      ;
  char *className ;

  sgVec4 ambient  ;
  sgVec4 diffuse  ;
  sgVec4 specular ;
  sgVec4 emission ;
  float shininess ;

  float uvwUoffset ;
  float uvwVoffset ;
  float uvwUtiling ;
  float uvwVtiling ;
  float uvwAngle   ;
  float uvwBlur    ;
  float uvwBlurOffset ;

  float transparency ;  /* Unused */
  int blendMode ;       /* 0 == No blend,
                           1 == BLEND_SRC_ALPHA,
                           2 == BLEND_CONST (unused) */

  int   textures ;
  char **mapName ;

  int subMaterials ;    /* Should be zero */
  dofMaterial *MAT0 ;   /* Should be unused */

  ssgSimpleState *ssg_material ;
} ;


static ulList *materials ;


static int getToken ( FILE *fd )
{
  char s [ 5 ] ;

  if ( fread ( s, 1, 4, fd ) != 4 )
    return TOKEN_EOF ;

  s [ 4 ] = '\0' ;

  for ( int i = 0 ; tags [ i ] . name != 0 ; i++ )
    if ( strcmp ( tags[i].name, s ) == 0 )
      return tags[i].value ;

  ulSetError ( UL_WARNING, "Illegal token '%s'", s ) ;
  return TOKEN_ILLEGAL ;
}


static short getShort ( FILE *fd )
{
  short val ;

  if ( fread ( & val, sizeof(short), 1, fd ) != 1 )
    return 0 ;

  val = ulEndianLittle16 ( (unsigned short) val ) ;
  return val ;
}


static void getColour ( FILE *fd, float *colour )
{
  if ( fread ( colour, sizeof(float), 4, fd ) != 4 )
    return ;

  ulEndianLittleArrayFloat ( colour, 4 ) ;
}


static float getFloat ( FILE *fd )
{
  float val ;

  if ( fread ( & val, sizeof(float), 1, fd ) != 1 )
    return 0 ;

  val = ulEndianLittleFloat ( val ) ;
  return val ;
}


static int getInteger ( FILE *fd )
{
  int val ;

  if ( fread ( & val, sizeof(int), 1, fd ) != 1 )
    return 0 ;

  val = ulEndianLittle32 ( (unsigned int) val ) ;
  return val ;
}


static int getLength ( FILE *fd )
{
  return getInteger ( fd ) ;
}


static int getTokenAndLength ( FILE *fd, int *len )
{
  int tok = getToken ( fd ) ;

  if ( tok == TOKEN_EOF )
  {
    *len = 0 ;
    return tok ;
  }

  *len = getLength ( fd ) ;
  return tok ;
}


static char *getQString ( FILE *fd, int *l )
{
  short len = getShort ( fd ) ;

  (*l) -= len + sizeof ( short ) ;

  char *res = new char [ len + 1 ] ;

  fread ( res, sizeof(char), len, fd ) ;
  res [ len ] = '\0' ;

  return res ;
}


static void getMAT0 ( FILE *fd )
{
  dofMaterial *mat = new dofMaterial ;

  mat -> ssg_material = new ssgSimpleState () ;

  materials -> addEntity ( mat ) ;

  while ( 1 )
  {
    int len ;

    switch ( getToken ( fd ) )
    {
      case TOKEN_MHDR :
        {
          len = getLength ( fd ) ;
          mat -> name = getQString ( fd, &len ) ;
          mat -> className = getQString ( fd, &len ) ;
fprintf ( stderr, "MAT0:  Name='%s', className='%s'\n", 
                   mat -> name, mat -> className ) ;

        }
        break ;
        
      case TOKEN_MTEX :
        {
          len = getLength ( fd ) ;
          mat -> textures = getInteger ( fd ) ;
          len -= sizeof ( int ) ;
          mat -> mapName = new char * [ mat -> textures ] ;
          for ( int i = 0 ; i < mat -> textures ; i++ )
            mat -> mapName [ i ] = getQString ( fd, & len ) ;
        }
        break ;

      case TOKEN_MCOL :
        {
          len = getLength ( fd ) ;
          getColour ( fd, mat -> ambient  ) ;
          getColour ( fd, mat -> diffuse  ) ;
          getColour ( fd, mat -> specular ) ;
          getColour ( fd, mat -> emission ) ;
          mat -> shininess = getFloat ( fd ) ;
          len -= 17 * sizeof(float) ;
        }
        break ;

      case TOKEN_MUVW :
        len = getLength ( fd ) ;
        mat -> uvwUoffset    = getFloat ( fd ) ;
        mat -> uvwVoffset    = getFloat ( fd ) ;
        mat -> uvwUtiling    = getFloat ( fd ) ;
        mat -> uvwVtiling    = getFloat ( fd ) ;
        mat -> uvwAngle      = getFloat ( fd ) ;
        mat -> uvwBlur       = getFloat ( fd ) ;
        mat -> uvwBlurOffset = getFloat ( fd ) ;
        len -= 7 * sizeof(float) ;
        break ;

      case TOKEN_MTRA :
        len = getLength ( fd ) ;
        mat -> transparency = getFloat   ( fd ) ;
        mat -> blendMode    = getInteger ( fd ) ;
        len -= sizeof(float) + sizeof(int) ;
        break ;

      case TOKEN_MSUB :
        {
          len = getLength ( fd ) ;

          mat -> subMaterials = getInteger ( fd ) ;

          if ( mat -> subMaterials != 0 )
            ulSetError ( UL_WARNING,
               "ssgLoadDOF: Sub-Materials are not supported." ) ;

          len -= sizeof ( int ) ;
        }
        break ;

      case TOKEN_MEND :
        {
          ssgSimpleState *m = mat -> ssg_material ;

          if ( mat -> textures > 0 )
          {
            m -> enable ( GL_TEXTURE_2D ) ;
            m -> setTexture ( mat -> mapName[0] ) ;
          }
          else
            m -> disable ( GL_TEXTURE_2D ) ;

          m -> disable ( GL_COLOR_MATERIAL ) ;
          m -> setMaterial ( GL_AMBIENT , mat -> ambient  ) ;
          m -> setMaterial ( GL_DIFFUSE , mat -> diffuse  ) ;
          m -> setMaterial ( GL_SPECULAR, mat -> specular ) ;
          m -> setMaterial ( GL_EMISSION, mat -> emission ) ;
          m -> setShininess ( mat -> shininess ) ;
        }
        return ; 

      case TOKEN_EOF : return ;

      default :
        {
          ulSetError ( UL_WARNING,
                 "ssgLoadDOF: Material contains an unrecognised token?!?" ) ;

          len = getLength ( fd ) ;
        }
        break ;
    }

    for ( int i = 0 ; i < len ; i++ ) getc ( fd ) ;
  }
}


static void getMaterials ( FILE *fd, int howMany )
{
ulSetError ( UL_WARNING, "Getting %d materials", howMany ) ;

  for ( int i = 0 ; i < howMany ; i++ )
  {
    int len ;

    switch ( getToken ( fd ) )
    {
      case TOKEN_MAT0 :
        len = getLength ( fd ) ;
        getMAT0 ( fd ) ;
        break ;


      case TOKEN_EOF  : return ;

      default :
        {
          ulSetError ( UL_WARNING,
                 "ssgLoadDOF: Material contains an unrecognised token?!?" ) ;

          len = getLength ( fd ) ;

          for ( int i = 0 ; i < len ; i++ )
            getc ( fd ) ;
        }
        return ;
    }
  }
}


static ssgEntity *getGOB1 ( FILE *fd )
{
  int flags       = 0 ;
  int paintflags  = 0 ;
  int materialRef = 0 ;
  int indices     = 0 ; short *index   = NULL ;
  int vertices    = 0 ; float *vertex  = NULL ;
  int tvertices   = 0 ; float *tvertex = NULL ;
  int normals     = 0 ; float *normal  = NULL ;
  int bursts      = 0 ;
  int *burstStart = NULL ;
  int *burstCount = NULL ;
  int *burstMtlID = NULL ;
  int *burstVperP = NULL ;

  while ( 1 )
  {
    int len ;

    switch ( getToken ( fd ) )
    {
      case TOKEN_GHDR :
        {
          len = getLength ( fd ) ;

          flags       = getInteger ( fd ) ;
          paintflags  = getInteger ( fd ) ;
          materialRef = getInteger ( fd ) ;

          len -= 3 * sizeof ( int ) ;
        }
        break ;

      case TOKEN_INDI :
        {
          len = getLength ( fd ) ;
          indices = getInteger ( fd ) ;
          index = new short [ indices ] ;
          fread ( index, sizeof(short), indices, fd ) ;
          ulEndianLittleArray16 ( (unsigned short *) index, indices ) ;
          len -= sizeof(int) + sizeof(short) * indices ;
        }
        break ;

      case TOKEN_VERT :
        {
          len = getLength ( fd ) ;
          vertices = getInteger ( fd ) ;
          vertex = new float [ vertices * 3 ] ;
          fread ( vertex, sizeof(float), vertices * 3, fd ) ;
          ulEndianLittleArrayFloat ( vertex, vertices * 3 ) ;
          len -= sizeof(int) + sizeof(float) * vertices * 3 ;
        }
        break ;

      case TOKEN_TVER :
        {
          len = getLength ( fd ) ;
          tvertices = getInteger ( fd ) ;
          tvertex = new float [ tvertices * 2 ] ;
          fread ( tvertex, sizeof(float), tvertices * 2, fd ) ;
          ulEndianLittleArrayFloat ( tvertex, tvertices * 2 ) ;
          len -= sizeof(int) + sizeof(float) * tvertices * 2 ;
        }
        break ;

      case TOKEN_NORM :
        {
          len = getLength ( fd ) ;
          normals = getInteger ( fd ) ;
          normal = new float [ normals * 3 ] ;
          fread ( normal, sizeof(float), normals * 3, fd ) ;
          ulEndianLittleArrayFloat ( normal, normals * 3 ) ;
          len -= sizeof(int) + sizeof(float) * normals * 3 ;
        }
        break ;

      case TOKEN_BRST :
        {
          len = getLength ( fd ) ;
          bursts = getInteger ( fd ) ;
          burstStart = new int [ bursts ] ;
          burstCount = new int [ bursts ] ;
          burstMtlID = new int [ bursts ] ;
          burstVperP = new int [ bursts ] ;
          fread ( burstStart, sizeof(int), bursts, fd ) ;
          fread ( burstCount, sizeof(int), bursts, fd ) ;
          fread ( burstMtlID, sizeof(int), bursts, fd ) ;
          fread ( burstVperP, sizeof(int), bursts, fd ) ;
          ulEndianLittleArray32 ( (unsigned int *) burstStart, bursts ) ;
          ulEndianLittleArray32 ( (unsigned int *) burstCount, bursts ) ;
          ulEndianLittleArray32 ( (unsigned int *) burstMtlID, bursts ) ;
          ulEndianLittleArray32 ( (unsigned int *) burstVperP, bursts ) ;

          len -= sizeof(int) + sizeof(int) * bursts * 4 ;
        }
        break ;

      case TOKEN_GEND :
        {
          float *colours = new float [ vertices * 4 ] ;

          for ( int i = 0 ; i < vertices ; i++ )
          {
            colours [ i*4 + 0 ] = 0.5f ;
            colours [ i*4 + 1 ] = 0.5f ;
            colours [ i*4 + 2 ] = 0.5f ;
            colours [ i*4 + 3 ] = 1.0f ;

            float tmp ;
            tmp                =  vertex [ i*3 + 1 ] ;
            vertex [ i*3 + 1 ] = -vertex [ i*3 + 2 ] ;
            vertex [ i*3 + 2 ] =  tmp ;
            tmp                =  normal [ i*3 + 1 ] ;
            normal [ i*3 + 1 ] = -normal [ i*3 + 2 ] ;
            normal [ i*3 + 2 ] =  tmp ;
          }
          ssgVertexArray    *vx = new ssgVertexArray   ( vertices , (sgVec3 *) vertex  ) ;
          ssgColourArray    *co = new ssgColourArray   ( vertices , (sgVec4 *) colours ) ;
          ssgNormalArray    *nm = new ssgNormalArray   ( normals  , (sgVec3 *) normal  ) ;
          ssgTexCoordArray *tx = new ssgTexCoordArray ( tvertices, (sgVec2 *) tvertex ) ;
          ssgIndexArray     *ix = new ssgIndexArray    ( indices  , index ) ;

          ssgVtxArray *va = new ssgVtxArray ( GL_TRIANGLES,
                                 vx, nm, tx, co, ix ) ;

          va -> setState ( ( (dofMaterial *)
                             ( materials -> getEntity ( materialRef ) )
                           ) -> ssg_material ) ;
          delete [] burstStart ;
          delete [] burstCount ;
          delete [] burstMtlID ;
          delete [] burstVperP ;

          return va ;
        }

      case TOKEN_EOF  : return NULL ;

      default :
        {
          ulSetError ( UL_WARNING,
                 "ssgLoadDOF: GOB1 contains an unrecognised token?!?" ) ;

          len = getLength ( fd ) ;
        }
        break ;
    }

    for ( int i = 0 ; i < len ; i++ ) getc ( fd ) ;
  }
}



static ssgBranch *getGeode ( FILE *fd, int howMany )
{
  ssgBranch *br = new ssgBranch () ;

ulSetError ( UL_WARNING, "Getting %d geodes", howMany ) ;

  for ( int i = 0 ; i < howMany ; i++ )
  {
    int len ;

    switch ( getToken ( fd ) )
    {
      case TOKEN_GOB1 :
        len = getLength ( fd ) ;

        br -> addKid ( getGOB1 ( fd ) ) ;

        break ;

      case TOKEN_EOF  : return br ;

      default :
        {
          ulSetError ( UL_WARNING,
                 "ssgLoadDOF: Geode contains an unrecognised token?!?" ) ;

          len = getLength ( fd ) ;

          for ( int i = 0 ; i < len ; i++ )
            getc ( fd ) ;
        }
        return br ;
    }
  }

  return br ;
}


static void cleanUp ()
{
  for ( int i = 0 ; i < materials -> getNumEntities () ; i++ )
    delete (dofMaterial *) ( materials -> getEntity ( i ) ) ;
  delete materials ;
}


ssgEntity *ssgLoadDOF ( const char *fname, const ssgLoaderOptions* options )
{
  ssgBranch *model ;

  ssgSetCurrentOptions ( (ssgLoaderOptions*)options ) ;
  current_options = ssgGetCurrentOptions () ;

  char filename [ 1024 ] ;
  current_options -> makeModelPath ( filename, fname ) ;

  FILE *fd = fopen ( filename, "ra" ) ;

  if ( fd == NULL )
  {
    ulSetError ( UL_WARNING,
                 "ssgLoadDOF: Failed to open '%s' for reading", filename ) ;
    return NULL ;
  }

  int len ;

  if ( getTokenAndLength ( fd, &len ) != TOKEN_DOF1 )
  {
    ulSetError ( UL_WARNING,
                 "ssgLoadDOF: '%s' does not start with 'DOF1'?!?", filename ) ;
    return NULL ;
  }
 
  materials = new ulList () ;
  model = new ssgBranch () ;

  while ( 1 )
  {
    switch ( getToken ( fd ) )
    {
      case TOKEN_MATS :
        getLength    ( fd ) ;
        getMaterials ( fd, getInteger ( fd ) ) ;
        break ;

      case TOKEN_GEOB :
        getLength ( fd ) ;
        model -> addKid ( getGeode  ( fd, getInteger ( fd ) ) ) ;
        break ;

      case TOKEN_EDOF : 
        cleanUp () ;
        return model ;

      case TOKEN_EOF  :
        cleanUp () ;
        return NULL ;

      default :
        ulSetError ( UL_WARNING,
                 "ssgLoadDOF: '%s' contains an unrecognised token?!?", filename ) ;
        cleanUp () ;
        return NULL ;
    }
  }
}



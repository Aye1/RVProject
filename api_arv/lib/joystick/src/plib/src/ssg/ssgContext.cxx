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

     $Id: ssgContext.cxx,v 1.20 2002/12/02 15:04:06 sjbaker Exp $
*/


#include "ssgLocal.h"

ssgContext *_ssgCurrentContext = NULL ;

ssgContext::~ssgContext ()
{
  if ( isCurrent() )
    _ssgCurrentContext = NULL ;

  //~~ T.G. was ordinary delete before - must be derefed
  ssgDeRefDelete ( currentState );   
  ssgDeRefDelete (  basicState );    	

  delete frustum ;
}


ssgContext::ssgContext ()
{
  makeCurrent () ;
  currentState     = NULL  ;
  basicState       = NULL  ;
  //orthographic   = FALSE ;
  cullFace         = TRUE  ;
  ovTexture        = FALSE ;
  ovCullface       = FALSE ;
  ovState          = NULL  ;
  sgCopyMat4 ( cameraMatrix, _ssgOpenGLAxisSwapMatrix ) ;

  frustum = new sgFrustum ;
  frustum -> setNearFar ( 1.0, 10000.0 ) ;
  frustum -> setFOV     ( 60.0, 45.0 ) ;

  currentState = new ssgSimpleState ( 1 ) ;
  currentState -> ref(); //~~ T.G. 

  basicState   = new ssgSimpleState ( 0 ) ;
  basicState -> ref(); //~~ T.G. 

  currentState -> force () ;

  /* The order of the two following lines is essential.
     setTexture(NULL) currently sets the TEXTURE bit in
     dont_care... not the desired effect here. /PL */

  basicState->setTexture ( (ssgTexture*) NULL ) ;
  basicState->dont_care      = 0 ;
  basicState->colour_material_mode = GL_AMBIENT_AND_DIFFUSE ;
  sgSetVec4(basicState->specular_colour,1.0f,1.0f,1.0f,1.0f);
  sgSetVec4(basicState->emission_colour,0.0f,0.0f,0.0f,1.0f);
  sgSetVec4(basicState->ambient_colour, 1.0f,1.0f,1.0f,1.0f);
  sgSetVec4(basicState->diffuse_colour, 1.0f,1.0f,1.0f,1.0f);
  basicState->shade_model = GL_SMOOTH ;
  basicState->shininess   = 0.0f ;
  basicState->alpha_clamp = 0.01f ;

  for ( int i = 0 ; i < 6 ; i++ )
    sgSetVec4 ( clipPlane[i], 0, 0, 1, 0 ) ;

  enabledClipPlanes = 0 ;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


int ssgContext::isCurrent ()
{
  return _ssgCurrentContext == this ;
}

void ssgContext::forceBasicState ()
{
  if ( ! ovCullface )
    glEnable ( GL_CULL_FACE ) ;

  cullFace = TRUE ;

  if ( ovState != NULL )
    ovState -> force () ;
  else
    basicState -> force () ;
}


void ssgContext::makeCurrent ()
{
  _ssgCurrentContext = this ;
}

void ssgContext::getCameraPosition ( sgVec3 pos )
{
  sgCopyVec3 ( pos, cameraMatrix [ 3 ] ) ;
}

void ssgContext::overrideState ( ssgState *s )
{
  ovState = s ;
}

void ssgContext::overrideTexture ( int on_off )
{
  ovTexture = on_off ;
}

void ssgContext::overrideCullface ( int on_off )
{
  ovCullface = on_off ;
}

 
void ssgContext::setFrustum ( float l, float r, float b, float t, float n, float f )
{
  frustum -> setFrustum ( l, r, b, t, n, f ) ;
}

void ssgContext::setOrtho ( float l, float r, float b, float t, float n, float f )
{
  frustum -> setOrtho ( l, r, b, t, n, f ) ;
}

void ssgContext::getNearFar ( float *n, float *f )
{
  frustum -> getNearFar ( n, f ) ;
}

void ssgContext::getFOV ( float *w, float *h )
{
  frustum -> getFOV ( w, h ) ;
}

void ssgContext::setNearFar ( float n, float f )
{
  frustum -> setNearFar ( n, f ) ;
}

void ssgContext::getOrtho ( float *w, float *h )
{
  frustum -> getOrtho ( w, h ) ;
}

void ssgContext::setOrtho ( float w, float h )
{
  frustum -> setOrtho ( w, h ) ;
}

void ssgContext::setFOV ( float w, float h )
{
  frustum -> setFOV ( w, h ) ;
}


void ssgContext::setCamera ( sgMat4 mat )
{
  sgMat4 viewmat ;

  sgTransposeNegateMat4 ( viewmat, mat ) ;

  sgCopyMat4    ( cameraMatrix, _ssgOpenGLAxisSwapMatrix ) ;
  sgPreMultMat4 ( cameraMatrix, viewmat ) ;
}

void ssgContext::setCamera ( const sgCoord *coord )
{
  sgMat4 viewmat, mat ;

  sgMakeCoordMat4 ( mat, coord ) ;
  sgTransposeNegateMat4 ( viewmat, mat ) ;

  sgCopyMat4      ( cameraMatrix, _ssgOpenGLAxisSwapMatrix ) ;
  sgPreMultMat4   ( cameraMatrix, viewmat ) ;
}

void ssgContext::setCameraLookAt ( const sgVec3 eye, const sgVec3 center, const sgVec3 up )
{
  sgMat4 mat ;
  sgMakeLookAtMat4 ( mat, eye, center, up ) ;

  setCamera ( mat ) ;
}

void ssgContext::setCameraLookAt ( const sgVec3 eye, const sgVec3 center )
{
  sgVec3 up ;
  sgSetVec3 ( up, 0.0f, 0.0f, 1.0f ) ;

  sgMat4 mat ;
  sgMakeLookAtMat4 ( mat, eye, center, up ) ;

  setCamera ( mat ) ;
}

void ssgContext::loadProjectionMatrix()
{
  glLoadIdentity () ;
  pushProjectionMatrix () ;
}

void ssgContext::pushProjectionMatrix ()
{
  pushProjectionMatrix ( frustum ) ;
}

void ssgContext::pushProjectionMatrix ( sgFrustum *f )
{
  if ( f -> isOrtho() )
    glOrtho   ( f -> getLeft() , f -> getRight(),
                f -> getBot () , f -> getTop  (),
                f -> getNear() , f -> getFar  () ) ;
  else
    glFrustum ( f -> getLeft() , f -> getRight(),
                f -> getBot () , f -> getTop  (),
                f -> getNear() , f -> getFar  () ) ;
}

void ssgContext::getProjectionMatrix ( sgMat4 dst )
{
  frustum -> getMat4 ( dst ) ;
}

void ssgContext::getModelviewMatrix ( sgMat4 dst )
{
  sgCopyMat4 ( dst, cameraMatrix ) ;
}

void ssgContext::loadModelviewMatrix ()
{
  glLoadMatrixf ( (float *) cameraMatrix ) ;
}

void ssgContext::loadModelviewMatrix ( sgMat4 mat )
{
  glLoadMatrixf ( (float *) mat ) ;
}

void ssgContext::cull ( ssgBranch *r )
{
  r -> cull ( frustum, cameraMatrix, TRUE ) ;
}


void ssgContext::removeClipPlanes ()
{
  for ( int i = 0 ; i < 6 ; i++ )
    glDisable ( (GLenum)( GL_CLIP_PLANE0 + i) ) ;
}

void ssgContext::applyClipPlanes ()
{
  if ( enabledClipPlanes == 0 )
    return ;

  for ( int i = 0 ; i < 6 ; i++ )
  {
    if ( enabledClipPlanes & (1<<i) )
    {
      double eqn[4] ;
      eqn[0] = (double) clipPlane[i][0] ;
      eqn[1] = (double) clipPlane[i][1] ;
      eqn[2] = (double) clipPlane[i][2] ;
      eqn[3] = (double) clipPlane[i][3] ;
      glClipPlane ( (GLenum)(GL_CLIP_PLANE0 + i), eqn ) ; 
      glEnable ( (GLenum)(GL_CLIP_PLANE0 + i) ) ;
    }
    else
      glDisable ( (GLenum)(GL_CLIP_PLANE0 + i) ) ;
  }
}



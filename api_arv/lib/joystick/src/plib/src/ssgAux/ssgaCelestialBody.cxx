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

     $Id: ssgaCelestialBody.cxx,v 1.3 2003/09/26 14:54:20 sjbaker Exp $
*/

// Written by Durk Talsma. Originally started October 1997, for distribution  
// with the FlightGear project. Version 2 was written in August and 
// September 1998. This code is based upon algorithms and data kindly 
// provided by Mr. Paul Schlyter. (pausch@saaf.se). 
//
// Separated out rendering pieces and converted to ssg by Curt Olson,
// March 2000
//
// Moved into ssgAux, July 2003.

#include <math.h>
#include "ssgaSky.h"
#include "ssgaSphere.h"


static int ssgaCelestialBodyOrbPreDraw( ssgEntity *e )
{
  ssgLeaf *f = (ssgLeaf *)e;
  if ( f -> hasState () ) f->getState()->apply() ;

  glPushAttrib( GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT );

  //glDisable( GL_DEPTH_TEST );
  glDisable( GL_FOG );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE ) ;

  return true;
}


static int ssgaCelestialBodyOrbPostDraw( ssgEntity *e )
{
  glPopAttrib();

  return true;
}


static int ssgaCelestialBodyHaloPreDraw( ssgEntity *e )
{
  ssgLeaf *f = (ssgLeaf *)e;
  if ( f -> hasState () ) f->getState()->apply() ;

  glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_FOG_BIT );

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_FOG );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

  return true;
}


static int ssgaCelestialBodyHaloPostDraw( ssgEntity *e )
{
  glPopAttrib();

  return true;
}


ssgaCelestialBody::ssgaCelestialBody( void )
{
  transform = 0;
  body_angle = 0;
  body_rotation = 0;
  body_right_ascension = 0;
  body_declination = 0;
  body_dist = 0;
}


ssgaCelestialBody::~ssgaCelestialBody( void )
{
  ssgDeRefDelete( transform );
}


ssgBranch * ssgaCelestialBody::build( const char* body_tex_path, const char* halo_tex_path, double body_size )
{
  ssgSimpleState *orb_state = NULL;
  ssgSimpleState *halo_state = NULL;

  // set up the orb state
  orb_state = new ssgSimpleState();
  if (body_tex_path) {
    orb_state->setTexture( body_tex_path );
    orb_state->setShadeModel( GL_SMOOTH );
    orb_state->enable( GL_LIGHTING );
    orb_state->enable( GL_CULL_FACE );
    orb_state->enable( GL_TEXTURE_2D );
    orb_state->enable( GL_COLOR_MATERIAL );
    orb_state->setColourMaterial( GL_DIFFUSE );
    orb_state->setMaterial( GL_AMBIENT, 0, 0, 0, 1 );
    orb_state->setMaterial( GL_EMISSION, 0, 0, 0, 1 );
    orb_state->setMaterial( GL_SPECULAR, 0, 0, 0, 1 );
    orb_state->enable( GL_BLEND );
    orb_state->enable( GL_ALPHA_TEST );
    orb_state->setAlphaClamp( 0.01f );
  }
  else {
    orb_state->setShadeModel( GL_SMOOTH );
    orb_state->disable( GL_LIGHTING );
    orb_state->enable( GL_CULL_FACE );
    orb_state->disable( GL_TEXTURE_2D );
    orb_state->enable( GL_COLOR_MATERIAL );
    orb_state->setColourMaterial( GL_AMBIENT_AND_DIFFUSE );
    orb_state->setMaterial( GL_EMISSION, 0, 0, 0, 1 );
    orb_state->setMaterial( GL_SPECULAR, 0, 0, 0, 1 );
    orb_state->disable( GL_BLEND );
    orb_state->disable( GL_ALPHA_TEST );
  }

  if (halo_tex_path) {

    // build the halo
    // GLuint texid;
    // GLubyte *texbuf;
    // texbuf = new GLubyte[64*64*3];
    // texid = makeHalo( texbuf, 64 );
    // my_glWritePPMFile("halo.ppm", texbuf, 64, 64, RGB);

    // set up the halo state
    halo_state = new ssgSimpleState();
    halo_state->setTexture( halo_tex_path );
    halo_state->enable( GL_TEXTURE_2D );
    halo_state->disable( GL_LIGHTING );
    halo_state->setShadeModel( GL_SMOOTH );
    halo_state->disable( GL_CULL_FACE );
    halo_state->enable( GL_COLOR_MATERIAL );
    halo_state->setColourMaterial( GL_AMBIENT_AND_DIFFUSE );
    halo_state->setMaterial( GL_EMISSION, 0, 0, 0, 1 );
    halo_state->setMaterial( GL_SPECULAR, 0, 0, 0, 1 );
    halo_state->enable( GL_ALPHA_TEST );
    halo_state->setAlphaClamp(0.01f);
    halo_state->enable ( GL_BLEND ) ;
  }

  return build( orb_state, halo_state, body_size );
}


ssgBranch * ssgaCelestialBody::build( ssgSimpleState *orb_state, ssgSimpleState *halo_state, double body_size )
{
  ssgVertexArray *halo_vl;
  ssgTexCoordArray *halo_tl;

  // clean-up previous
  ssgDeRefDelete( transform );

  // build the ssg scene graph sub tree for the sky and connected
  // into the provide scene graph branch
  transform = new ssgTransform;
  transform->ref();

  cl = new ssgColourArray( 1 );
  sgVec4 color;
  sgSetVec4( color, 1.0, 1.0, 1.0, 1.0 );
  cl->add( color );

  ssgBranch *orb = ssgaMakeSphere( orb_state, cl, (float)body_size, 15, 15, 
    ssgaCelestialBodyOrbPreDraw, ssgaCelestialBodyOrbPostDraw );

  transform->addKid( orb );

  // force a repaint of the colors with arbitrary defaults
  repaint( 0.0 );

  if (halo_state) {

    // Build ssg structure
    float size = float(body_size * 10.0);
    sgVec3 v3;
    halo_vl = new ssgVertexArray;
    sgSetVec3( v3, -size, 0.0, -size );
    halo_vl->add( v3 );
    sgSetVec3( v3, size, 0.0, -size );
    halo_vl->add( v3 );
    sgSetVec3( v3, -size, 0.0,  size );
    halo_vl->add( v3 );
    sgSetVec3( v3, size, 0.0,  size );
    halo_vl->add( v3 );

    sgVec2 v2;
    halo_tl = new ssgTexCoordArray;
    sgSetVec2( v2, 0.0f, 0.0f );
    halo_tl->add( v2 );
    sgSetVec2( v2, 1.0, 0.0 );
    halo_tl->add( v2 );
    sgSetVec2( v2, 0.0, 1.0 );
    halo_tl->add( v2 );
    sgSetVec2( v2, 1.0, 1.0 );
    halo_tl->add( v2 );

    ssgLeaf *halo = 
    new ssgVtxTable ( GL_TRIANGLE_STRIP, halo_vl, NULL, halo_tl, cl );
      halo->setState( halo_state );

    halo->setCallback( SSG_CALLBACK_PREDRAW, ssgaCelestialBodyHaloPreDraw );
    halo->setCallback( SSG_CALLBACK_POSTDRAW, ssgaCelestialBodyHaloPostDraw );

    transform->addKid( halo );
  }

  return transform;
}


bool ssgaCelestialBody::reposition( sgVec3 p, double angle, double rightAscension, double declination, double sol_dist )
{
  sgMat4 T1, T2, GST, RA, DEC;
  sgVec3 axis;
  sgVec3 v;

  sgMakeTransMat4( T1, p );

  sgSetVec3( axis, 0.0, 0.0, -1.0 );
  sgMakeRotMat4( GST, (float)angle, axis );

  sgSetVec3( axis, 0.0, 0.0, 1.0 );
  sgMakeRotMat4( RA, (float)((rightAscension * SGD_RADIANS_TO_DEGREES) - 90.0), axis );

  sgSetVec3( axis, 1.0, 0.0, 0.0 );
  sgMakeRotMat4( DEC, (float)(declination * SGD_RADIANS_TO_DEGREES), axis );

  sgSetVec3( v, 0.0, (float)sol_dist, 0.0 );
  sgMakeTransMat4( T2, v );

  sgMat4 TRANSFORM;
  sgCopyMat4( TRANSFORM, T1 );
  sgPreMultMat4( TRANSFORM, GST );
  sgPreMultMat4( TRANSFORM, RA );
  sgPreMultMat4( TRANSFORM, DEC );
  sgPreMultMat4( TRANSFORM, T2 );

  sgCoord skypos;
  sgSetCoord( &skypos, TRANSFORM );

  transform->setTransform( &skypos );

  return true;
}


bool ssgaCelestialBody::repaint( double angle )
{
  static double prev_angle = 9999.0;

  if (prev_angle != angle) {

    prev_angle = angle;

    double factor = 4*cos(angle);

    if (factor > 1) factor = 1.0;
    if (factor < -1) factor = -1.0;
    factor = factor/2 + 0.5f;

    sgVec4 color;
    color[0] = (float)pow(factor, 0.25);
    color[1] = (float)pow(factor, 0.50);
    color[2] = (float)pow(factor, 4.0);
    color[3] = 1.0;

    ssgaGammaCorrectRGB( color );

    float *ptr;
    ptr = cl->get( 0 );
    sgCopyVec4( ptr, color );
  }

  return true;
}


#if 0
static GLuint makeHalo( GLubyte *texbuf, int width )
{
    int texSize;
    GLuint texid;
    GLubyte *p;
    int i,j;
    double radius;
  
    // create a texture id
#ifdef GL_VERSION_1_1
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
#elif GL_EXT_texture_object
    glGenTexturesEXT(1, &texid);
    glBindTextureEXT(GL_TEXTURE_2D, texid);
#else
#   error port me
#endif

    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ) ;
 
    // create the actual texture contents
    texSize = width * width;
  
    if ( !texbuf ) {
	cout << "ouch ..." << endl;
	exit(-1);  // Ugly!
    }

    p = texbuf;
  
    radius = (double)(width / 2);
  
    GLubyte value;
    double x, y, d;
    for ( i = 0; i < width; i++ ) {
	for ( j = 0; j < width; j++ ) {
	    x = fabs((double)(i - (width / 2)));
	    y = fabs((double)(j - (width / 2)));
	    d = sqrt((x * x) + (y * y));
	    if (d < radius) {
		// t is 1.0 at center, 0.0 at edge
		double t = 1.0 - (d / radius);

		// inverse square looks nice 
		value = (int)((double) 0xff * (t*t));
	    } else {
		value = 0x00;
	    }
	    *p = value;
	    *(p+1) = value;
	    *(p+2) = value;
	    // *(p+3) = value;

	    p += 3;
	}
    }

    /* glTexImage2D( GL_TEXTURE_2D,
		  0,
		  GL_RGBA,
		  width, width,
		  0,
		  GL_RGBA, GL_UNSIGNED_BYTE,
		  texbuf ); */

    return texid;
}


#define RGB  3			// 3 bytes of color info per pixel
#define RGBA 4			// 4 bytes of color+alpha info
void my_glWritePPMFile(const char *filename, GLubyte *buffer, int win_width, int win_height, int mode)
{
    int i, j, k, q;
    unsigned char *ibuffer;
    FILE *fp;
    int pixelSize = mode==GL_RGBA?4:3;

    ibuffer = (unsigned char *) malloc(win_width*win_height*RGB);

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n# CREATOR: glReadPixel()\n%d %d\n%d\n",
	    win_width, win_height, UCHAR_MAX);
    q = 0;
    for (i = 0; i < win_height; i++) {
	for (j = 0; j < win_width; j++) {
	    for (k = 0; k < RGB; k++) {
		ibuffer[q++] = (unsigned char)
		    *(buffer + (pixelSize*((win_height-1-i)*win_width+j)+k));
	    }
	}
    }

    // *(buffer + (pixelSize*((win_height-1-i)*win_width+j)+k));

    fwrite(ibuffer, sizeof(unsigned char), RGB*win_width*win_height, fp);
    fclose(fp);
    free(ibuffer);

    printf("wrote file (%d x %d pixels, %d bytes)\n",
	   win_width, win_height, RGB*win_width*win_height);
}
#endif

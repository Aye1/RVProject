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

     $Id: ssgaStars.cxx,v 1.3 2003/09/26 14:54:20 sjbaker Exp $
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

#include "ssgaSky.h"


static int ssgaStarPreDraw( ssgEntity *e )
{
  ssgLeaf *f = (ssgLeaf *)e;
  if ( f -> hasState () ) f->getState()->apply() ;

  glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_FOG_BIT );

  //glDisable( GL_DEPTH_TEST );
  glDisable( GL_FOG );
  // glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

  return true;
}


static int ssgaStarPostDraw( ssgEntity *e )
{
    glPopAttrib();

    return true;
}


ssgaStars::ssgaStars( void ) :
  stars_transform(0),
  old_phase(-1)
{
}

  
ssgaStars::~ssgaStars( void )
{
  ssgDeRefDelete( stars_transform );
}


ssgBranch * ssgaStars::build( int num, sgdVec3 *star_data, double star_dist )
{
  sgVec4 color;

  // clean-up previous
  ssgDeRefDelete( stars_transform );

  // create new
  stars_transform = new ssgTransform;
  stars_transform->ref();

  if ( star_data == NULL )
    ulSetError(UL_WARNING, "null star data passed to ssgaStars::build()");

  // set up the orb state
  state = new ssgSimpleState();
  state->disable( GL_LIGHTING );
  state->disable( GL_CULL_FACE );
  state->disable( GL_TEXTURE_2D );
  state->enable( GL_COLOR_MATERIAL );
  state->setColourMaterial( GL_AMBIENT_AND_DIFFUSE );
  state->setMaterial( GL_EMISSION, 0, 0, 0, 1 );
  state->setMaterial( GL_SPECULAR, 0, 0, 0, 1 );
  state->enable( GL_BLEND );
  state->disable( GL_ALPHA_TEST );

  vl = new ssgVertexArray( num );
  cl = new ssgColourArray( num );
  // cl = new ssgColourArray( 1 );
  // sgSetVec4( color, 1.0, 1.0, 1.0, 1.0 );
  // cl->add( color );

  // Build ssg structure
  sgVec3 p;
  for ( int i = 0; i < num; ++i ) {
    // position seeded to arbitrary values
    sgSetVec3( p, 
      (float)( star_dist * cos( star_data[i][0] )
        * cos( star_data[i][1] )),
      (float)( star_dist * sin( star_data[i][0] )
        * cos( star_data[i][1] )),
      (float)( star_dist * sin( star_data[i][1] )));
    vl->add( p );

    // color (magnitude)
    sgSetVec4( color, 1.0, 1.0, 1.0, 1.0 );
    cl->add( color );
  }

  ssgLeaf *stars_obj = 
    new ssgVtxTable ( GL_POINTS, vl, NULL, NULL, cl );
  stars_obj->setState( state );
  stars_obj->setCallback( SSG_CALLBACK_PREDRAW, ssgaStarPreDraw );
  stars_obj->setCallback( SSG_CALLBACK_POSTDRAW, ssgaStarPostDraw );

  stars_transform->addKid( stars_obj );

  return stars_transform;
}


bool ssgaStars::reposition( sgVec3 p, double angle )
{
  sgMat4 T1, GST;
  sgVec3 axis;

  sgMakeTransMat4( T1, p );

  sgSetVec3( axis, 0.0, 0.0, -1.0 );
  sgMakeRotMat4( GST, (float)angle, axis );

  sgMat4 TRANSFORM;
  sgCopyMat4( TRANSFORM, T1 );
  sgPreMultMat4( TRANSFORM, GST );

  sgCoord skypos;
  sgSetCoord( &skypos, TRANSFORM );

  stars_transform->setTransform( &skypos );

  return true;
}


bool ssgaStars::repaint( double sol_angle, int num, sgdVec3 *star_data )
{
  double mag, nmag, alpha, factor, cutoff;
  float *color;

  int phase;

  // determine which star structure to draw
  if ( sol_angle > (0.5 * SGD_PI + 10.0 * SGD_DEGREES_TO_RADIANS ) ) {
    // deep night
    factor = 1.0;
    cutoff = 4.5;
    phase = 0;
  }
  else if ( sol_angle > (0.5 * SGD_PI + 8.8 * SGD_DEGREES_TO_RADIANS ) ) {
    factor = 1.0;
    cutoff = 3.8;
    phase = 1;
  }
  else if ( sol_angle > (0.5 * SGD_PI + 7.5 * SGD_DEGREES_TO_RADIANS ) ) {
    factor = 0.95;
    cutoff = 3.1;
    phase = 2;
  }
  else if ( sol_angle > (0.5 * SGD_PI + 7.0 * SGD_DEGREES_TO_RADIANS ) ) {
    factor = 0.9;
    cutoff = 2.4;
    phase = 3;
  }
  else if ( sol_angle > (0.5 * SGD_PI + 6.5 * SGD_DEGREES_TO_RADIANS ) ) {
    factor = 0.85;
    cutoff = 1.8;
    phase = 4;
  }
  else if ( sol_angle > (0.5 * SGD_PI + 6.0 * SGD_DEGREES_TO_RADIANS ) ) {
    factor = 0.8;
    cutoff = 1.2;
    phase = 5;
  }
  else if ( sol_angle > (0.5 * SGD_PI + 5.5 * SGD_DEGREES_TO_RADIANS ) ) {
    factor = 0.75;
    cutoff = 0.6;
    phase = 6;
  }
  else {
    // early dusk or late dawn
    factor = 0.7;
    cutoff = 0.0;
    phase = 7;
  }

  if( phase != old_phase ) {
    old_phase = phase;
    for ( int i = 0; i < num; ++i ) {
      // if ( star_data[i][2] < min ) { min = star_data[i][2]; }
      // if ( star_data[i][2] > max ) { max = star_data[i][2]; }

      // magnitude ranges from -1 (bright) to 4 (dim).  The
      // range of star and planet magnitudes can actually go
      // outside of this, but for our purpose, if it is brighter
      // that -1, we'll color it full white/alpha anyway and 4
      // is a convenient cutoff point which keeps the number of
      // stars drawn at about 500.

      // color (magnitude)
      mag = star_data[i][2];
      if ( mag < cutoff ) {
        nmag = ( 4.5 - mag ) / 5.5; // translate to 0 ... 1.0 scale
        // alpha = nmag * 0.7 + 0.3; // translate to a 0.3 ... 1.0 scale
        alpha = nmag * 0.85 + 0.15; // translate to a 0.15 ... 1.0 scale
        alpha *= factor;          // dim when the sun is brighter
      }
	  else {
        alpha = 0.0;
      }

      if (alpha > 1.0) { alpha = 1.0; }
      if (alpha < 0.0) { alpha = 0.0; }

      color = cl->get( i );
      sgSetVec4( color, 1.0, 1.0, 1.0, (float)alpha );
    }
  }

  return true;
}

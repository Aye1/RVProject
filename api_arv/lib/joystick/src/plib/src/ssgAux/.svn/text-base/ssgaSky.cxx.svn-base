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

     $Id: ssgaSky.cxx,v 1.3 2003/09/26 14:54:20 sjbaker Exp $
*/

// Written by Curtis Olson, started December 1997.
// SSG-ified by Curtis Olson, February 2000.
//
// Moved into ssgAux, July 2003.

#include "ssgaSky.h"


// Used for rise/set effects (flat earth - no rotation of skydome considered here )
void calc_celestial_angles( const sgVec3 body, const sgVec3 view, double& angle, double& rotation )
{
  sgVec3 pos;
  sgSubVec3(pos, body, view);
  angle = (90*SGD_DEGREES_TO_RADIANS) - atan2(pos[2], sqrt(pos[0]*pos[0] + pos[1]*pos[1]));
  rotation = (90*SGD_DEGREES_TO_RADIANS) - atan2(pos[0], pos[1]);
}


ssgaSky::ssgaSky( void )
{
  dome = 0;
  planets = 0;
  stars = 0;
  sol_ref = 0;
  pre_root = 0;
  post_root = 0;

  effective_visibility = visibility = 10000.0;

  // near cloud visibility state variables
  in_puff = false;
  puff_length = 0;
  puff_progression = 0;
  ramp_up = 0.15;
  ramp_down = 0.15;
}


ssgaSky::~ssgaSky( void )
{
  delete dome;
  delete planets;
  delete stars;
  delete pre_root;
  delete post_root;
}


void ssgaSky::build( double h_radius, double v_radius,
                     int nplanets, sgdVec3 *planet_data,
                     int nstars, sgdVec3 *star_data )
{
  // clean-up previous
  delete dome;
  delete planets;
  delete stars;
  delete pre_root;
  delete post_root;
  bodies.removeAll();
  clouds.removeAll();

  // build new
  pre_root = new ssgRoot;
  post_root = new ssgRoot;

  pre_selector = new ssgSelector;
  post_selector = new ssgSelector;

  pre_transform = new ssgTransform;
  post_transform = new ssgTransform;

  bodies_transform = new ssgTransform;
  stars_transform = new ssgTransform;

  dome = new ssgaSkyDome;
  pre_transform -> addKid( dome->build( h_radius, v_radius ) );

  planets = new ssgaStars;
  stars_transform -> addKid( planets->build( nplanets, planet_data, h_radius ) );

  stars = new ssgaStars;
  stars_transform -> addKid( stars->build( nstars, star_data, h_radius ) );

  pre_transform -> addKid( bodies_transform );
  pre_transform -> addKid( stars_transform );

  pre_selector->addKid( pre_transform );
  pre_selector->clrTraversalMaskBits( SSGTRAV_HOT );

  post_selector->addKid( post_transform );
  post_selector->clrTraversalMaskBits( SSGTRAV_HOT );

  pre_root->addKid( pre_selector );
  post_root->addKid( post_selector );
}


ssgaCelestialBody*
ssgaSky::addBody( const char *body_tex_path, const char *halo_tex_path, double size, double dist, bool sol )
{
  ssgaCelestialBody* body = new ssgaCelestialBody;
  bodies_transform->addKid( body->build( body_tex_path, halo_tex_path, size ) );
  bodies.add( body );

  body -> setDist( dist );

  if ( sol )
    sol_ref = body;

  return body;
}


ssgaCelestialBody*
ssgaSky::addBody( ssgSimpleState *orb_state, ssgSimpleState *halo_state, double size, double dist, bool sol )
{
  ssgaCelestialBody* body = new ssgaCelestialBody;
  bodies_transform->addKid( body->build( orb_state, halo_state, size ) );
  bodies.add( body );

  body -> setDist( dist );

  if ( sol )
    sol_ref = body;

  return body;
}


ssgaCloudLayer*
ssgaSky::addCloud( const char *cloud_tex_path, float span, float elevation, float thickness, float transition )
{
  ssgaCloudLayer* cloud = new ssgaCloudLayer;
  cloud->build ( cloud_tex_path, span, elevation, thickness, transition );
  clouds.add( cloud );
  return cloud;
}


ssgaCloudLayer*
ssgaSky::addCloud( ssgSimpleState *cloud_state, float span, float elevation, float thickness, float transition )
{
  ssgaCloudLayer* cloud = new ssgaCloudLayer;
  cloud->build ( cloud_state, span, elevation, thickness, transition );
  clouds.add( cloud );
  return cloud;
}


bool ssgaSky::repositionFlat( sgVec3 view_pos, double spin, double dt )
{
  int i;
  double angle;
  double rotation;
  sgCoord pos;

  for ( i = 0; i < bodies.getNum (); i++ ) {
    ssgaCelestialBody *body = bodies.get(i);
	body->reposition( view_pos, 0 );

	// Calc angles for rise/set effects
    body->getPosition ( & pos );
	calc_celestial_angles( pos.xyz, view_pos, angle, rotation );
    body->setAngle( angle );
    body->setRotation( rotation );
  }

  for ( i = 0; i < clouds.getNum (); i++ ) {
    clouds.get(i)->repositionFlat( view_pos, dt );
  }

  planets->reposition( view_pos, 0 );
  stars->reposition( view_pos, 0 );

  if ( sol_ref ) {
    dome->repositionFlat( view_pos, sol_ref->getRotation() );
  }
  else {
    dome->repositionFlat( view_pos, spin );
  }

  return true;
}


bool ssgaSky::reposition( sgVec3 view_pos, sgVec3 zero_elev, sgVec3 view_up, double lon, double lat, double alt, double spin, double gst, double dt )
{
  int i;

  double angle = gst * 15;	// degrees

  dome->reposition( zero_elev, lon, lat, spin );

  for ( i = 0; i < bodies.getNum (); i++ )
    bodies.get(i)->reposition( view_pos, angle );

  for ( i = 0; i < clouds.getNum (); i++ )
    clouds.get(i)->reposition( zero_elev, view_up, lon, lat, alt, dt );

  planets->reposition( view_pos, angle );
  stars->reposition( view_pos, angle );

  return true;
}


bool ssgaSky::repaint( sgVec4 sky_color, sgVec4 fog_color, sgVec4 cloud_color, double sol_angle,
                       int nplanets, sgdVec3 *planet_data,
                       int nstars, sgdVec3 *star_data )
{
  int i;

  if ( effective_visibility > 1000.0 ) {
    // turn on sky
    enable();

    dome->repaint( sky_color, fog_color, sol_angle, effective_visibility );

    for ( i = 0; i < bodies.getNum (); i++ )
      bodies.get(i)->repaint();

    for ( i = 0; i < clouds.getNum (); i++ )
      clouds.get(i)->repaint( cloud_color );

	planets->repaint( sol_angle, nplanets, planet_data );
	stars->repaint( sol_angle, nstars, star_data );
  }
  else {
    // turn off sky
    disable();
  }

  return true;
}


void ssgaSky::preDraw()
{
  ssgCullAndDraw( pre_root );
}


void ssgaSky::postDraw( float alt )
{
  // Sort clouds in order of distance from alt (furthest to closest)
  int i, j;
  int num = clouds.getNum ();
  if ( num > 0 ) {

	// Initialise cloud index
    int *index = new int [ num ];
    for ( i = 0; i < num; i++ ) {
		index [i] = i;
	}

	// Sort cloud index
    int temp;   // holding variable
    for ( i = 0; i < ( num - 1 ); i++ )    // to represent element to be compared
	{
      for( j = ( i + 1 ); j < num; j++ )   // to represent the rest of the elements
	  {
	    float d1 = (float)(fabs(alt - clouds.get(i)->getElevation()));
		float d2 = (float)(fabs(alt - clouds.get(j)->getElevation()));

        if (d1 < d2)
		{
          temp = index[i];
          index[i] = index[j];
          index[j] = temp;
		}
	  }
	}

    float slop = 5.0; // if we are closer than this to a cloud layer, don't draw cloud

	for ( int i = 0; i < num; i++ ) {

      ssgaCloudLayer *cloud = clouds.get(index[i]);

      float asl = cloud->getElevation();
      float thickness = cloud->getThickness();

	  // draw cloud only if below or above cloud layer
	  if ( alt < asl - slop || alt > asl + thickness + slop )
	    cloud->draw();
	}

	delete [] index;
  }
}


void ssgaSky::modifyVisibility( float alt, float time_factor )
{
  float effvis = visibility;

  for ( int i = 0; i < clouds.getNum (); ++i ) {

    ssgaCloudLayer *cloud = clouds.get(i);

    if ( cloud->isEnabled() ) {

      float asl = cloud->getElevation();
      float thickness = cloud->getThickness();
      float transition = cloud->getTransition();

      float ratio = 1.0;

      if ( alt < asl - transition ) {
        // below cloud layer
        ratio = 1.0;
	  }
	  else if ( alt < asl ) {
        // in lower transition
        ratio = (asl - alt) / transition;
	  }
	  else if ( alt < asl + thickness ) {
        // in cloud layer
        ratio = 0.0;
	  }
	  else if ( alt < asl + thickness + transition ) {
        // in upper transition
        ratio = (alt - (asl + thickness)) / transition;
      }
	  else {
        // above cloud layer
        ratio = 1.0;
	  }

      // accumulate effects from multiple cloud layers
      effvis *= ratio;

      if ( ratio < 1.0 ) {
        if ( ! in_puff ) {
          // calc chance of entering cloud puff
          double rnd = ssgaRandom();
          double chance = rnd * rnd * rnd;
          if ( chance > 0.95 ) { // * (diff - 25) / 50.0
            in_puff = true;
            puff_length = ssgaRandom() * 2.0; // up to 2 seconds
            puff_progression = 0.0;
		  }
	    }

        if ( in_puff ) {
          // modify actual_visibility based on puff envelope
          if ( puff_progression <= ramp_up ) {
            double x = 0.5 * SGD_PI * puff_progression / ramp_up;
            double factor = 1.0 - sin( x );
            effvis = (float)(effvis * factor);
	      }
	      else if ( puff_progression >= ramp_up + puff_length ) {
            double x = 0.5 * SGD_PI * 
              (puff_progression - (ramp_up + puff_length)) /
              ramp_down;
            double factor = sin( x );
            effvis = (float)(effvis * factor);
	      }
	      else {
            effvis = 0.0;
	      }

          puff_progression += time_factor;

          if ( puff_progression > puff_length + ramp_up + ramp_down) {
            in_puff = false; 
		  }
	    }

        // never let visibility drop below 25 meters
        if ( effvis <= 25.0 ) {
          effvis = 25.0;
	    }
	  }
	}
  } // for

  effective_visibility = effvis;
}

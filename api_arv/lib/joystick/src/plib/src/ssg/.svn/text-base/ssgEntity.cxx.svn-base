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

     $Id: ssgEntity.cxx,v 1.26 2003/11/20 13:58:35 stromberg Exp $
*/


#include "ssgLocal.h" 

void ssgEntity::copy_from ( ssgEntity *src, int clone_flags )
{
  ssgBase::copy_from ( src, clone_flags ) ;

  traversal_mask = src -> getTraversalMask () ;

  setTravCallback( SSG_CALLBACK_PRETRAV,  src->getTravCallback( SSG_CALLBACK_PRETRAV  ) );
  setTravCallback( SSG_CALLBACK_POSTTRAV, src->getTravCallback( SSG_CALLBACK_POSTTRAV ) );

  dirtyBSphere () ;
}


ssgEntity::ssgEntity (void)
{
  traversal_mask = 0xFFFFFFFF ;
  type = ssgTypeEntity () ;
  bsphere_is_invalid = TRUE ;

  preTravCB = NULL ;
  postTravCB = NULL ;
}


ssgEntity::~ssgEntity (void)
{
}


void ssgEntity::dirtyBSphere ()
{
  if ( bsphere_is_invalid ) return ;

  bsphere_is_invalid = TRUE ;

  int np = getNumParents () ; 

  for ( int i = 0  ; i < np ; i++ )
    getParent ( i ) -> dirtyBSphere () ;
}

void ssgEntity::visualiseBSphere ()
{
  if ( bsphere.isEmpty () )
    return ;

  glDisable ( GL_LIGHTING ) ;

  glTranslatef ( bsphere.getCenter()[0],
                 bsphere.getCenter()[1],
                 bsphere.getCenter()[2] ) ;

#ifdef USE_GLUT_SPHERES
  int spherebotch = (int)(this) % 9 ;

  switch ( spherebotch++ )
  {
    case 0 : glutWireSphere(bsphere.getRadius(),10,10); break ;
    case 1 : glutWireSphere(bsphere.getRadius(),11,10); break ;
    case 2 : glutWireSphere(bsphere.getRadius(),12,10); break ;
    case 3 : glutWireSphere(bsphere.getRadius(),10,11); break ;
    case 4 : glutWireSphere(bsphere.getRadius(),11,11); break ;
    case 5 : glutWireSphere(bsphere.getRadius(),12,11); break ;
    case 6 : glutWireSphere(bsphere.getRadius(),10,12); break ;
    case 7 : glutWireSphere(bsphere.getRadius(),11,12); break ;
    case 8 : glutWireSphere(bsphere.getRadius(),12,12); spherebotch=0 ; break ;
  }
#endif

  glTranslatef ( -bsphere.getCenter()[0],
                 -bsphere.getCenter()[1],
                 -bsphere.getCenter()[2] ) ;

  glEnable ( GL_LIGHTING ) ;
}


void ssgEntity::print ( FILE *fd, char *indent, int how_much )
{
  ssgBase::print ( fd, indent, how_much ) ;

  if ( how_much > 1 )
    fprintf ( fd, "%s  Num Parents=%d\n", indent, parents.getNumEntities () ) ;
}


/*
  Get the entity whos name equals match (case sensitive) or 0 if there isn't 
   This will be overridden by descendant classes to search trees, etc.
   This makes recovery of tagged parts of objects simple.
   I envisage it being used to find axles, gimbles etc and to seperate many objects
   from a single loaded model file.  The base version in ssgEntity returns the
   ssgEntity if the name refers to this entity or 0 if not.
*/

ssgEntity* ssgEntity::getByName ( char *match )
{
  return ( getName() != NULL && strcmp ( getName(), match ) == 0 ) ?
                                                this : (ssgEntity *) NULL ;
}


/*
  Get the entity specified by the path.  A path is a series of names
  seperated by '/'.  Each sub-unit is searched so long as the path matches.
  A single leading '/' on the path is ignored.
*/

ssgEntity* ssgEntity::getByPath ( char *path )
{
  /* ignore leading '/' */

  if ( *path == '/' )
    ++path ;

  /* return this if the name is the path else NULL */

  return ( getName() != NULL && strcmp ( getName(), path ) == 0 ) ?
                                                this : (ssgEntity *) NULL ;
}
  

ssgCallback ssgEntity::getCallback ( int cb_type )
{
  if ( isAKindOf ( ssgTypeLeaf() ) )
    return ((ssgLeaf*)this) -> getCallback ( cb_type ) ;

  /*
   *  Because of transparency sorting, having a pre/post draw callback
   *  doesn't make sense for anything but a leaf.
   */

  ulSetError ( UL_WARNING, "getCallback() ignored for non-leaf entity");
 
  return NULL ;
}


void ssgEntity::setCallback ( int cb_type, ssgCallback cb )
{
  if ( isAKindOf ( ssgTypeLeaf() ) )
  {
    ((ssgLeaf*)this) -> setCallback ( cb_type, cb ) ;
  }
  else
  {
    /*
     *  Because of transparency sorting, having a pre/post draw callback
     *  doesn't make sense for anything but a leaf.
     */

    ulSetError ( UL_WARNING, "setCallback() ignored for non-leaf entity");
  }
}


int ssgEntity::preTravTests ( int *test_needed, int which )
{
  if ( (getTraversalMask() & which) == 0 )
  {
    if ( which & SSGTRAV_HOT )
      stats_hot_no_trav ++ ;
    /*
    if ( which & SSGTRAV_LOS )
      stats_hot_no_trav ++ ;*/
    return FALSE ;
  }

  if ( preTravCB != NULL )
  {
    int result = (*preTravCB)(this,which) ;

    if ( result == 0 ) return FALSE ;
    if ( result == 2 ) *test_needed = 0 ;
  }

  return TRUE ;
}


void ssgEntity::postTravTests ( int which )
{
  if ( postTravCB != NULL )
    (*postTravCB)(this,which) ;
}


ssgCullResult ssgEntity::cull_test ( sgFrustum *f, sgMat4 m, int test_needed )
{
  if ( ! test_needed )
    return SSG_INSIDE ;

  stats_cull_test++ ;
  sgSphere tmp = *(getBSphere()) ;

  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  return (ssgCullResult) f -> contains ( &tmp ) ;
}


ssgCullResult ssgEntity::isect_test ( sgSphere *s, sgMat4 m, int test_needed )
{
  if ( ! test_needed )
    return SSG_INSIDE ;

  stats_isect_test++ ;
  sgSphere tmp = *(getBSphere()) ;

  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  /* Check axial distances for trivial reject */

  sgVec3 center_vec ;
  float sum_radii = s->getRadius() + tmp.getRadius() ;

  sgSubVec3 ( center_vec, s->getCenter(), tmp.getCenter() ) ;

  if ( sgAbs(center_vec[0]) > sum_radii ||
       sgAbs(center_vec[1]) > sum_radii ||
       sgAbs(center_vec[2]) > sum_radii )
    return SSG_OUTSIDE ;

  float separation_sqd = sgScalarProductVec3 ( center_vec, center_vec ) ;

  float tmp_radius_sqd = sgSquare ( tmp.getRadius() ) ;
  float sph_radius_sqd = sgSquare ( s ->getRadius() ) ;

  float sum_radii_sqd = tmp_radius_sqd + sph_radius_sqd ;

  if ( separation_sqd >= sum_radii_sqd )
    return SSG_OUTSIDE ;

  if ( separation_sqd + tmp_radius_sqd <= sph_radius_sqd )
    return SSG_INSIDE ;

  return SSG_STRADDLE ;
}


/*
  Places the addresses of the entities whose names or paths
   are listed in bind into the variables whose addresses are
   listed in bind.  The bind array must terminate with a
   NULL name.  Returns TRUE if all names were found and
   bound to variables.
*/

int ssgEntity::bindEntities ( ssgEntityBinding *bind )
{
  int success = TRUE ;

  while ( bind -> nameOrPath != NULL )
  {
    ssgEntity *e = NULL ;

    if ( strchr ( bind -> nameOrPath, '/' ) )
      e = getByPath ( bind -> nameOrPath ) ;
    else
      e = getByName ( bind -> nameOrPath ) ;

    if ( e != NULL )
      *( bind -> entity ) = e ;
    else
      success = FALSE ;

    ++bind ;
  }

  return success ;
}


ssgCullResult ssgEntity::hot_test ( sgVec3 s, sgMat4 m, int test_needed )
{
  /* Height-Off-Terrain Test */
  stats_hot_test++ ;

  if ( !test_needed )
  {
    stats_hot_triv_accept++ ;
    return SSG_INSIDE ;
  }

  sgSphere tmp = *(getBSphere()) ;

  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  float d = sgSquare ( s[0] - tmp.getCenter()[0] ) +
            sgSquare ( s[1] - tmp.getCenter()[1] ) ;

  if ( d > sgSquare ( tmp.getRadius() ) )
  {
    stats_hot_radius_reject++ ;
    return SSG_OUTSIDE ;
  }

  stats_hot_straddle++ ;
  return SSG_STRADDLE ;
}



ssgCullResult ssgEntity::los_test ( sgVec3 s, sgMat4 m, int test_needed )
{
  /* Line of Sight Test
   * Determine whether a line of sight that starts at the origin and proceeds along the
   *  vector "s" intersects the B-sphere of this entity (I think that's right)
   * Algorithm:  (please bear with the vector calculus)
   *  Define the line of sight to be L = O + u S
   *    where "O" is the origin of the line of sight (always (0.0.0) for this application)
   *          "u" is a parametric coordinate which measures distance along the line of sight
   *          "S" is the direction vector, given by the variable "s" in this function
   *  Define the B-sphere as a sphere of radius "r" centered at a point "C".
   *  The vector from the center of the sphere to a point on the line is given by
   *     V = O + u S - C = ( O - C ) + u S
   *  The line of sight intersects the sphere if the shortest length of this vector is less
   *  than the radius of the sphere.  We will work with the square of the length as it is
   *  easier and gives the same result.  This distance-squared from the point on the line to
   *  the center of the sphere is given by the dot product of "V" with itself:
   *     d2 = V dot V = ( O - C ) dot ( O - C ) + 2 u * ( O - C ) dot S + u^2 * S dot S
   *  To get the shortest distance, differentiate "d2" with respect to "u" and set the
   *  derivative to zero:
   *     2 ( O - C ) dot S + 2 u * S dot S = 0
   *     u = - [ ( O - C ) dot S ] / [ S dot S ]
   *  For this value of "u", the distance squared becomes
   *     d2 = ( O - C ) dot ( O - C ) - [ ( O - C ) dot S ] ^2 / [ S dot S ]
   *  If d2 < radius of sphere squared, then the line of sight intersects the sphere.
   */

  stats_los_test++ ;

  if ( !test_needed )
  {
    stats_los_triv_accept++ ;
    return SSG_INSIDE ;
  }

  sgSphere tmp = *(getBSphere()) ;

  /* If there is no B-sphere, the LOS is outside of it by definition */
  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  sgVec3 p1 ; /* End point at eye of Line-Of-Sight */

  sgSetVec3 ( p1, 0.0, 0.0, 0.0 ) ; /* Eye point always at origin */
        
  /* Get the center and square the radius of the sphere  */
  sgFloat const *center = tmp.getCenter () ;
  sgFloat radius2 = sgSquare ( tmp.getRadius () ) ;

  /* Calculate the distance from the line to the center of the sphere  */
  sgVec3 oc ; 
  sgSubVec3 ( oc, p1, center ) ; 

  /* Check whether the eyepoint is within the sphere */
  sgFloat eye_to_center_squared = sgSquare ( oc[0] ) + sgSquare ( oc[1] ) + sgSquare ( oc[2] ) ;
  if ( eye_to_center_squared < radius2 )
  {
    stats_los_straddle++ ;
    return SSG_STRADDLE ;
  }

  /* Calculate the dot product of the vector from the center to the eyepoint and the
   * line of sight vector.  If this dot product is positive or zero, then the point of
   * closest approach to the sphere is behind the eyepoint or at the eyepoint.  Since the
   * eyepoint is outside the sphere, this means that the line of sight forward from the
   * eyepoint will not intersect the sphere.
   */
  sgFloat oc_dot_s = oc[0] * s[0] + oc[1] * s[1] + oc[2] * s[2] ;
  if ( oc_dot_s >= 0.0 )
  {
    stats_los_radius_reject++ ;
    return SSG_OUTSIDE ;
  }

  float dmin2 = eye_to_center_squared - sgSquare ( oc_dot_s ) / 
                                ( sgSquare ( s[0] ) + sgSquare ( s[1] ) + sgSquare ( s[2] ) ) ; 

  /* Compare minimum distance squared with sphere radius squared */
  if ( dmin2 > radius2 ) 
  { 
    stats_los_radius_reject++; 
    return SSG_OUTSIDE; 
  } 

  stats_los_straddle++ ;
  return SSG_STRADDLE ;
}


int ssgEntity::load ( FILE *fd )
{
  bsphere_is_invalid = TRUE ;
  _ssgReadInt    ( fd, &traversal_mask ) ;
  return ssgBase::load(fd) ;
}



int ssgEntity::save ( FILE *fd )
{
  _ssgWriteInt    ( fd, traversal_mask ) ;
  return ssgBase::save(fd) ;
}



void ssgEntity::getNetTransform ( sgMat4 xform )
{
  if ( getNumParents () > 0 )
    getParent ( 0 ) -> getNetTransform ( xform ) ;
  else
    sgMakeIdentMat4 ( xform ) ;
}


void ssgEntity::getLastNetTransform ( sgMat4 xform )
{
  if ( getNumParents () > 0 )
    getParent ( 0 ) -> getLastNetTransform ( xform ) ;
  else
    sgMakeIdentMat4 ( xform ) ;
}



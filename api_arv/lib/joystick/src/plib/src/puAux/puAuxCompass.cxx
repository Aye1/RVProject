/*
     PUI Auxiliary Widget Library
     Derived from PLIB, the Portable Game Library by Steve Baker.
     Copyright (C) 1998,2002,2004  Steve Baker

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

     $Id: puAuxCompass.cxx,v 1.2 2004/02/28 22:35:03 sjbaker Exp $
*/


#include "puAux.h"

#ifdef UL_MAC_OSX
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

UL_RTTI_DEF1(puaCompass,puObject)


/* Function to rotate a vector through a given quaternion using the formula
 * R = Q r Q-1 -- this gives the components of a ROTATED vector in a STATIONARY
 * coordinate system.  We assume that Q is a unit quaternion.
 */
void sgRotateVecQuat ( sgVec3 vec, sgQuat q )
{
  sgVec3 rot ;
  sgFloat qwqw = q[SG_W] * q[SG_W] ;
  sgFloat qwqx = q[SG_W] * q[SG_X] ;
  sgFloat qwqy = q[SG_W] * q[SG_Y] ;
  sgFloat qwqz = q[SG_W] * q[SG_Z] ;
  sgFloat qxqx = q[SG_X] * q[SG_X] ;
  sgFloat qxqy = q[SG_X] * q[SG_Y] ;
  sgFloat qxqz = q[SG_X] * q[SG_Z] ;
  sgFloat qyqy = q[SG_Y] * q[SG_Y] ;
  sgFloat qyqz = q[SG_Y] * q[SG_Z] ;
  sgFloat qzqz = q[SG_Z] * q[SG_Z] ;
  rot[SG_X] = ( qwqw + qxqx - qyqy - qzqz ) * vec[SG_X] + 2.0f * ( qxqy - qwqz ) * vec[SG_Y] + 2.0f * ( qxqz + qwqy ) * vec[SG_Z] ;
  rot[SG_Y] = ( qwqw - qxqx + qyqy - qzqz ) * vec[SG_Y] + 2.0f * ( qyqz - qwqx ) * vec[SG_Z] + 2.0f * ( qxqy + qwqz ) * vec[SG_X] ;
  rot[SG_Z] = ( qwqw - qxqx - qyqy + qzqz ) * vec[SG_Z] + 2.0f * ( qxqz - qwqy ) * vec[SG_X] + 2.0f * ( qyqz + qwqx ) * vec[SG_Y] ;
  sgCopyVec3 ( vec, rot ) ;
}

/* Function to rotate a vector through a given quaternion using the formula
 * R = Q-1 r Q -- this gives the components of a STATIONARY vector in a ROTATED
 * coordinate system.  We assume that Q is a unit quaternion.
 */
void sgRotateCoordQuat ( sgVec3 vec, sgQuat q )
{
  sgVec3 rot ;
  sgFloat qwqw = q[SG_W] * q[SG_W] ;
  sgFloat qwqx = q[SG_W] * q[SG_X] ;
  sgFloat qwqy = q[SG_W] * q[SG_Y] ;
  sgFloat qwqz = q[SG_W] * q[SG_Z] ;
  sgFloat qxqx = q[SG_X] * q[SG_X] ;
  sgFloat qxqy = q[SG_X] * q[SG_Y] ;
  sgFloat qxqz = q[SG_X] * q[SG_Z] ;
  sgFloat qyqy = q[SG_Y] * q[SG_Y] ;
  sgFloat qyqz = q[SG_Y] * q[SG_Z] ;
  sgFloat qzqz = q[SG_Z] * q[SG_Z] ;
  rot[SG_X] = ( qwqw + qxqx - qyqy - qzqz ) * vec[SG_X] + 2.0f * ( qxqy + qwqz ) * vec[SG_Y] + 2.0f * ( qxqz - qwqy ) * vec[SG_Z] ;
  rot[SG_Y] = ( qwqw - qxqx + qyqy - qzqz ) * vec[SG_Y] + 2.0f * ( qyqz + qwqx ) * vec[SG_Z] + 2.0f * ( qxqy - qwqz ) * vec[SG_X] ;
  rot[SG_Z] = ( qwqw - qxqx - qyqy + qzqz ) * vec[SG_Z] + 2.0f * ( qxqz + qwqy ) * vec[SG_X] + 2.0f * ( qyqz - qwqx ) * vec[SG_Y] ;
  sgCopyVec3 ( vec, rot ) ;
}


#define SET_ARC_COLOUR1(value)  \
  if ( getIntegerValue () == value ) \
    glColor4fv ( colour [ PUCOL_LEGEND ] ) ; \
  else \
    glColor4f ( 1.0, 1.0, 1.0, 1.0 )

#define SET_ARC_COLOUR2(value1,value2)  \
  if ( ( getIntegerValue () == value1 ) || ( getIntegerValue () == value2 ) ) \
    glColor4fv ( colour [ PUCOL_LEGEND ] ) ; \
  else \
    glColor4f ( 1.0, 1.0, 1.0, 1.0 )

#define SET_ARC_COLOUR3(value1,value2,value3)  \
  if ( ( getIntegerValue () == value1 ) || ( getIntegerValue () == value2 ) || ( getIntegerValue () == value3 ) ) \
    glColor4fv ( colour [ PUCOL_LEGEND ] ) ; \
  else \
    glColor4f ( 1.0, 1.0, 1.0, 1.0 )

void puaCompass::draw ( int dx, int dy )
{
  if ( !visible || ( window != puGetWindow () ) ) return ;

  // Draw the frame surrounding the compass

//  if ( ( style == PUSTYLE_BEVELLED ) ||
//       ( style == PUSTYLE_SHADED ) )
//    abox.draw ( dx, dy, -PUSTYLE_BOXED, colour, FALSE, 2 ) ;
//  else
//    abox.draw ( dx, dy, -style, colour, FALSE, border_thickness ) ;

  if ( r_cb )
    r_cb ( this, dx, dy, render_data ) ;
  else
  {
    float xsize = float(abox.max[0] - abox.min[0]) ;
    float ysize = float(abox.max[1] - abox.min[1]) ;
    float xcenter = float(dx) + xsize / 2.0f ;
    float ycenter = float(dy) + ysize / 2.0f ;
    float size = ( ( xsize > ysize ) ? ysize : xsize ) / 2.5f ;

    glMatrixMode ( GL_MODELVIEW ) ;
    glPushMatrix () ;
    glLoadIdentity () ;
    glTranslatef ( xcenter, ycenter, 0 ) ;
    glScalef ( size, size, size ) ;

    /* Make the points circular instead of square */
    glEnable ( GL_POINT_SMOOTH ) ;
    glPointSize ( point_size ) ;

    /* Ideally, we would be able to use a "glRotate" to rotate the compass.
     * Unfortunately, PUI widgets are all 2-d and so our z-coordinate will cause
     * the widget to be clipped.  Our solution is to rotate all the compass
     * components into the widget plane and just plot with a z-coordinate of zero.
     */
    glColor4fv ( colour [ PUCOL_LEGEND ] ) ;

    sgVec3 pt ;
    /* The points */
    glBegin ( GL_POINTS ) ;
      SET_ARC_COLOUR1 ( PUACOMPASS_RESET ) ;
      glVertex3f ( 0.0, 0.0, 0.0 ) ;  /* the origin */
      SET_ARC_COLOUR1 ( PUACOMPASS_ROTATE_Y_Z ) ;
      if ( getIntegerValue () != PUACOMPASS_ROTATE_Y_Z )  // Set x-point red for now since point size doesn't seem to work
        glColor4f ( 1.0, 0.0, 0.0, 1.0 ) ;
      sgSetVec3 ( pt, 1.0, 0.0, 0.0 ) ;  /* x-axis point */
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      SET_ARC_COLOUR1 ( PUACOMPASS_ROTATE_Z_X ) ;
      if ( getIntegerValue () != PUACOMPASS_ROTATE_Z_X )  // Set y-point green for now since point size doesn't seem to work
        glColor4f ( 0.0, 1.0, 0.0, 1.0 ) ;
      sgSetVec3 ( pt, 0.0, 1.0, 0.0 ) ;  /* y-axis point */
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      SET_ARC_COLOUR1 ( PUACOMPASS_ROTATE_X_Y ) ;
      if ( getIntegerValue () != PUACOMPASS_ROTATE_X_Y )  // Set z-point blue for now since point size doesn't seem to work
        glColor4f ( 0.0, 0.0, 1.0, 1.0 ) ;
      sgSetVec3 ( pt, 0.0, 0.0, 1.0 ) ;  /* z-axis point */
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
    glEnd () ;

    glLineWidth ( 3 ) ;

    /* The axis lines */
    glBegin ( GL_LINES ) ;
      SET_ARC_COLOUR3 ( PUACOMPASS_TRANSLATE_X, PUACOMPASS_TRANSLATE_X_Y, PUACOMPASS_TRANSLATE_Z_X ) ;
      sgSetVec3 ( pt, 0.0, 0.0, 0.0 ) ;
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      sgSetVec3 ( pt, 1.0, 0.0, 0.0 ) ;  /* x-axis */
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      SET_ARC_COLOUR3 ( PUACOMPASS_TRANSLATE_Y, PUACOMPASS_TRANSLATE_X_Y, PUACOMPASS_TRANSLATE_Y_Z ) ;
      sgSetVec3 ( pt, 0.0, 0.0, 0.0 ) ;
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      sgSetVec3 ( pt, 0.0, 1.0, 0.0 ) ;  /* y-axis */
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      SET_ARC_COLOUR3 ( PUACOMPASS_TRANSLATE_Z, PUACOMPASS_TRANSLATE_Y_Z, PUACOMPASS_TRANSLATE_Z_X ) ;
      sgSetVec3 ( pt, 0.0, 0.0, 0.0 ) ;
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      sgSetVec3 ( pt, 0.0, 0.0, 1.0 ) ;  /* z-axis */
      sgRotateVecQuat ( pt, rotation ) ;
      glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
    glEnd () ;

    /* The quarter-arcs */
    int num_points = 13 ;
    float coords[13] = { .000000f, .130526f, .258819f, .382683f, .500000f, .608761f,
                         .707107f, .793353f, .866025f, .923879f, .965926f, .991445f, 1.0f } ;
    int i ;
    glBegin ( GL_LINE_STRIP ) ;
      SET_ARC_COLOUR2 ( PUACOMPASS_ROTATE_Z, PUACOMPASS_TRANSLATE_X_Y ) ;
      for ( i = 0; i < num_points; i++ )
      {
        sgSetVec3 ( pt, coords[i], coords[num_points-1-i], 0.0 ) ;
        sgRotateVecQuat ( pt, rotation ) ;
        glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      }
    glEnd () ;

    glBegin ( GL_LINE_STRIP ) ;
      SET_ARC_COLOUR2 ( PUACOMPASS_ROTATE_Y, PUACOMPASS_TRANSLATE_Z_X ) ;
      for ( i = 0; i < num_points; i++ )
      {
        sgSetVec3 ( pt, coords[i], 0.0, coords[num_points-1-i] ) ;
        sgRotateVecQuat ( pt, rotation ) ;
        glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      }
    glEnd () ;

    glBegin ( GL_LINE_STRIP ) ;
      SET_ARC_COLOUR2 ( PUACOMPASS_ROTATE_X, PUACOMPASS_TRANSLATE_Y_Z ) ;
      for ( i = 0; i < num_points; i++ )
      {
        sgSetVec3 ( pt, 0.0, coords[i], coords[num_points-1-i] ) ;
        sgRotateVecQuat ( pt, rotation ) ;
        glVertex3f ( pt[SG_X], pt[SG_Y], 0.0f ) ;
      }
    glEnd () ;

    glMatrixMode ( GL_MODELVIEW ) ;
    glPopMatrix () ;

    draw_legend ( dx, dy ) ;
  }

  draw_label ( dx, dy ) ;
}

sgFloat sgDistSquaredToLineLineSegment ( const sgLineSegment3 seg, const sgLine3 line )
{
  /* Convert the line segment to a line.  We will deal with the segment limits later. */
  sgLine3 line2 ;
  sgLineSegment3ToLine3 ( &line2, seg ) ;

  /* Get the dot product of the two direction vectors */
  sgFloat t1_dot_t2 = sgScalarProductVec3 ( line.direction_vector, line2.direction_vector ) ;

  /* If the lines are parallel, distance is just the distance from a point to the other line */
  if ( fabs ( t1_dot_t2 ) >= 1.0 )
    return sgDistSquaredToLineVec3 ( line, seg.a ) ;

  /* Get the parametric coordinates of the closest points on the two lines.  The first line
   * is parameterized by r = r1 + t1 u while the second is parameterized by r = r2 + t2 v.
   * The square of the distance between them is [ ( r1 + t1 u ) - ( r2 + t2 v ) ] dot
   * [ ( r1 + t1 u ) - ( r2 + t2 v ) ].  Differentiating this dot product with respect to
   * u and v and setting the derivatives to zero gives a matrix equation:
   * [ 1         -(t1 dot t2) ] [ u ] = [  ( r1 - r2 ) dot t1 ]
   * [ -(t1 dot t2)         1 ] [ v ]   [ -( r1 - r2 ) dot t2 ]
   * We invert the matrix to get the equations below.
   */
  sgVec3 r1_minus_r2 ;
  sgSubVec3 ( r1_minus_r2, line.point_on_line, line2.point_on_line ) ;

  /* t1_t2_t2_minus_t1 = ( t1 dot t2 ) t2 - t1
   * t2_minus_t1_t2_t1 = t2 - ( t1 dot t2 ) t1
   */
  sgVec3 t1_t2_t2_minus_t1, t2_minus_t1_t2_t1 ;
  sgAddScaledVec3 ( t1_t2_t2_minus_t1, line.direction_vector, line2.direction_vector, -t1_dot_t2 ) ;
  sgNegateVec3 ( t1_t2_t2_minus_t1 ) ;
  sgAddScaledVec3 ( t2_minus_t1_t2_t1, line2.direction_vector, line.direction_vector, -t1_dot_t2 ) ;

  sgFloat u = sgScalarProductVec3 ( r1_minus_r2, t1_t2_t2_minus_t1 ) / ( 1.0f - t1_dot_t2 * t1_dot_t2 ) ;
  sgFloat v = sgScalarProductVec3 ( r1_minus_r2, t2_minus_t1_t2_t1 ) / ( 1.0f - t1_dot_t2 * t1_dot_t2 ) ;

  /* Since line 2 is a line segment, we limit "v" to between 0 and the distance between the points. */
  sgFloat length = sgDistanceVec3 ( seg.a, seg.b ) ;
  if ( v < 0.0 ) v = 0.0 ;
  if ( v > length ) v = length ;

  sgVec3 point1, point2 ;
  sgAddScaledVec3 ( point1, line.point_on_line, line.direction_vector, u ) ;
  sgAddScaledVec3 ( point2, line2.point_on_line, line2.direction_vector, v ) ;
  return sgDistanceSquaredVec3 ( point1, point2 ) ;
}


void puaCompass::doHit ( int button, int updown, int x, int y )
{
  if ( puActiveWidget() && ( this != puActiveWidget() ) )
  {
    puActiveWidget() -> invokeDownCallback () ;
    puDeactivateWidget () ;
  }

  float xsize = float(abox.max[0] - abox.min[0]) ;
  float ysize = float(abox.max[1] - abox.min[1]) ;
  float size = ( ( xsize > ysize ) ? ysize : xsize ) / 2.5f ;

  /* Set the "prev_rotation" quaternion.  This will stop being updated after the downclick.
   * We use it in the transformation so that the transformation "freezes" on the downclick.
   * In the meantime, the actual rotation continues to change and updates the widget "look."
   */
  if ( button_state == PU_UP )
    sgCopyQuat ( prev_rotation, rotation ) ;

  /* The mouse position forms a line in 3-space.  The equation of this line in window
   * coordinates is (xm,ym,0) + u k, where "xm" and "ym" are the mouse position (x and y),
   * "u" is a parametric coordinate along the line, and "k" is the unit vector in the z-
   * direction.  We rotate these into the compass coordinate system to get the mouse click
   * somewhere on a line R = R0 + u T, where R0 is (xm,ym,0) rotated and T is "k" rotated.
   */
  sgLine3 mouse_click_line ;
  mouse_click_line.direction_vector[0] = 2.0f * ( -prev_rotation[SG_W] * prev_rotation[SG_Y] + prev_rotation[SG_X] * prev_rotation[SG_Z] ) ;
  mouse_click_line.direction_vector[1] = 2.0f * (  prev_rotation[SG_W] * prev_rotation[SG_X] + prev_rotation[SG_Y] * prev_rotation[SG_Z] ) ;
  mouse_click_line.direction_vector[2] = prev_rotation[SG_W] * prev_rotation[SG_W] + prev_rotation[SG_Z] * prev_rotation[SG_Z] - prev_rotation[SG_X] * prev_rotation[SG_X] - prev_rotation[SG_Y] * prev_rotation[SG_Y] ;

  float xm = float(x) - xsize / 2.0f ;
  float ym = float(y) - ysize / 2.0f ;
  sgSetVec3 ( mouse_click_line.point_on_line, xm, ym, 0.0f ) ;
  sgRotateCoordQuat ( mouse_click_line.point_on_line, prev_rotation ) ;

  /* If the button state is up (whether moving or down-clicking), find the
   * nearest element in the compass (if any) to activate it.
   */
  if ( button_state == PU_UP )
  {
    /* Set the widget value to zero.  Then perform thirteen tests, each one figuring out whether
     * the mouse is over (or has clicked on) so that we can activate the appropriate part of
     * the widget.
     */
    setValue ( PUACOMPASS_INACTIVE ) ;

    sgVec3 point ;
    /* Minimum distance (squared) from the points (1,0,0), (0,1,0), and (0,0,1) to the mouse click line */
    sgSetVec3 ( point, size, 0.0, 0.0 ) ;
    float dmin2 = sgDistSquaredToLineVec3 ( mouse_click_line, point ) ;
    if ( dmin2 < point_size * point_size )
      setValue ( PUACOMPASS_ROTATE_Y_Z ) ;

    if ( getIntegerValue () == PUACOMPASS_INACTIVE )
    {
      sgSetVec3 ( point, 0.0, size, 0.0 ) ;
      dmin2 = sgDistSquaredToLineVec3 ( mouse_click_line, point ) ;
      if ( dmin2 < point_size * point_size )
        setValue ( PUACOMPASS_ROTATE_Z_X ) ;
    }

    if ( getIntegerValue () == PUACOMPASS_INACTIVE )
    {
      sgSetVec3 ( point, 0.0, 0.0, size ) ;
      dmin2 = sgDistSquaredToLineVec3 ( mouse_click_line, point ) ;
      if ( dmin2 < point_size * point_size )
        setValue ( PUACOMPASS_ROTATE_X_Y ) ;
    }

    /* Minimum distance (squared) from the origin */
    if ( getIntegerValue () == PUACOMPASS_INACTIVE )
    {
      sgSetVec3 ( point, 0.0, 0.0, 0.0 ) ;
      dmin2 = sgDistSquaredToLineVec3 ( mouse_click_line, point ) ;
      if ( dmin2 < point_size * point_size )
        setValue ( PUACOMPASS_RESET ) ;
    }

    /* Minimum distance (squared) from the three axis segments */
    sgLineSegment3 axis ;
    sgSetVec3 ( axis.a, 0.0, 0.0, 0.0 ) ;
    if ( getIntegerValue () == PUACOMPASS_INACTIVE )
    {
      sgSetVec3 ( axis.b, size, 0.0, 0.0 ) ;
      dmin2 = sgDistSquaredToLineLineSegment ( axis, mouse_click_line ) ;
      if ( dmin2 < point_size * point_size )
        setValue ( PUACOMPASS_TRANSLATE_X ) ;
    }

    if ( getIntegerValue () == PUACOMPASS_INACTIVE )
    {
      sgSetVec3 ( axis.b, 0.0, size, 0.0 ) ;
      dmin2 = sgDistSquaredToLineLineSegment ( axis, mouse_click_line ) ;
      if ( dmin2 < point_size * point_size )
        setValue ( PUACOMPASS_TRANSLATE_Y ) ;
    }

    if ( getIntegerValue () == PUACOMPASS_INACTIVE )
    {
      sgSetVec3 ( axis.b, 0.0, 0.0, size ) ;
      dmin2 = sgDistSquaredToLineLineSegment ( axis, mouse_click_line ) ;
      if ( dmin2 < point_size * point_size )
        setValue ( PUACOMPASS_TRANSLATE_Z ) ;
    }

    /* Project the mouse click line on to each of the coordinate planes.  If it intersects
     * within the given radius (size +- point_size) of the origin, highlight the arc.
     */
    sgFloat xydist2 = 0.0 ;
    sgFloat xyplane_xint = 0.0, xyplane_yint = 0.0 ;
    if ( ( getIntegerValue () == PUACOMPASS_INACTIVE ) && ( mouse_click_line.direction_vector[2] != 0.0 ) )
    {
      xyplane_xint = mouse_click_line.point_on_line[0] - mouse_click_line.point_on_line[2] * mouse_click_line.direction_vector[0] / mouse_click_line.direction_vector[2] ;
      xyplane_yint = mouse_click_line.point_on_line[1] - mouse_click_line.point_on_line[2] * mouse_click_line.direction_vector[1] / mouse_click_line.direction_vector[2] ;
      xydist2 = xyplane_xint * xyplane_xint + xyplane_yint * xyplane_yint ;
      if ( ( xyplane_xint > 0.0 ) && ( xyplane_yint > 0.0 ) )
      {
        if ( fabs ( xydist2 - size * size ) < 2.0f * size * point_size )
        {
          setValue ( PUACOMPASS_ROTATE_Z ) ;
          xint = xyplane_xint ;
          yint = xyplane_yint ;
        }
      }
    }

    sgFloat yzdist2 = 0.0 ;
    sgFloat yzplane_yint = 0.0, yzplane_zint = 0.0 ;
    if ( ( getIntegerValue () == PUACOMPASS_INACTIVE ) && ( mouse_click_line.direction_vector[0] != 0.0 ) )
    {
      yzplane_yint = mouse_click_line.point_on_line[1] - mouse_click_line.point_on_line[0] * mouse_click_line.direction_vector[1] / mouse_click_line.direction_vector[0] ;
      yzplane_zint = mouse_click_line.point_on_line[2] - mouse_click_line.point_on_line[0] * mouse_click_line.direction_vector[2] / mouse_click_line.direction_vector[0] ;
      yzdist2 = yzplane_yint * yzplane_yint + yzplane_zint * yzplane_zint ;
      if ( ( yzplane_yint > 0.0 ) && ( yzplane_zint > 0.0 ) )
      {
        if ( fabs ( yzdist2 - size * size ) < 2.0f * size * point_size )
        {
          setValue ( PUACOMPASS_ROTATE_X ) ;
          yint = yzplane_yint ;
          zint = yzplane_zint ;
        }
      }
    }

    sgFloat zxdist2 = 0.0 ;
    sgFloat zxplane_zint = 0.0, zxplane_xint = 0.0 ;
    if ( ( getIntegerValue () == PUACOMPASS_INACTIVE ) && ( mouse_click_line.direction_vector[1] != 0.0 ) )
    {
      zxplane_zint = mouse_click_line.point_on_line[2] - mouse_click_line.point_on_line[1] * mouse_click_line.direction_vector[2] / mouse_click_line.direction_vector[1] ;
      zxplane_xint = mouse_click_line.point_on_line[0] - mouse_click_line.point_on_line[1] * mouse_click_line.direction_vector[0] / mouse_click_line.direction_vector[1] ;
      zxdist2 = zxplane_zint * zxplane_zint + zxplane_xint * zxplane_xint ;
      if ( ( zxplane_zint > 0.0 ) && ( zxplane_xint > 0.0 ) )
      {
        if ( fabs ( zxdist2 - size * size ) < 2.0f * size * point_size )
        {
          setValue ( PUACOMPASS_ROTATE_Y ) ;
          zint = zxplane_zint ;
          xint = zxplane_xint ;
        }
      }
    }

    /* If we missed the arc (meaning that the value is still INACTIVE), check whether we
     * got inside the quadrants.
     */
    if ( ( getIntegerValue () == PUACOMPASS_INACTIVE ) && ( mouse_click_line.direction_vector[2] != 0.0 ) )
    {
      if ( ( xydist2 < size * size ) && ( xyplane_xint > 0.0 ) && ( xyplane_yint > 0.0 ) )
      {
        setValue ( PUACOMPASS_TRANSLATE_X_Y ) ;
        xint = xyplane_xint ;
        yint = xyplane_yint ;
      }
    }

    if ( ( getIntegerValue () == PUACOMPASS_INACTIVE ) && ( mouse_click_line.direction_vector[0] != 0.0 ) )
    {
      if ( ( yzdist2 < size * size ) && ( yzplane_yint > 0.0 ) && ( yzplane_zint > 0.0 ) )
      {
        setValue ( PUACOMPASS_TRANSLATE_Y_Z ) ;
        yint = yzplane_yint ;
        zint = yzplane_zint ;
      }
    }

    if ( ( getIntegerValue () == PUACOMPASS_INACTIVE ) && ( mouse_click_line.direction_vector[1] != 0.0 ) )
    {
      if ( ( zxdist2 < size * size ) && ( zxplane_zint > 0.0 ) && ( zxplane_xint > 0.0 ) )
      {
        setValue ( PUACOMPASS_TRANSLATE_Z_X ) ;
        zint = zxplane_zint ;
        xint = zxplane_xint ;
      }
    }
  }

  if ( button != getTriggerButton () )
    return ;

  if ( updown == PU_DOWN )
  {
    if ( getIntegerValue () == PUACOMPASS_ROTATE_X )
      prev_angle = sgATan2 ( zint, yint ) ;
    else if ( getIntegerValue () == PUACOMPASS_ROTATE_Y )
      prev_angle = sgATan2 ( xint, zint ) ;
    else if ( getIntegerValue () == PUACOMPASS_ROTATE_Z )
      prev_angle = sgATan2 ( yint, xint ) ;

    mouse_x = xm ;
    mouse_y = ym ;
    button_state = PU_DOWN ;

    // If we are rotating about two axes, we need to mouse z-coordinate for future reference.
    if ( ( getIntegerValue () == PUACOMPASS_ROTATE_Y_Z ) ||
         ( getIntegerValue () == PUACOMPASS_ROTATE_Z_X ) ||
         ( getIntegerValue () == PUACOMPASS_ROTATE_X_Y ) )
    {
      sgVec3 temp ;
      sgCopyVec3 ( temp, mouse_click_line.point_on_line ) ;
      // Parametric coordinate of point on line closest to origin
      float u_origin = -sgScalarProductVec3 ( temp, mouse_click_line.direction_vector ) ;
      float u = 0.0 ;

      if ( getIntegerValue () == PUACOMPASS_ROTATE_Y_Z )
      {
        temp[SG_X] -= size ;
        u = -sgScalarProductVec3 ( temp, mouse_click_line.direction_vector ) ;
      }
      else if ( getIntegerValue () == PUACOMPASS_ROTATE_Z_X )
      {
        temp[SG_Y] -= size ;
        u = -sgScalarProductVec3 ( temp, mouse_click_line.direction_vector ) ;
      }
      else if ( getIntegerValue () == PUACOMPASS_ROTATE_X_Y )
      {
        temp[SG_Z] -= size ;
        u = -sgScalarProductVec3 ( temp, mouse_click_line.direction_vector ) ;
      }

      mouse_z = size * size - xm * xm - ym * ym ;
      if ( mouse_z < 0.0 ) mouse_z = 0.0 ;
      mouse_z = sgSqrt ( mouse_z ) ;
      if ( u < u_origin ) mouse_z = -mouse_z ;
    }
  }
  else if ( updown == PU_UP )
  {
    if ( getIntegerValue () == PUACOMPASS_RESET )
    {
      sgSetQuat ( rotation, 1.0f, 0.0f, 0.0f, 0.0f ) ;
      sgSetQuat ( prev_rotation, 1.0f, 0.0f, 0.0f, 0.0f ) ;
      sgSetVec3 ( translation, 0.0f, 0.0f, 0.0f ) ;
      setValue ( PUACOMPASS_INACTIVE ) ;
    }

    button_state = PU_UP ;
  }
  else if ( updown == PU_DRAG )
  {
    float zm = size * size - xm * xm - ym * ym ;
    if ( zm < 0.0 ) zm = 0.0 ;
    zm = ( mouse_z >= 0.0 ) ? sgSqrt ( zm ) : -sgSqrt ( zm ) ;
    float new_size = sgSqrt ( xm * xm + ym * ym + zm * zm ) ;
    float cosang = ( xm * mouse_x + ym * mouse_y + zm * mouse_z ) / ( size * new_size ) ;
    cosang = sgSqrt ( ( 1.0f + cosang ) / 2.0f ) ;

    sgVec3 displacement ;
    sgSetVec3 ( displacement, xm - mouse_x, ym - mouse_y, 0.0f ) ;
    sgRotateVecQuat ( displacement, prev_rotation ) ;

    sgQuat rot ;
    sgSetQuat ( rot, 2.0f, 2.0f, 2.0f, 2.0f ) ;  /* Values will change if we have a rotation */

    sgFloat angle = prev_angle ;

    switch ( getIntegerValue () )
    {
    case PUACOMPASS_INACTIVE         :
      break ;
    case PUACOMPASS_RESET            :
      break ;
    case PUACOMPASS_ROTATE_Y_Z       :
//      rot[SG_X] =  0.0 ;
//      rot[SG_Y] = -displacement[2] / size ;
//      rot[SG_Z] =  displacement[1] / size ;
//      rot[SG_W] =  sgSqrt ( 1.0f - rot[SG_Y] * rot[SG_Y] - rot[SG_Z] * rot[SG_Z] ) ;
//      sgPostMultQuat ( rotation, rot ) ;
//      break ;
    case PUACOMPASS_ROTATE_Z_X       :
//      rot[SG_X] =  displacement[2] / size ;
//      rot[SG_Y] =  0.0 ;
//      rot[SG_Z] = -displacement[0] / size ;
//      rot[SG_W] =  sgSqrt ( 1.0f - rot[SG_X] * rot[SG_X] - rot[SG_Z] * rot[SG_Z] ) ;
//      sgPostMultQuat ( rotation, rot ) ;
//      break ;
    case PUACOMPASS_ROTATE_X_Y       :
//      rot[SG_X] = -displacement[1] / size ;
//      rot[SG_Y] =  displacement[0] / size ;
//      rot[SG_Z] =  0.0 ;
//      rot[SG_W] =  sgSqrt ( 1.0f - rot[SG_X] * rot[SG_X] - rot[SG_Y] * rot[SG_Y] ) ;
      rot[SG_W] = cosang ;
      rot[SG_X] = ( mouse_y * zm - mouse_z * ym ) * 0.5f / ( size * new_size * cosang ) ;
      rot[SG_Y] = ( mouse_z * xm - mouse_x * zm ) * 0.5f / ( size * new_size * cosang ) ;
      rot[SG_Z] = ( mouse_x * ym - mouse_y * xm ) * 0.5f / ( size * new_size * cosang ) ;
      sgRotateVecQuat ( rot, prev_rotation ) ;
      printf ( "Rotation  %f %f %f %f %f %f %f %f\n", prev_angle, xm, ym, zm, rot[SG_W], rot[SG_X], rot[SG_Y], rot[SG_Z] ) ;
      sgPreMultQuat ( rotation, rot ) ;
      break ;
    case PUACOMPASS_TRANSLATE_X      :
      translation[0] += translation_sensitivity * displacement[0] ;
      break ;
    case PUACOMPASS_TRANSLATE_Y      :
      translation[1] += translation_sensitivity * displacement[1] ;
      break ;
    case PUACOMPASS_TRANSLATE_Z      :
      translation[2] += translation_sensitivity * displacement[2] ;
      break ;
    case PUACOMPASS_ROTATE_Z         :
      xint = mouse_click_line.point_on_line[0] - mouse_click_line.point_on_line[2] * mouse_click_line.direction_vector[0] / mouse_click_line.direction_vector[2] ;
      yint = mouse_click_line.point_on_line[1] - mouse_click_line.point_on_line[2] * mouse_click_line.direction_vector[1] / mouse_click_line.direction_vector[2] ;
      angle = sgATan2 ( yint, xint ) - prev_angle ;
      rot[SG_X] = 0.0 ;
      rot[SG_Y] = 0.0 ;
      rot[SG_Z] = sgSin ( angle / 2.0f ) ;
      rot[SG_W] = sgCos ( angle / 2.0f ) ;
      sgPostMultQuat ( rotation, rot ) ;
      prev_angle += angle ;
      break ;
    case PUACOMPASS_ROTATE_X         :
      yint = mouse_click_line.point_on_line[1] - mouse_click_line.point_on_line[0] * mouse_click_line.direction_vector[1] / mouse_click_line.direction_vector[0] ;
      zint = mouse_click_line.point_on_line[2] - mouse_click_line.point_on_line[0] * mouse_click_line.direction_vector[2] / mouse_click_line.direction_vector[0] ;
      angle = sgATan2 ( zint, yint ) - prev_angle ;
      rot[SG_X] = sgSin ( angle / 2.0f ) ;
      rot[SG_Y] = 0.0 ;
      rot[SG_Z] = 0.0 ;
      rot[SG_W] = sgCos ( angle / 2.0f ) ;
      sgPostMultQuat ( rotation, rot ) ;
      prev_angle += angle ;
      break ;
    case PUACOMPASS_ROTATE_Y         :
      zint = mouse_click_line.point_on_line[2] - mouse_click_line.point_on_line[1] * mouse_click_line.direction_vector[2] / mouse_click_line.direction_vector[1] ;
      xint = mouse_click_line.point_on_line[0] - mouse_click_line.point_on_line[1] * mouse_click_line.direction_vector[0] / mouse_click_line.direction_vector[1] ;
      angle = sgATan2 ( xint, zint ) - prev_angle ;
      rot[SG_X] = 0.0 ;
      rot[SG_Y] = sgSin ( angle / 2.0f ) ;
      rot[SG_Z] = 0.0 ;
      rot[SG_W] = sgCos ( angle / 2.0f ) ;
      sgPostMultQuat ( rotation, rot ) ;
      prev_angle += angle ;
      break ;
    case PUACOMPASS_TRANSLATE_X_Y    :
      translation[0] += translation_sensitivity * displacement[0] ;
      translation[1] += translation_sensitivity * displacement[1] ;
      break ;
    case PUACOMPASS_TRANSLATE_Y_Z    :
      translation[1] += translation_sensitivity * displacement[1] ;
      translation[2] += translation_sensitivity * displacement[2] ;
      break ;
    case PUACOMPASS_TRANSLATE_Z_X    :
      translation[2] += translation_sensitivity * displacement[2] ;
      translation[0] += translation_sensitivity * displacement[0] ;
      break ;
    }

    mouse_x = xm * size / new_size ;
    mouse_y = ym * size / new_size ;
    mouse_z = zm * size / new_size ;
//    if ( rot[0] < 1.5 )
//    {
//      sgPostMultQuat ( rotation, rot ) ;
//      prev_angle += angle ;
//      printf ( "Rotation  %f %f %f %f %f\n", prev_angle, rotation[SG_W], rotation[SG_X], rotation[SG_Y], rotation[SG_Z] ) ;
//    }
  }
}

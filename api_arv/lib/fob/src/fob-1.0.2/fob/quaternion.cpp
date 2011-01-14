/*
-------------------------------------------------------------------------------
libfob - C++ interface to Ascension Technology Corporation's
         Flock of Birds position and orientation measurement system.
Copyright (C) 2002 Nathan Cournia

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------
*/

//FILE:         quaternion.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include <cassert>
#include "fob/quaternion.h"

//////////////////////////////////////////////////////////////////////////
const math::quaternion math::quaternion::IDENTITY( 
	0.0, 0.0, 0.0, //xyz
	1.0 //w
);

//////////////////////////////////////////////////////////////////////////
void
math::quaternion::get_rotation_matrix( math::matrix4& m ) const
{
	real_t x2 = m_v.x( ) + m_v.x( );
	real_t y2 = m_v.y( ) + m_v.y( );
	real_t z2 = m_v.z( ) + m_v.z( );

	real_t xx = m_v.x( ) * x2;
	real_t xy = m_v.x( ) * y2;
	real_t xz = m_v.x( ) * z2;
	real_t xw = x2 * m_w;

	real_t yy = m_v.y( ) * y2;
	real_t yz = m_v.y( ) * z2;
	real_t yw = y2 * m_w;

	real_t zz = m_v.z( ) * z2;
	real_t zw = z2 * m_w;

	m.set( 0, 0, 1.0 - ( yy + zz ) );
	m.set( 0, 1, ( xy - zw ) );
	m.set( 0, 2, ( xz + yw ) );
	m.set( 0, 3, 0.0 );

	m.set( 1, 0, ( xy + zw ) );
	m.set( 1, 1, 1.0 - ( xx + zz ) );
	m.set( 1, 2, ( yz - xw ) );
	m.set( 1, 3, 0.0 );

	m.set( 2, 0, ( xz - yw ) );
	m.set( 2, 1, ( yz + xw ) );
	m.set( 2, 2, 1.0 - ( xx + yy ) );
	m.set( 2, 3, 0.0 );

	m.set( 3, 0, 0.0 );
	m.set( 3, 1, 0.0 );
	m.set( 3, 2, 0.0 );
	m.set( 3, 3, 1.0 );
}

//////////////////////////////////////////////////////////////////////////
math::matrix4
math::quaternion::get_rotation_matrix( void ) const
{
	math::matrix4 m;
	get_rotation_matrix( m );
	return m;
}

//////////////////////////////////////////////////////////////////////////
void
math::quaternion::get_transposed_rotation_matrix( math::matrix4& m ) const
{
	real_t x2 = m_v.x( ) + m_v.x( );
	real_t y2 = m_v.y( ) + m_v.y( );
	real_t z2 = m_v.z( ) + m_v.z( );

	real_t xx = m_v.x( ) * x2;
	real_t xy = m_v.x( ) * y2;
	real_t xz = m_v.x( ) * z2;
	real_t xw = x2 * m_w;

	real_t yy = m_v.y( ) * y2;
	real_t yz = m_v.y( ) * z2;
	real_t yw = y2 * m_w;

	real_t zz = m_v.z( ) * z2;
	real_t zw = z2 * m_w;

	m.set( 0, 0, 1.0 - ( yy + zz ) );
	m.set( 0, 1, ( xy + zw ) );
	m.set( 0, 2, ( xz - yw ) );
	m.set( 0, 3, 0.0 );

	m.set( 1, 0, ( xy - zw ) );
	m.set( 1, 1, 1.0 - ( xx + zz ) );
	m.set( 1, 2, ( yz + xw ) );
	m.set( 1, 3, 0.0 );

	m.set( 2, 0, ( xz + yw ) );
	m.set( 2, 1, ( yz - xw ) );
	m.set( 2, 2, 1.0 - ( xx + yy ) );
	m.set( 2, 3, 0.0 );

	m.set( 3, 0, 0.0 );
	m.set( 3, 1, 0.0 );
	m.set( 3, 2, 0.0 );
	m.set( 3, 3, 1.0 );
}

//////////////////////////////////////////////////////////////////////////
math::matrix4
math::quaternion::get_transposed_rotation_matrix( void ) const
{
	math::matrix4 m;
	get_transposed_rotation_matrix( m );
	return m;
}

//////////////////////////////////////////////////////////////////////////
//not communative
math::quaternion
math::quaternion::operator* ( const math::quaternion& rhs ) const
{
	return math::quaternion( 
		rhs.m_w * m_v.x() + rhs.m_v.x() * m_w + 
			rhs.m_v.z() * m_v.y() - rhs.m_v.y() * m_v.z(), //x
		rhs.m_w * m_v.y() + rhs.m_v.y() * m_w + 
			rhs.m_v.x() * m_v.z() - rhs.m_v.z() * m_v.x(), //y
		rhs.m_w * m_v.z() + rhs.m_v.z() * m_w + 
			rhs.m_v.y() * m_v.x() - rhs.m_v.x() * m_v.y(),  //z
		rhs.m_w * m_w - rhs.m_v.x() * m_v.x() - 
			rhs.m_v.y() * m_v.y() - rhs.m_v.z() * m_v.z() //w
		);
}

//////////////////////////////////////////////////////////////////////////
math::vector3 
math::quaternion::operator* ( const math::vector3& rhs ) const
{
	math::matrix4 rotation;
	get_rotation_matrix( rotation );
	return rotation * rhs;
}
	

//////////////////////////////////////////////////////////////////////////
void
math::quaternion::from_angle_axis( real_t radians, const math::vector3& axis )
{
	//axis should be normalized
	assert( math::equals( axis.length( ), 1.0 ) );

	radians *= 0.5;
	math::multiply( m_v, axis, sin( radians ) );
	m_w = cos( radians );
}
	
//////////////////////////////////////////////////////////////////////////
void
math::quaternion::get_angle_axis( real_t& radians, math::vector3& axis ) const
{
	//math::quaternion should be normalized
	assert( math::equals( magnitude( ), 1.0 ) );

	//angle
	radians = acos( m_w ) * 2.0;

	//axis
	real_t sin_a = sqrt( 1.0 - m_w * m_w );

	//make sure we don't divide by zero
	if( math::equals( sin_a, 0.0 ) ) {
		sin_a = 1.0;
	}

	//set the axis
	axis.set( 
		m_v.x( ) / sin_a, 
		m_v.y( ) / sin_a, 
		m_v.z( ) / sin_a 
	);
}

//////////////////////////////////////////////////////////////////////////
//this method can probably be optimized
void
math::quaternion::from_angles( real_t x_rad, real_t y_rad, 
	real_t z_rad )
{
	math::quaternion q1, q2;
	q1.from_angle_axis( x_rad, math::vector3::X_AXIS );
	q2.from_angle_axis( y_rad, math::vector3::Y_AXIS );
	q1 = q1 * q2;
	q2.from_angle_axis( z_rad, math::vector3::Z_AXIS );
	q1 = q1 * q2;
	q1.normalize( ); //needed?
	*this = q1;
}

//////////////////////////////////////////////////////////////////////////
//modified from matrix faq
//http://www.j3d.org/matrix_faq/matrfaq_latest.html
//assumes 3x3 matrix in row major format
void
math::quaternion::from_matrix3( const real_t *mat )
{
	//find trace
	real_t s;
	real_t t = 1.0 + mat[ 0 ] + mat[ 4 ] + mat[ 8 ];
	if( t > 0.000001 ) {
		//perform "instant" calc (this happens most of the time?)
		s = sqrt( t ) * 2.0;
		m_v.x( (mat[ 7 ] - mat[ 5 ]) / s );
		m_v.y( (mat[ 2 ] - mat[ 6 ]) / s );
		m_v.z( (mat[ 3 ] - mat[ 1 ]) / s );
		m_w = 0.25 * s;
		return;
	}

	if( mat[ 0 ] > mat[ 4 ] && mat[ 0 ] > mat[ 8 ] ) {
		s  = sqrt( 1.0 + mat[ 0 ] - mat[ 4 ] - mat[ 8 ] ) * 2.0;
		m_v.x( 0.25 * s );
		m_v.y( (mat[ 3 ] + mat[ 1 ]) / s );
		m_v.z( (mat[ 2 ] + mat[ 6 ]) / s );
		m_w = (mat[ 7 ] - mat[ 5 ]) / s;
	} else if( mat[ 4 ] > mat[ 8 ] ) {
		s  = sqrt( 1.0 + mat[ 4 ] - mat[ 0 ] - mat[ 8 ]) * 2;
		m_v.x( (mat[ 3 ] + mat[ 1 ]) / s );
		m_v.y( 0.25 * s );
		m_v.z( (mat[ 7 ] + mat[ 5 ]) / s );
		m_w = (mat[ 2 ] - mat[ 6 ]) / s;
	} else {
		s  = sqrt( 1.0 + mat[ 8 ] - mat[ 0 ] - mat[ 4 ]) * 2;
		m_v.x( (mat[ 2 ] + mat[ 6 ]) / s );
		m_v.y( (mat[ 7 ] + mat[ 5 ]) / s );
		m_v.z(  0.25 * s );
		m_w = (mat[ 3 ] - mat[ 1 ]) / s;
	}
}

//////////////////////////////////////////////////////////////////////////
//modified from matrix faq
//http://www.j3d.org/matrix_faq/matrfaq_latest.html
//assumes 4x4 matrix in row major format
void
math::quaternion::from_matrix4( const real_t *mat )
{
	//find trace
	real_t s;
	real_t t = 1.0 + mat[ 0 ] + mat[ 5 ] + mat[ 10 ];
	if( t > 0.000001 ) {
		//perform "instant" calc (this happens most of the time?)
		s = sqrt( t ) * 2.0;
		m_v.x( (mat[ 9 ] - mat[ 6 ]) / s );
		m_v.y( (mat[ 2 ] - mat[ 8 ]) / s );
		m_v.z( (mat[ 4 ] - mat[ 1 ]) / s );
		m_w = 0.25 * s;
		return;
	}

	if( mat[ 0 ] > mat[ 5 ] && mat[ 0 ] > mat[ 10 ] ) {
		s  = sqrt( 1.0 + mat[ 0 ] - mat[ 5 ] - mat[ 10 ] ) * 2.0;
		m_v.x( 0.25 * s );
		m_v.y( (mat[ 4 ] + mat[ 1 ]) / s );
		m_v.z( (mat[ 2 ] + mat[ 8 ]) / s );
		m_w = (mat[ 9 ] - mat[ 6 ]) / s;
	} else if( mat[ 5 ] > mat[ 10 ] ) {
		s  = sqrt( 1.0 + mat[ 5 ] - mat[ 0 ] - mat[ 10 ]) * 2;
		m_v.x( (mat[ 4 ] + mat[ 1 ]) / s );
		m_v.y( 0.25 * s );
		m_v.z( (mat[ 9 ] + mat[ 6 ]) / s );
		m_w = (mat[ 2 ] - mat[ 8 ]) / s;
	} else {
		s  = sqrt( 1.0 + mat[ 10 ] - mat[ 0 ] - mat[ 5 ]) * 2;
		m_v.x( (mat[ 2 ] + mat[ 8 ]) / s );
		m_v.y( (mat[ 9 ] + mat[ 6 ]) / s );
		m_v.z(  0.25 * s );
		m_w = (mat[ 4 ] - mat[ 1 ]) / s;
	}
}

//////////////////////////////////////////////////////////////////////////
//modified from http://www.magic-software.com/Documentation/quat.pdf
math::quaternion
math::quaternion::slerp( real_t percent, const math::quaternion& qa, 
	math::quaternion qb )
{
	//quick check to see if quats are the same
	if( math::equals( qa.w( ), qb.w( ) ) && qa.v( ) == qb.v( ) ) {
		return qa;
	}

	real_t cos_a = qa.w( ) * qb.w( ) + math::dot( qa.v( ), qb.v( ) );
	if( cos_a < 0.0 ) {
		qb.m_v.negate( );
		qb.m_w *= -1.0;
		cos_a = -cos_a;
	}
		
	//set default scales to lerp
	real_t a = 1.0 - percent;
	real_t b = percent;

	//is angle great enough to do real slerp?
	if( 1 - cos_a > 0.1 ) {
		real_t angle = acos( cos_a );
		real_t inverse_sin = 1.0 / sin( angle );
		a = sin( (1.0 - percent) * angle ) * inverse_sin;
		b = sin( percent * angle) * inverse_sin;
	}
	
	return math::quaternion( 
		a * qa.x( ) + b * qb.x( ),
		a * qa.y( ) + b * qb.y( ),
		a * qa.z( ) + b * qb.z( ),
		a * qa.w( ) + b * qb.w( )
	);
}

//////////////////////////////////////////////////////////////////////////
std::ostream& 
operator<< ( std::ostream& o, const math::quaternion& q )
{
	return o << q.m_v << " " << q.m_w;
}

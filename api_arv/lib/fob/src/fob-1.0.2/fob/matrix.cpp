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

//FILE:         matrix.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include <iomanip>
#include "fob/matrix.h"

//////////////////////////////////////////////////////////////////////////
const math::matrix4 math::matrix4::IDENTITY(
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
);

//////////////////////////////////////////////////////////////////////////
const math::matrix4 math::matrix4::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0
);

//////////////////////////////////////////////////////////////////////////
math::matrix4::matrix4( void )
{ }

//////////////////////////////////////////////////////////////////////////
math::matrix4::matrix4( 
	real_t d00, real_t d01, real_t d02, 
	real_t d10, real_t d11, real_t d12, 
	real_t d20, real_t d21, real_t d22 )
{
	m_data[ 0 ][ 0 ] = d00;
	m_data[ 0 ][ 1 ] = d01;
	m_data[ 0 ][ 2 ] = d02;
	m_data[ 0 ][ 3 ] = 0.0;

	m_data[ 1 ][ 0 ] = d10;
	m_data[ 1 ][ 1 ] = d11;
	m_data[ 1 ][ 2 ] = d12;
	m_data[ 1 ][ 3 ] = 0.0;

	m_data[ 2 ][ 0 ] = d20;
	m_data[ 2 ][ 1 ] = d21;
	m_data[ 2 ][ 2 ] = d22;
	m_data[ 2 ][ 3 ] = 0.0;

	m_data[ 3 ][ 0 ] = 0.0;
	m_data[ 3 ][ 1 ] = 0.0;
	m_data[ 3 ][ 2 ] = 0.0;
	m_data[ 3 ][ 3 ] = 1.0;
}

//////////////////////////////////////////////////////////////////////////
math::matrix4::matrix4(
	real_t d00, real_t d01, real_t d02, real_t d03, 
	real_t d10, real_t d11, real_t d12, real_t d13,
	real_t d20, real_t d21, real_t d22, real_t d23,
	real_t d30, real_t d31, real_t d32, real_t d33 )
{
	m_data[ 0 ][ 0 ] = d00;
	m_data[ 0 ][ 1 ] = d01;
	m_data[ 0 ][ 2 ] = d02;
	m_data[ 0 ][ 3 ] = d03;

	m_data[ 1 ][ 0 ] = d10;
	m_data[ 1 ][ 1 ] = d11;
	m_data[ 1 ][ 2 ] = d12;
	m_data[ 1 ][ 3 ] = d13;

	m_data[ 2 ][ 0 ] = d20;
	m_data[ 2 ][ 1 ] = d21;
	m_data[ 2 ][ 2 ] = d22;
	m_data[ 2 ][ 3 ] = d23;

	m_data[ 3 ][ 0 ] = d30;
	m_data[ 3 ][ 1 ] = d31;
	m_data[ 3 ][ 2 ] = d32;
	m_data[ 3 ][ 3 ] = d33;
}

//////////////////////////////////////////////////////////////////////////
math::matrix4::matrix4( const math::matrix4& m )
{
	for( unsigned int r = 0; r < 4; ++r ) {
		for( unsigned int c = 0; c < 4; ++c ) {
			m_data[ r ][ c ] = m.m_data[ r ][ c ];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
math::matrix4&
math::matrix4::operator= ( const math::matrix4& m )
{
	for( unsigned int r = 0; r < 4; ++r ) {
		for( unsigned int c = 0; c < 4; ++c ) {
			m_data[ r ][ c ] = m.m_data[ r ][ c ];
		}
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////
void
math::matrix4::set( 
	real_t d00, real_t d01, real_t d02, 
	real_t d10, real_t d11, real_t d12, 
	real_t d20, real_t d21, real_t d22 )
{
	m_data[ 0 ][ 0 ] = d00;
	m_data[ 0 ][ 1 ] = d01;
	m_data[ 0 ][ 2 ] = d02;
	m_data[ 0 ][ 3 ] = 0.0;

	m_data[ 1 ][ 0 ] = d10;
	m_data[ 1 ][ 1 ] = d11;
	m_data[ 1 ][ 2 ] = d12;
	m_data[ 1 ][ 3 ] = 0.0;

	m_data[ 2 ][ 0 ] = d20;
	m_data[ 2 ][ 1 ] = d21;
	m_data[ 2 ][ 2 ] = d22;
	m_data[ 2 ][ 3 ] = 0.0;

	m_data[ 3 ][ 0 ] = 0.0;
	m_data[ 3 ][ 1 ] = 0.0;
	m_data[ 3 ][ 2 ] = 0.0;
	m_data[ 3 ][ 3 ] = 1.0;
}

//////////////////////////////////////////////////////////////////////////
void
math::matrix4::set(
	real_t d00, real_t d01, real_t d02, real_t d03, 
	real_t d10, real_t d11, real_t d12, real_t d13,
	real_t d20, real_t d21, real_t d22, real_t d23,
	real_t d30, real_t d31, real_t d32, real_t d33 )
{
	m_data[ 0 ][ 0 ] = d00;
	m_data[ 0 ][ 1 ] = d01;
	m_data[ 0 ][ 2 ] = d02;
	m_data[ 0 ][ 3 ] = d03;

	m_data[ 1 ][ 0 ] = d10;
	m_data[ 1 ][ 1 ] = d11;
	m_data[ 1 ][ 2 ] = d12;
	m_data[ 1 ][ 3 ] = d13;

	m_data[ 2 ][ 0 ] = d20;
	m_data[ 2 ][ 1 ] = d21;
	m_data[ 2 ][ 2 ] = d22;
	m_data[ 2 ][ 3 ] = d23;

	m_data[ 3 ][ 0 ] = d30;
	m_data[ 3 ][ 1 ] = d31;
	m_data[ 3 ][ 2 ] = d32;
	m_data[ 3 ][ 3 ] = d33;
}

//////////////////////////////////////////////////////////////////////////
math::vector3
math::matrix4::operator* ( const math::vector3& v ) const
{
	math::vector3 res;
	for( unsigned int i = 0; i < 3; ++i ) {
		res( i ) = 
			m_data[ i ][ 0 ] * v.x( ) +
			m_data[ i ][ 1 ] * v.y( ) +
			m_data[ i ][ 2 ] * v.z( ) +
			m_data[ i ][ 3 ];
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////
math::matrix4
math::matrix4::operator*( const math::matrix4& rhs ) const
{
	math::matrix4 dest;
	for( unsigned int r = 0; r < 4; ++r ) {
		for( unsigned int c = 0; c < 4; ++c ) {
			dest.m_data[ r ][c ] = 
				m_data[ r ][ 0 ] * rhs.m_data[ 0 ][ c ] +
				m_data[ r ][ 1 ] * rhs.m_data[ 1 ][ c ] +
				m_data[ r ][ 2 ] * rhs.m_data[ 2 ][ c ] +
				m_data[ r ][ 3 ] * rhs.m_data[ 3 ][ c ];
		}
	}
	return dest;
}

//////////////////////////////////////////////////////////////////////////
math::matrix4
math::matrix4::operator+( const math::matrix4& rhs ) const
{
	math::matrix4 dest;
	for( unsigned int r = 0; r < 4; ++r ) {
		for( unsigned int c = 0; c < 4; ++c ) {
			dest.m_data[ r ][ c ] = m_data[ r ][ c ] + rhs.m_data[ r ][ c ];
		}
	}
	return dest;
}

//////////////////////////////////////////////////////////////////////////
math::matrix4
math::matrix4::operator-( const math::matrix4& rhs ) const
{
	math::matrix4 dest;
	for( unsigned int r = 0; r < 4; ++r ) {
		for( unsigned int c = 0; c < 4; ++c ) {
			dest.m_data[ r ][c ] = m_data[ r ][ c ] - rhs.m_data[ r ][ c ];
		}
	}
	return dest;
}

//////////////////////////////////////////////////////////////////////////
void
math::matrix4::transpose( void )
{
	for( unsigned int r = 0; r < 4; ++r ) {
		for( unsigned int c = r; c < 4; ++c ) {
			math::swap( m_data[ r ][ c ], m_data[ c ][ r ] );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
math::vector3
operator* ( const math::vector3& v, const math::matrix4& m )
{
	math::vector3 res;
	for( unsigned int i = 0; i < 3; ++i ) {
		res( i ) = 
			m.m_data[ 0 ][ i ] * v.x( ) +
			m.m_data[ 1 ][ i ] * v.y( ) +
			m.m_data[ 2 ][ i ] * v.z( ) +
			m.m_data[ 3 ][ i ];
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////
void
math::matrix4::from_angle_axis( real_t radians, const math::vector3& axis )
{
	real_t rcos = cos( radians );
	real_t rsin = sin( radians );
	real_t xy = axis.x( ) * axis.y( );
	real_t xz = axis.x( ) * axis.z( );
	real_t yz = axis.y( ) * axis.z( );
	real_t t = 1.0 - rcos;
	
	m_data[ 0 ][ 0 ] = rcos + axis.x( ) * axis.x( ) * t;
	m_data[ 0 ][ 1 ] = -axis.z( ) * rsin + xy * t;
	m_data[ 0 ][ 2 ] = axis.y( ) * rsin + xz * t;
	m_data[ 0 ][ 3 ] = 0.0;

	m_data[ 1 ][ 0 ] = axis.z( ) * rsin + xy * t;
	m_data[ 1 ][ 1 ] = rcos + axis.y( ) * axis.y( ) * t;
	m_data[ 1 ][ 2 ] = -axis.x( ) * rsin + yz * t;
	m_data[ 1 ][ 3 ] = 0.0;

	m_data[ 2 ][ 0 ] = -axis.y( ) * rsin + xz * t;
	m_data[ 2 ][ 1 ] = axis.x( ) * rsin + yz * t;
	m_data[ 2 ][ 2 ] = rcos + axis.z( ) * axis.z( ) * t;
	m_data[ 2 ][ 3 ] = 0.0;

	m_data[ 3 ][ 0 ] = 0.0;
	m_data[ 3 ][ 1 ] = 0.0;
	m_data[ 3 ][ 2 ] = 0.0;
	m_data[ 3 ][ 3 ] = 1.0;
}

//////////////////////////////////////////////////////////////////////////
//assumes an inverse exist
math::matrix4
math::matrix4::get_inverted_rotation( void ) const
{
	math::matrix4 inv( IDENTITY );
	inv.m_data[ 0 ][ 0 ] = m_data[ 1 ][ 1 ] * m_data[ 2 ][ 2 ] -
		m_data[ 1 ][ 2 ] * m_data[ 2 ][ 1 ];
	inv.m_data[ 0 ][ 1 ] = m_data[ 0 ][ 2 ] * m_data[ 2 ][ 1 ] -
		m_data[ 0 ][ 1 ] * m_data[ 2 ][ 2 ];
	inv.m_data[ 0 ][ 2 ] = m_data[ 0 ][ 1 ] * m_data[ 1 ][ 2 ] -
		m_data[ 0 ][ 2 ] * m_data[ 1 ][ 1 ];
	inv.m_data[ 1 ][ 0 ] = m_data[ 1 ][ 2 ] * m_data[ 2 ][ 0 ] -
		m_data[ 1 ][ 0 ] * m_data[ 2 ][ 2 ];
	inv.m_data[ 1 ][ 1 ] = m_data[ 0 ][ 0 ] * m_data[ 2 ][ 2 ] -
		m_data[ 0 ][ 2 ] * m_data[ 2 ][ 0 ];
	inv.m_data[ 1 ][ 2 ] = m_data[ 0 ][ 2 ] * m_data[ 1 ][ 0 ] -
		m_data[ 0 ][ 0 ] * m_data[ 1 ][ 2 ];
	inv.m_data[ 2 ][ 0 ] = m_data[ 1 ][ 0 ] * m_data[ 2 ][ 1 ] -
		m_data[ 1 ][ 1 ] * m_data[ 2 ][ 0 ];
	inv.m_data[ 2 ][ 1 ] = m_data[ 0 ][ 1 ] * m_data[ 2 ][ 0 ] -
		m_data[ 0 ][ 0 ] * m_data[ 2 ][ 1 ];
	inv.m_data[ 2 ][ 2 ] = m_data[ 0 ][ 0 ] * m_data[ 1 ][ 1 ] -
		m_data[ 0 ][ 1 ] * m_data[ 1 ][ 0 ];

	real_t det = m_data[ 0 ][ 0 ] * inv.m_data[ 0 ][ 0 ] +
		m_data[ 0 ][ 1 ] * inv.m_data[ 1 ][ 0 ] +
		m_data[ 0 ][ 2 ] * inv.m_data[ 2 ][ 0 ];

	if( math::equals( det, 0.0 ) ) {
		//fuck, no inverse
		return IDENTITY;
	}

	det = 1.0 / det;
	for( unsigned int r = 0; r < 3; ++r ) {
		for( unsigned int c = 0; c < 3; ++c ) {
			inv.m_data[ r ][ c ] *= det;
		}
	}

	return inv;
}

//////////////////////////////////////////////////////////////////////////
//from matrix/quaterion faq
math::vector3 
math::matrix4::get_radians( void )
{
	real_t x, y, z, c, rx, ry;
	
	y = asin( m_data[ 0 ][ 2 ] );
	c = cos( y );

	if( fabs( c ) > 0.005 ) {
		rx = m_data[ 2 ][ 2 ] / c;
		ry = -m_data[ 1 ][ 2 ] / c;
		
		x = atan2( ry, rx );

		rx = m_data[ 0 ][ 0 ] / c;
		ry = -m_data[ 0 ][ 1 ] / c;

		z = atan2( ry, rx );
	} else {
		//gimball lock
		x = 0;
		rx = m_data[ 1 ][ 1 ];
		ry = m_data[ 1 ][ 0 ];
		z = atan2( ry, rx );
	}

	return math::vector3( x, y, z );
}

//////////////////////////////////////////////////////////////////////////
math::vector3 
math::matrix4::get_degrees( void )
{
	math::vector3 degs = get_radians( );
	degs *= (180.0 / M_PI);
	degs( 0 ) = math::angle_normalize_360( degs( 0 ) );
	degs( 1 ) = math::angle_normalize_360( degs( 1 ) );
	degs( 2 ) = math::angle_normalize_360( degs( 2 ) );
	return degs;
}

//////////////////////////////////////////////////////////////////////////
void 
math::matrix4::from_matrix3( const real_t *mat )
{
	set( 
		mat[ 0 ], mat[ 1 ], mat[ 2 ],
		mat[ 3 ], mat[ 4 ], mat[ 5 ],
		mat[ 6 ], mat[ 7 ], mat[ 8 ]
	);
}

//////////////////////////////////////////////////////////////////////////
void 
math::matrix4::from_matrix4( const real_t *mat )
{
	set( 
		mat[ 0 ], mat[ 1 ], mat[ 2 ], mat[ 3 ],
		mat[ 4 ], mat[ 5 ], mat[ 6 ], mat[ 7 ],
		mat[ 8 ], mat[ 9 ], mat[ 10 ], mat[ 11 ],
		mat[ 12 ], mat[ 13 ], mat[ 14 ], mat[ 15 ]
	);
}


//////////////////////////////////////////////////////////////////////////
std::ostream&
operator<< ( std::ostream& o, const math::matrix4& m )
{
	for( unsigned int r = 0; r < 3; ++r ) {
		o << std::setw( 5 ) << m.m_data[ r ][ 0 ] << " "
			<< std::setw( 5 ) << m.m_data[ r ][ 1 ] << " "
			<< std::setw( 5 ) << m.m_data[ r ][ 2 ] << " " 
			<< std::setw( 5 ) << m.m_data[ r ][ 3 ] << " "
			<< std::endl;
	}
	
	//for the last line do not put a line return
	o << std::setw( 5 ) << m.m_data[ 3 ][ 0 ] << " "
			<< std::setw( 5 ) << m.m_data[ 3 ][ 1 ] << " "
			<< std::setw( 5 ) << m.m_data[ 3 ][ 2 ] << " " 
			<< std::setw( 5 ) << m.m_data[ 3 ][ 3 ] << " ";
	return o;
}

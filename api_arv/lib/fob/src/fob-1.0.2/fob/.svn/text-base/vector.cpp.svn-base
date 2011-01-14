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

//FILE:         vector.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include "fob/vector.h"

//////////////////////////////////////////////////////////////////////////
const math::vector3 math::vector3::ZERO( 0.0, 0.0, 0.0 );
const math::vector3 math::vector3::X_AXIS( 1.0, 0.0, 0.0 );
const math::vector3 math::vector3::Y_AXIS( 0.0, 1.0, 0.0 );
const math::vector3 math::vector3::Z_AXIS( 0.0, 0.0, 1.0 );

//////////////////////////////////////////////////////////////////////////
math::vector3 
operator* ( real_t lhs, const math::vector3& rhs )
{
	return math::vector3( (rhs.m_data[ 0 ] * lhs), 
		(rhs.m_data[ 1 ] * lhs),
		(rhs.m_data[ 2 ] * lhs)
	);
}

//////////////////////////////////////////////////////////////////////////
std::ostream& 
operator<< ( std::ostream& o, const math::vector3& vec )
{
	return o << vec.m_data[ 0 ] << " " << vec.m_data[ 1 ] 
		<< " " << vec.m_data[ 2 ];
}


//////////////////////////////////////////////////////////////////////////
math::vector3 
math::cross( const math::vector3& u, const math::vector3& v )
{
	return math::vector3( 
		u.m_data[ 1 ] * v.m_data[ 2 ] - u.m_data[ 2 ] * v.m_data[ 1 ],
		u.m_data[ 2 ] * v.m_data[ 0 ] - u.m_data[ 0 ] * v.m_data[ 2 ],
		u.m_data[ 0 ] * v.m_data[ 1 ] - u.m_data[ 1 ] * v.m_data[ 0 ] 
	);
}

//////////////////////////////////////////////////////////////////////////
void 
math::add( math::vector3& dest, const math::vector3& lhs, 
	const math::vector3& rhs )
{
	dest.m_data[ 0 ] = lhs.m_data[ 0 ] + rhs.m_data[ 0 ];
	dest.m_data[ 1 ] = lhs.m_data[ 1 ] + rhs.m_data[ 1 ];
	dest.m_data[ 2 ] = lhs.m_data[ 2 ] + rhs.m_data[ 2 ];
}

//////////////////////////////////////////////////////////////////////////
void 
math::subtract( math::vector3& dest, const math::vector3& lhs, 
	const math::vector3& rhs )
{
	dest.m_data[ 0 ] = lhs.m_data[ 0 ] - rhs.m_data[ 0 ];
	dest.m_data[ 1 ] = lhs.m_data[ 1 ] - rhs.m_data[ 1 ];
	dest.m_data[ 2 ] = lhs.m_data[ 2 ] - rhs.m_data[ 2 ];
}

//////////////////////////////////////////////////////////////////////////
void 
math::multiply( math::vector3& dest, const math::vector3& lhs, 
	const math::vector3& rhs )
{
	dest.m_data[ 0 ] = lhs.m_data[ 0 ] * rhs.m_data[ 0 ];
	dest.m_data[ 1 ] = lhs.m_data[ 1 ] * rhs.m_data[ 1 ];
	dest.m_data[ 2 ] = lhs.m_data[ 2 ] * rhs.m_data[ 2 ];
}

//////////////////////////////////////////////////////////////////////////
void 
math::divide( math::vector3& dest, const math::vector3& lhs, 
	const math::vector3& rhs )
{
	dest.m_data[ 0 ] = lhs.m_data[ 0 ] / rhs.m_data[ 0 ];
	dest.m_data[ 1 ] = lhs.m_data[ 1 ] / rhs.m_data[ 1 ];
	dest.m_data[ 2 ] = lhs.m_data[ 2 ] / rhs.m_data[ 2 ];
}

//////////////////////////////////////////////////////////////////////////
void 
math::multiply( math::vector3& dest, const math::vector3& lhs, real_t rhs )
{
	dest.m_data[ 0 ] = lhs.m_data[ 0 ] * rhs;
	dest.m_data[ 1 ] = lhs.m_data[ 1 ] * rhs;
	dest.m_data[ 2 ] = lhs.m_data[ 2 ] * rhs;
}

//////////////////////////////////////////////////////////////////////////
void 
math::multiply( math::vector3& dest, real_t lhs, const math::vector3& rhs )
{
	dest.m_data[ 0 ] = rhs.m_data[ 0 ] * lhs;
	dest.m_data[ 1 ] = rhs.m_data[ 1 ] * lhs;
	dest.m_data[ 2 ] = rhs.m_data[ 2 ] * lhs;
}

//////////////////////////////////////////////////////////////////////////
real_t 
math::dot( const math::vector3& lhs, const math::vector3& rhs )
{
	return ( 
		(lhs.m_data[ 0 ] * rhs.m_data[ 0 ]) +
		(lhs.m_data[ 1 ] * rhs.m_data[ 1 ]) +
		(lhs.m_data[ 2 ] * rhs.m_data[ 2 ])
	);
}

//////////////////////////////////////////////////////////////////////////
math::vector3
math::vector3::lerp( real_t percent, const math::vector3& a, 
	const math::vector3& b )
{
	return a + percent * (b - a);
}

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

//FILE:         mathematics.h
//AUTHOR:       Nathan Cournia <nathan@cournia.com>
/*!
 * \file mathematics.h
 * \brief Math related functions, macros, and symbols.
 */

#ifndef COURNIA_MATHEMATICS_H
#define COURNIA_MATHEMATICS_H 1 

//! Real number type.
typedef float real_t;

#include <cstdlib>
#include <cmath>

//#define MATH_EPSILON 1.19209290e-07f
//! Precision error symbol.
#define MATH_EPSILON 0.00001

//!Math related functions and classes.
namespace math {

//!Returns a random number between 0.0 and 1.0.
inline double
rand01( void ) 
{
	return (rand( ) / (double)RAND_MAX);
}

//!Converts radians to degrees.
/*!
 * \param x \c double in radians
 * \returns \a x converted to degrees.
 */
inline double
to_degrees( double x )
{
	return (x * 180.0 / M_PI);
}

//!Converts degrees to radians.
/*!
 * \param x \c double in degrees
 * \returns \a x converted to radians.
 */
inline double
to_radians( double x )
{
	return (x * M_PI / 180.0);
}

//!Precision aware equals.
/*!
 * Returns \c true if \a x is equal to \a y.  To account for precision errors,
 * \c true is returned if \a x differs from \a y by less than ::MATH_EPSILON.
 */
inline bool
equals( double x, double y ) 
{
	return (std::fabs( x - y ) < MATH_EPSILON);
}

//!Swaps \a x and \a y.
/*!
 * Templated swap function.  Assumes the following:
 * <ul>
 *   <li>type has copy constructor
 *   <li>the assignment operator is defined for type
 * </ul>
 * \note std::swap() works just as well.  This function is included here to be backwards compatible with old code.
 */
template <class T>
inline void
swap( T& x, T& y )
{
	T z = x;
	x = y;
	y = z;
}

//!Finds the sign of \a x.
/*!
 * \returns -1.0 if \a x is less than 0.0, 1.0 otherwise.
 */
inline double
sign( double x )
{
	if( x < 0.0 ) {
		return -1.0;
	} else {
		return 1.0;
	}
}

//!Checks if \a x and \a y share the same sign.
/*!
 * \returns \c true if \a x and \a y share the same sign.
 */
inline bool
same_sign( double x, double y )
{
	if( x * y >= 0.0 ) {
		return true;
	} else {
		return false;
	}
}

//!Check if \a x is negative.
/*!
 * \returns \c true if \a x is less than 0.0.
 */
inline bool
negative( double x )
{
	if( x < 0.0 ) {
		return true;
	} else {
		return false;
	}
}

//!Check if \a x is positive.
/*!
 * \returns \c true if \a x is greater than or equal to 0.0.
 */
inline bool
positive( double x )
{
	if( x < 0.0 ) {
		return false;
	} else {
		return true;
	}
}

//! Returns a normalized angle [0,360).
//Ripped from Quake 3 Mod SDK code/game/q_math.c.
//Seems to be pretty common on the net, so this can probably be considered
//public domain code.
inline float
angle_normalize_360( float angle )
{
	return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535);
}

//! Returns a normalized angle (-180,180].
inline float
angle_normalize_180( float angle )
{
	angle = angle_normalize_360( angle );
	if( angle > 180.0 ) {
		angle -= 360.0;
	}
	return angle;
}

//! Returns the difference of 2 angles.  Result is in the range (-180,180]
inline float
angle_subtract( float a, float b )
{
	return angle_normalize_180( a - b );
}


//! Clamp the value to the specified range.
/*
\param value The value to clamp.
\param min The smallest allowable value.
\param max The largest allowable value.
\return The clamped value.
*/
template <class T>
inline T 
clamp( T value, T min, T max )
{
	if( value < min ) {
		//value less, clamp
		return min;
	}

	if( value > max ) {
		//value greater, clamp
		return max;
	}

	//value ok
	return value;
}

} //end of namespace math

#endif

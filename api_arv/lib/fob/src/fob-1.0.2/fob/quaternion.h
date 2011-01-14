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

//FILE:         quaternion.h
//AUTHOR:       Nathan Cournia <nathan@cournia.com>
//NOTE:         Not by any means a complete implementaion.

#ifndef COURNIA_QUATERNION_H
#define COURNIA_QUATERNION_H 1

#include <iostream>
#include <cmath>
#include "fob/vector.h"
#include "fob/matrix.h"

namespace math {
	class quaternion;
}

std::ostream& operator<< ( std::ostream& o, const math::quaternion& q );

//!A quaternion.
/*!
 * Quaternions take on the form:
 * \f[(x,y,z,w)\f]
 * \note When sending the quaternion to OpenGL, use get_transposed_matrix().
 */
class math::quaternion {
private:
	math::vector3 m_v;
	real_t m_w;

	//! \c stream output operator.
	/*!
	 * Outputs quaternion as \a x \a y \a z \a w.
	 */
	friend std::ostream& operator<< ( std::ostream& o, const math::quaternion& vec );

public:
	//! Default Constructor
	/*! 
	 * Sets quaternion to \f$(0,0,0,1)\f$.
	 */
	quaternion( void ): m_w( 1.0 ) 
	{ }

	//! Constructor to set \a x \a y \a z \a w.
	/*!
	 * \param v_ \c vector which defines the quaternion's \a x \a y \a z.
	 * \param w_ \c real_t which define the quaternion's \a w. 
	 */
	quaternion( const math::vector3& v_, real_t w_ ): m_v( v_ ), m_w( w_ ) 
	{ }

	//! Constructor to set a x \a y \a z \a w.
	/*!
	 * \param x_ \c real_t which defines the quaternion's \a x.
	 * \param y_ \c real_t which defines the quaternion's \a y.
	 * \param z_ \c real_t which defines the quaternion's \a z.
	 * \param w_ \c real_t which defines the quaternion's \a w.
	 */
	quaternion( real_t x_, real_t y_, real_t z_, real_t w_ ):
		m_v( x_, y_, z_ ), m_w( w_ ) 
	{ }

	//! Copy Constructor.
	quaternion( const math::quaternion& p ): m_v( p.m_v ), m_w( p.m_w )  
	{ }

	//! Assignment Operator.
	inline math::quaternion& operator= ( const math::quaternion& p ) {
		m_v = p.m_v;
		m_w = p.m_w;
		return *this;
	}

	//!Returns a copy of \a w.
	inline real_t w( void ) const { 
		return m_w; 
	}

	//!Returns a copy of \a x.
	inline real_t x( void ) const { 
		return m_v.x( ); 
	}

	//!Returns a copy of \a y.
	inline real_t y( void ) const { 
		return m_v.y( ); 
	}

	//!Returns a copy of \a z.
	inline real_t z( void ) const { 
		return m_v.z( ); 
	}

	//!Returns a copy of \a x \a y \a z.
	/*!
	 * \sa vec
	 * \returns A \c vector containing \a x \a y \a z.
	 */
	inline math::vector3 v( void ) const { 
		return m_v; 
	}

	//!Returns a copy of \a x \a y \a z.
	/*!
	 * \sa v
	 * \returns A \c vector containing \a x \a y \a z.
	 */
	inline math::vector3 vec( void ) const { 
		return m_v; 
	}

	//!Returns a copy of \a w.
	/*!
	 * \sa w
	 */
	inline real_t scalar( void ) const { 
		return m_w; 
	}

	//! Sets the quaternion.
	/*!
	 * \param x_ \c real_t which defines the quaternion's \a x.
	 * \param y_ \c real_t which defines the quaternion's \a y.
	 * \param z_ \c real_t which defines the quaternion's \a z.
	 * \param w_ \c real_t which defines the quaternion's \a w.
	 */
	inline void set( real_t x_, real_t y_, real_t z_, real_t w_ ) {
		m_v.set( x_, y_, z_ );
		m_w = w_;
	}

	//! Sets the quaternion.
	/*!
	 * \param v_ \c vector which defines the quaternion's \a x \a y \a z.
	 * \param w_ \c real_t which define the quaternion's \a w. 
	 */
	inline void
	set( const math::vector3& v_,  real_t w_ ) {
		m_v = v_;
		m_w = w_;
	}

	//! Returns the conjugate of the quaternion.
	/*!
	 * The conjugate of the quaternion is defined as:
	 * \f[(x,y,z,-w)\f]
	 * \note Do data within quaternion is modified.  A \em copy of the quaternion is returned.
	 */
	inline math::quaternion conjugate( void ) const {
		return math::quaternion( -1.0 * m_v, m_w );
	}

	//! Returns the inverse of the quaternion.
	/*!
	 * The inverse of a normalized quaternion is defined as:
	 * \f[(x,y,z,-w)\f]
	 * \note Assumes the quaternion is normalized.  No normalization check is performed.
	 */
	inline math::quaternion operator! ( void ) const {
		return math::quaternion( -1.0 * m_v, m_w );
	}

	//! Returns the magnitude of the quaternion.
	/*!
	 * The magnitude of the quaternion is defined as:
	 * \f[\sqrt{x^2+y^2+z^2+w^2}\f]
	 */
	inline double
	magnitude( void ) const {
		return sqrt( (double)(m_w * m_w + math::dot( m_v, m_v )) );
	}

	//! Normalizes the quaternion.
	/*!
	 * \warn If the quaternion has zero magnitude, a divide by zero may be possible.
	 */
	inline void
	normalize( void ) {
		double mag = magnitude( );
		m_w /= mag;
		m_v.x( m_v.x( ) / mag );
		m_v.y( m_v.y( ) / mag );
		m_v.z( m_v.z( ) / mag );
	}

	//! Sets the quaternion to the identity quaternion.
	/*!
	 * The identity quaternion is defined as:
	 * \f[(0,0,0,1)\f]
	 */
	void
	set_identity( void ) {
		m_w = 1.0;
		m_v.set( 0.0, 0.0, 0.0 );
	}

	//! Returns the rotation matrix that describes this quaternion.
	/*!
	 * \returns A \c matrix4 that describes the rotation this quaternion represents.
	 */
	math::matrix4
	get_rotation_matrix( void ) const;

	//! Sets a rotation matrix that describes this quaternion.
	/*!
	 * \param m A \c matrix4 where the rotation matrix will be written.
	 */
	void
	get_rotation_matrix( math::matrix4& m ) const;

	//! Returns the transposed rotation matrix that describes this quaternion.
	/*!
	 * \note Use this method to send the quaternion to OpenGL functions such as glMultMatrix().
	 * \returns A transposed \c matrix4 that describes the rotation this quaternion represents.
	 */
	math::matrix4
	get_transposed_rotation_matrix( void ) const;

	//! Sets transposed rotation matrix that describes this quaternion.
	/*!
	 * \param m A \c matrix4 where the rotation matrix will be written.
	 * \note Use this method to send the quaternion to OpenGL functions such as glMultMatrix().
	 */
	void
	get_transposed_rotation_matrix( math::matrix4& m ) const;

	//!Quaternion Multiplication
	math::quaternion 
	operator* ( const math::quaternion& rhs ) const;

	//!Quaternion Vector Multiplication
	/*!
	 * Multiplies the matrix that this quaternion describes by 
	 * the vector \a rhs.
	 */
	math::vector3 
	operator* ( const math::vector3& rhs ) const;

	//! Creates a rotation matrix given around an arbitrary axis.
	/*! 
	 * Given an axis and an angle, this method will create a set this quaternion
	 * as the quaternion that describes the rotation around the axis
	 * by angle.
	 * \note Rotations are right-handed.
	 * \param radians A real number in radians.
	 * \param axis A unit length vector describing the axis to rotate around.
	 */
	void
	from_angle_axis( real_t radians, const math::vector3& axis );

	//! Returns the angle and axis the quaternion describes.
	/*! 
	 * \param radians The angle about \a axis in radians (output).
	 * \param axis The axis the quaternion's rotation is aroiund (output).
	 * \returns Sets \a radians and \a axis.
	 */
	void
	get_angle_axis( real_t& radians, math::vector3& axis ) const;

	//! Creates a unit length quaternion from three Euler angles.
	/*!
	 * \param x_rad Rotation around x axis in radians.
	 * \param y_rad Rotation around y axis in radians.
	 * \param z_rad Rotation around z axis in radians.
	 */
	void
	from_angles( real_t x_rad, real_t y_rad, real_t z_rad );

	//! Creates a unit length quaternion from a 3x3 rotation matrix.
	/*!
	 * \param mat Pointer to an \a real_t[ 9 ] representing a rotation matrix.  Matrix is assumed to be stored in row major format.
	 */
	void
	from_matrix3( const real_t *mat );

	//! Creates a unit length quaternion from a 4x4 rotation matrix.
	/*!
	 * \param mat Pointer to an \a real_t[ 16 ] representing a rotation matrix.  Matrix is assumed to be stored in row major format.
	 */
	void
	from_matrix4( const real_t *mat );

	//! Spherical Linear Interpretation
	/*!
	 * Interprets the between the two rotations that \a qa and \a qb describe.
	 * \param percent A \a real_t between [0-1] descibing how much to interpret between \a qa and \a qb.
	 */
	static math::quaternion
	slerp( real_t percent, const math::quaternion& qa, 
		math::quaternion qb );

	
	//! The identity quaternion.
	/*!
	 * The identity quaternion is described as:
	 * \f[(0,0,0,1)\f]
	 */
	static const math::quaternion IDENTITY;
}; //end of class math::quaternion

#endif

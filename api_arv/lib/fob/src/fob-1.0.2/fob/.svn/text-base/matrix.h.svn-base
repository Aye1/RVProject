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

//FILE:         matrix.h
//AUTHOR:       Nathan Cournia <nathan@cournia.com>
//NOTE:         Data is stoed in row major format.  OpenGL expects column
//              major data.  Use transpose( ) to set data suitable for
//              sending to OpenGL's glMultMatrix( ).

#ifndef COURNIA_MATRIX4_H
#define COURNIA_MATRIX4_H 1

#include <iostream>
#include <cassert>
#include "fob/vector.h"
#include "fob/mathematics.h"

namespace math {
	class matrix4;
}

math::vector3 operator* ( const math::vector3& v, const math::matrix4& m );
std::ostream& operator<< ( std::ostream& o, const math::matrix4& m );

//! A 4x4 matrix.
/*! 
 * A 4x4 matrix in row major format:
 * \f[
 * \left[
 * \begin{array}{rrrr}
 * a_{00}&a_{01}&a_{02}&a_{03}\\
 * a_{10}&a_{11}&a_{12}&a_{13}\\
 * a_{20}&a_{21}&a_{22}&a_{23}\\
 * a_{30}&a_{31}&a_{32}&a_{33}
 * \end{array}
 * \right]
 * \f]
 * Where \f$a_{row,col}\f$
 * \note Some graphics libraries like OpenGL expect matrices in column major format.  To convert the matirx to column major format, call transpose() before sending the matrix to OpenGL functions such as glMultMatrix()
 */
class math::matrix4
{
protected:
	real_t m_data[ 4 ][ 4 ]; //!< Actual matrix data.

	//!Vector Matrix Mulitplication
	/*! Multiplies the vector \a v by the matrix \a m.  The operation is
	 * defined as:
	 * \f[
	 * =
	 * \left[
	 * \begin{array}{rrrr}
	 * x&y&z&1\\
	 * \end{array}
	 * \right]
	 * \left[
	 * \begin{array}{rrrr}
	 * a_{00}&a_{01}&a_{02}&a_{03}\\
	 * a_{10}&a_{11}&a_{12}&a_{13}\\
	 * a_{20}&a_{21}&a_{22}&a_{23}\\
	 * a_{30}&a_{31}&a_{32}&a_{33}
	 * \end{array}
	 * \right]
	 * \f]
	 */
	friend math::vector3 operator* ( const math::vector3& v, const math::matrix4& m );

	//! \c stream output operator.
	friend std::ostream& operator<< ( std::ostream& o, const math::matrix4& m );

public:
	//!Default Constructor.
	matrix4( void );

	//!Rotation Set Constructor
	/*! 
	 * Sets the matrix as follows:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * d_{00}&d_{01}&d_{02}&0\\
	 * d_{10}&d_{11}&d_{12}&0\\
	 * d_{20}&d_{21}&d_{22}&0\\
	 * 0&0&0&1
	 * \end{array}
	 * \right]
	 * \f]
	 */
	matrix4( 
		real_t d00, real_t d01, real_t d02, 
		real_t d10, real_t d11, real_t d12, 
		real_t d20, real_t d21, real_t d22 
	);

	//! Full Matrix Constructor
	/*!
	 * Sets the matrix as follows:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * d_{00}&d_{01}&d_{02}&d_{03}\\
	 * d_{10}&d_{11}&d_{12}&d_{13}\\
	 * d_{20}&d_{21}&d_{22}&d_{23}\\
	 * d_{30}&d_{31}&d_{32}&d_{33}
	 * \end{array}
	 * \right]
	 * \f]
	 */
	matrix4( 
		real_t d00, real_t d01, real_t d02, real_t d03, 
		real_t d10, real_t d11, real_t d12, real_t d13,
		real_t d20, real_t d21, real_t d22, real_t d23,
		real_t d30, real_t d31, real_t d32, real_t d33
	);

	//! Copy Constructor.
	matrix4( const math::matrix4& m );

	//! Destructor
	virtual ~matrix4( void ) { }

	//! Assignment Operator.
	math::matrix4& operator= ( const math::matrix4 &m );

	//////////////////////////////////////////////////////////////////////
	//! Cast operator to \c real_t*.
	/*! \returns A read-only pointer to the internal matrix data.
	 */
	operator const real_t* ( void ) const {
		return static_cast<const real_t*>(m_data[ 0 ]);
	}

	//!Sets rotation part of matrix.
	/*! 
	 * Sets the matrix as follows:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * d_{00}&d_{01}&d_{02}&0\\
	 * d_{10}&d_{11}&d_{12}&0\\
	 * d_{20}&d_{21}&d_{22}&0\\
	 * 0&0&0&1
	 * \end{array}
	 * \right]
	 * \f]
	 */
	void set( 
		real_t d00, real_t d01, real_t d02, 
		real_t d10, real_t d11, real_t d12, 
		real_t d20, real_t d21, real_t d22 
	);

	//! Sets the matrix.
	/*!
	 * Sets the matrix as follows:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * d_{00}&d_{01}&d_{02}&d_{03}\\
	 * d_{10}&d_{11}&d_{12}&d_{13}\\
	 * d_{20}&d_{21}&d_{22}&d_{23}\\
	 * d_{30}&d_{31}&d_{32}&d_{33}
	 * \end{array}
	 * \right]
	 * \f]
	 */
	void set( 
		real_t d00, real_t d01, real_t d02, real_t d03, 
		real_t d10, real_t d11, real_t d12, real_t d13,
		real_t d20, real_t d21, real_t d22, real_t d23,
		real_t d30, real_t d31, real_t d32, real_t d33
	);

	//!Set the element at \a row, \a col to \a x.
	/*!
	 * \note abort( ) may be called if \a row or \a col is not one of
	 * 0, 1, 2, or 3.
	 */
	inline void set( unsigned int row, unsigned int col, real_t x ) {
		assert( (row < 4) && (col < 4) );
		m_data[ row ][ col ] = x;
	}

	//!Get the element at \a row, \a col.
	/*!
	 * \note abort( ) may be called if \a row or \a col is not one of
	 * 0, 1, 2, or 3.
	 */
	inline real_t operator()( unsigned int row, unsigned int col ) const {
		assert( (row < 4) && (col < 4) );
		return m_data[ row ][ col ];
	}

	//!Return a reference to the element at \a row, \a col.
	/*!
	 * \note abort( ) may be called if \a row or \a col is not one of
	 * 0, 1, 2, or 3.
	 */
	inline real_t& operator()( unsigned int row, unsigned int col ) {
		assert( (row < 4) && (col < 4) );
		return m_data[ row ][ col ];
	}

	//!Matrix Vector Mulitplication
	/*! Multiplies the  matrix \a m by vector \a v.  The operation is
	 * defined as:
	 * \f[
	 * =
	 * \left[
	 * \begin{array}{rrrr}
	 * a_{00}&a_{01}&a_{02}&a_{03}\\
	 * a_{10}&a_{11}&a_{12}&a_{13}\\
	 * a_{20}&a_{21}&a_{22}&a_{23}\\
	 * a_{30}&a_{31}&a_{32}&a_{33}
	 * \end{array}
	 * \right]
	 * \left[
	 * \begin{array}{r}
	 * x\\
	 * y\\
	 * z\\
	 * 1\\
	 * \end{array}
	 * \right]
	 * \f]
	 */
	math::vector3 operator* ( const math::vector3& v ) const;

	//! Matrix Multiplication
	math::matrix4 operator* ( const math::matrix4& rhs ) const;

	//! Matrix Addition
	math::matrix4 operator+ ( const math::matrix4& rhs ) const;

	//! Matrix Subtraction
	math::matrix4 operator- ( const math::matrix4& rhs ) const;

	//! Transposes the matrix.
	/*!
	 * Sets the matrix \f$M\f$ to \f$M^T\f$.
	 */
	void transpose( void );

	//! Sets the matirx to the identity matrix.
	/*!
	 * Sets the matrix to:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * 1&0&0&0\\
	 * 0&1&0&0\\
	 * 0&0&1&0\\
	 * 0&0&0&1
	 * \end{array}
	 * \right]
	 * \f]
	 */
	inline void set_identity( ) {
		*this = math::matrix4::IDENTITY;
	}

	//! Creates a rotation matrix given around an arbitrary axis.
	/*! 
	 * Given an axis and an angle, this method will create and set this matrix
	 * as the rotation matrix that describes the rotation around the axis
	 * by angle.
	 * \note Rotations are right-handed.
	 * \param radians A real number in radians.
	 * \param axis A unit length vector describing the axis to rotate around.
	 */
	void from_angle_axis( real_t radians, const math::vector3& axis );
	
	//! Computes the inverse of the rotation matrix.
	/*! Inverts the rotation part of this matrix.
	 * \note All translation, scaling, etc. info is lost in the matrix.  This method only inverts the \em rotation part of the matrix.
	 * \returns A new matrix containing the inverted rotation of this matrix.
	 */
	math::matrix4 get_inverted_rotation( void ) const;

	//! Sets the translation part of the matrix.
	/*! Sets the matrix as follows:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * d_{00}&d_{01}&d_{02}&x\\
	 * d_{10}&d_{11}&d_{12}&y\\
	 * d_{20}&d_{21}&d_{22}&z\\
	 * d_{30}&d_{31}&d_{32}&d_{33}
	 * \end{array}
	 * \right]
	 * \f]
	 * \note This method does not \em add to existing translations. This method \em sets the translation part of the matrix.
	 * \param trans A vector describing the new translation.
	 */
	inline void set_translation( const math::vector3& trans ) {
		m_data[ 0 ][ 3 ] = trans.x( );
		m_data[ 1 ][ 3 ] = trans.y( );
		m_data[ 2 ][ 3 ] = trans.z( );
	}

	//! Returns a vector describing the translation part of the matrix.
	inline math::vector3 get_translation( void ) const {
		return math::vector3( m_data[ 0 ][ 3 ],
			m_data[ 1 ][ 3 ],
			m_data[ 2 ][ 3 ]
		);
	}

	math::vector3 get_radians( void );

	math::vector3 get_degrees( void );
	
	//! Sets the matrix from a 3x3 rotation matrix.
	/*!
	 * \param mat Pointer to an \a real_t[ 9 ] representing a rotation matrix.  Matrix is assumed to be stored in row major format.
	 */
	void from_matrix3( const real_t *mat );

	//! Sets the matrix from a 4x4 rotation matrix.
	/*!
	 * \param mat Pointer to an \a real_t[ 16 ] representing a matrix.  Matrix is assumed to be stored in row major format.
	 */
	void from_matrix4( const real_t *mat );

	//! The identity matrix.
	/*! The identity matrix is defined as:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * 1&0&0&0\\
	 * 0&1&0&0\\
	 * 0&0&1&0\\
	 * 0&0&0&1
	 * \end{array}
	 * \right]
	 * \f]
	 */
	static const math::matrix4 IDENTITY;

	//! The zero matrix.
	/*! The zero matrix is defined as:
	 * \f[
	 * \left[
	 * \begin{array}{rrrr}
	 * 0&0&0&0\\
	 * 0&0&0&0\\
	 * 0&0&0&0\\
	 * 0&0&0&0
	 * \end{array}
	 * \right]
	 * \f]
	 */
	static const math::matrix4 ZERO;
};

#endif

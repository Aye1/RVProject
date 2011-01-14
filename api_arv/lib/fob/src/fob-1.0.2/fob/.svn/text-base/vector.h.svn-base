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

//FILE:         vector.h
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#ifndef COURNIA_VECTOR_H
#define COURNIA_VECTOR_H 1

#include <iostream>
#include <cassert>
#include "fob/mathematics.h"

//////////////////////////////////////////////////////////////////////////
namespace math {
	class vector3;
}

//////////////////////////////////////////////////////////////////////////
math::vector3 operator* ( real_t lhs, const math::vector3& rhs );
std::ostream& operator<< ( std::ostream& o, const math::vector3& vec );

namespace math {
	math::vector3 cross( const math::vector3& lhs, const math::vector3& rhs );
	void add( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );
	void subtract( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );
	void multiply( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );
	void divide( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );
	void multiply( math::vector3& dest, const math::vector3& lhs, real_t rhs );
	void multiply( math::vector3& dest, real_t rhs, const math::vector3& lhs );
	real_t dot( const math::vector3& lhs, const math::vector3& rhs ); 
}

//!A 3-dimensional vector.
class math::vector3
{
private:
	real_t m_data[ 3 ]; //!< Stores the x, y, and z components of the vector.

	//!Scalar Multiplication
	/*! 
	 * Returns the 
	 * <a href="http://mathworld.wolfram.com/ScalarMultiplication.html">scalar product</a>
	 * of \a lhs and \a rhs.
	 * \sa math::vector3::operator*(real_t) const
	 * \param lhs A scalar.
	 */
	friend math::vector3 operator* ( real_t lhs, const math::vector3& rhs );

	//!\c stream output operator
	/*!
	 * Outputs \a vec as follows:\n\n
	 * \a x \a y \a z \n\n
	 * No new line is placed at the end of the stream.
	 */
	friend std::ostream& operator<< ( std::ostream& o, const math::vector3& vec );

	//!Cross Product
	/*! 
	 * Returns the 
	 * <a href="http://mathworld.wolfram.com/CrossProduct.html">cross product</a>
	 * of \a lhs and \a rhs.
	 */
	friend math::vector3 math::cross( const math::vector3& lhs, const math::vector3& rhs );

	//!Vector Addition
	/*! 
	 * Computes the 
	 * <a href="http://mathworld.wolfram.com/VectorAddition.html">vector sum</a> 
	 * of \a lhs and \a rhs, placing the result in \a dest.
	 * \note This function will always be faster than 
	 * math::vector3::operator+(const math::vector3&) const
	 * \sa math::vector3::operator+(const math::vector3&) const
	 * \param dest Vector sum of \a lhs and \a rhs (ouput of function).
	 */
	friend void math::add( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );

	//!Vector Subtraction
	/*! 
	 * Computes the 
	 * <a href="http://mathworld.wolfram.com/VectorSubtraction.html">vector difference</a> 
	 * of \a lhs and \a rhs, placing the result in \a dest.
	 * \note This function will always be faster than 
	 * math::vector3::operator-(const math::vector3&) const
	 * \sa math::vector3::operator-(const math::vector3&) const
	 * \param dest Vector difference of \a lhs and \a rhs (ouput of function).
	 */
	friend void math::subtract( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );

	//!Vector Component Multiplication
	/*! 
	 * Multiplies the corresponding components of \a lhs and \a rhs, and
	 * places the result in \a dest.
	 * \note This function will always be faster than 
	 * math::vector3::operator*(const math::vector3&) const
	 * \sa math::vector3::operator*(const math::vector3&) const
	 * \param dest Result of the multiplication of each component of \a lhs and \a rhs (ouput of function).
	 */
	friend void math::multiply( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );

	//!Vector Component Division
	/*! 
	 * Divides the corresponding components of \a lhs by \a rhs, and
	 * places the result in \a dest.
	 * \note This function will always be faster than 
	 * math::vector3::operator/(const math::vector3&) const
	 * \sa math::vector3::operator/(const math::vector3&) const
	 * \param dest Result of the division of each component of \a lhs and \a rhs (ouput of function).
	 */

	friend void math::divide( math::vector3& dest, const math::vector3& lhs, const math::vector3& rhs );

	//!Scalar Multiplication
	/*! 
	 * Computes the 
	 * <a href="http://mathworld.wolfram.com/ScalarMultiplication.html">scalar product</a> 
	 * of \a lhs and \a rhs, placing the result in \a dest.
	 * \note This function will always be faster than 
	 * math::vector3::operator*(real_t) const
	 * \sa math::vector3::operator-(real_t) const
	 * \param dest Scalar product of \a lhs and \a rhs (ouput of function).
	 * \param rhs A scalar.
	 */
	friend void math::multiply( math::vector3& dest, const math::vector3& lhs, real_t rhs );

	//!Scalar Multiplication
	/*! 
	 * Computes the 
	 * <a href="http://mathworld.wolfram.com/ScalarMultiplication.html">scalar product</a> 
	 * of \a lhs and \a rhs, placing the result in \a dest.
	 * \note This function will always be faster than 
	 * math::vector3::operator*(real_t) const
	 * \sa math::vector3::operator-(real_t) const
	 * \param dest Scalar product of \a lhs and \a rhs (ouput of function).
	 * \param lhs A scalar.
	 */
	friend void math::multiply( math::vector3& dest, real_t rhs, const math::vector3& lhs );


	//!Dot Product
	/*! 
	 * Returns the 
	 * <a href="http://mathworld.wolfram.com/DotProduct.html">dot product</a>
	 * of \a lhs and \a rhs.
	 */
	friend real_t math::dot( const math::vector3& lhs, const math::vector3& rhs );

public:
	//!Default Constructor
	/*!
	 * Initializes the \a x, \a y, and \a z components to 0.0.
	 */
	vector3( void ) {
		m_data[ 0 ] = 0.0;
		m_data[ 1 ] = 0.0;
		m_data[ 2 ] = 0.0;
	}
	
	//!Constructor
	/*!
	 * Sets vector to (\a x_, \a y_, \a z_).
	 */
	vector3( real_t x_, real_t y_, real_t z_ ) {
		m_data[ 0 ] = x_;
		m_data[ 1 ] = y_;
		m_data[ 2 ] = z_;
	}

	//!Constructor
	/*!
	 * Sets vector to what \a r3 points too.
	 * \param r3 Pointer to a \c real_t[ 3 ]
	 */
	vector3( real_t *r3 ) {
		m_data[ 0 ] = r3[ 0 ];
		m_data[ 1 ] = r3[ 1 ];
		m_data[ 2 ] = r3[ 2 ];
	}

	//!Copy Constructor
	vector3( const math::vector3& p ) {
		m_data[ 0 ] = p.m_data[ 0 ];
		m_data[ 1 ] = p.m_data[ 1 ];
		m_data[ 2 ] = p.m_data[ 2 ];
	}
	
	//!Assignment Operator
	inline math::vector3& operator= ( const math::vector3& p ) {
		m_data[ 0 ] = p.m_data[ 0 ];
		m_data[ 1 ] = p.m_data[ 1 ];
		m_data[ 2 ] = p.m_data[ 2 ];
		return *this;
	}

	//!Sets vector to (\a x_, \a y_, \a z_).
	inline void set( real_t x_, real_t y_, real_t z_ ) {
		m_data[ 0 ] = x_;
		m_data[ 1 ] = y_;
		m_data[ 2 ] = z_;
	}

	//! Sets vector to array.
	/*!
	 * Sets vector to what \a r3 points too.
	 * \param r3 Pointer to a \c real_t[ 3 ]
	 */
	inline void set( real_t *r3 ) {
		m_data[ 0 ] = r3[ 0 ];
		m_data[ 1 ] = r3[ 1 ];
		m_data[ 2 ] = r3[ 2 ];
	}

	//! Array like \em read access to vector.
	/*!
	 * Returns a copy of either the \a x, \a y, or \a z component of 
	 * the vector based on the \a index passed in.  If the index is 
	 * not 0, 1, or 3, abort( ) may be called.
	 * \sa math::vector3::operator()(unsigned int) for write access
	 */
	inline real_t operator( ) ( unsigned int index ) const {
		assert( index < 3 );
		return m_data[ index ];
	}

	//! Array like \em write access to vector.
	/*!
	 * Returns a reference to either the \a x, \a y, or \a z component of 
	 * the vector based on the \a index passed in.  If the index is 
	 * not 0, 1, or 3, abort( ) may be called.
	 */
	inline real_t& operator( ) ( unsigned int index ) {
		assert( index < 3 );
		return m_data[ index ];
	}

	//!Vector Addition
	/*! 
	 * Returns the 
	 * <a href="http://mathworld.wolfram.com/VectorAddition.html">vector sum</a> 
	 * of the vector and \a rhs.
	 * \sa math::add(math::vector3&,const math::vector3&,const math::vector3&)
	 */
	inline math::vector3 operator+ ( const math::vector3& rhs ) const {
		return math::vector3( 
			(m_data[ 0 ] + rhs.m_data[ 0 ]), 
			(m_data[ 1 ] + rhs.m_data[ 1 ]),
			(m_data[ 2 ] + rhs.m_data[ 2 ])
		);
	}

	//!Vector Subtraction
	/*! 
	 * Returns the 
	 * <a href="http://mathworld.wolfram.com/VectorSubtraction.html">vector difference</a> 
	 * of the vector and \a rhs.
	 * \sa math::subtract(math::vector3&,const math::vector3&,const math::vector3&)
	 */
	inline math::vector3 operator- ( const math::vector3& rhs ) const {
		return math::vector3( 
			(m_data[ 0 ] - rhs.m_data[ 0 ]), 
			(m_data[ 1 ] - rhs.m_data[ 1 ]),
			(m_data[ 2 ] - rhs.m_data[ 2 ])
		);
	}

	//!Vector Component Multiplication
	/*! 
	 * Multiplies the corresponding components of the vector and \a rhs,
	 * returning the result.
	 * \sa math::multiply(math::vector3&,const math::vector3&,const math::vector3&)
	 */
	inline math::vector3 operator* ( const math::vector3& rhs ) const {
		return math::vector3( 
			(m_data[ 0 ] * rhs.m_data[ 0 ]), 
			(m_data[ 1 ] * rhs.m_data[ 1 ]),
			(m_data[ 2 ] * rhs.m_data[ 2 ])
		);
	}

	//!Vector Component Division
	/*! 
	 * Divides the corresponding components of \a this by \a rhs,
	 * returning the result.
	 * \sa math::divide(math::vector3&,const math::vector3&,const math::vector3&)
	 * \warn No divide by zero checks.
	 */
	inline math::vector3 operator/ ( const math::vector3& rhs ) const {
		return math::vector3( 
			(m_data[ 0 ] / rhs.m_data[ 0 ]), 
			(m_data[ 1 ] / rhs.m_data[ 1 ]),
			(m_data[ 2 ] / rhs.m_data[ 2 ])
		);
	}

	//!Returns the negated (flipped) vector.
	/*!
	 * \note A \em new vector is returned.  This vector's data is unchanged.
	 */
	inline math::vector3 operator- ( void ) const {
		return math::vector3( 
			(m_data[ 0 ] * -1.0), 
			(m_data[ 1 ] * -1.0),
			(m_data[ 2 ] * -1.0)
		);
	}

	//!Scalar Multiplication
	/*! 
	 * Returns the 
	 * <a href="http://mathworld.wolfram.com/ScalarMuliplication.html">scalar product</a> 
	 * of the vector and \a rhs.
	 * \sa math::multiply(math::vector3&,const math::vector3&,real_t)
	 * \param rhs A scalar.
	 */
	inline math::vector3 operator* ( real_t rhs ) const {
		return math::vector3( 
			(m_data[ 0 ] * rhs), 
			(m_data[ 1 ] * rhs),
			(m_data[ 2 ] * rhs)
		);
	}

	//!Scalar Division
	/*! 
	 * Divides each component of the vector by
	 * \a rhs, returning the result.
	 * \param rhs A scalar.
	 * \warn No divide by zero checks.
	 */
	inline math::vector3 operator/ ( real_t rhs ) const {
		return math::vector3( 
			(m_data[ 0 ] / rhs), 
			(m_data[ 1 ] / rhs),
			(m_data[ 2 ] / rhs)
		);
	}

	//!Vector Subtraction
	/*!
	 * Subtracts \a v from the vector.  The result is stored in \a this vector.
	 */
	inline void operator-= ( const math::vector3& v ) {
		m_data[ 0 ] -= v.m_data[ 0 ];
		m_data[ 1 ] -= v.m_data[ 1 ];
		m_data[ 2 ] -= v.m_data[ 2 ];
	}

	//!Vector Addition
	/*!
	 * Adds \a v to the vector.  The result is stored in \a this vector.
	 */
	inline void operator+= ( const math::vector3& v ) {
		m_data[ 0 ] += v.m_data[ 0 ];
		m_data[ 1 ] += v.m_data[ 1 ];
		m_data[ 2 ] += v.m_data[ 2 ];
	}

	//!Vector Component Multiplication
	/*!
	 * Multiplies the corresponding components of the vector and \a rhs,
	 * The result is stored in \a this vector.
	 */
	inline void operator*= ( const math::vector3 &rhs ) {
		m_data[ 0 ] *= rhs.m_data[ 0 ];
		m_data[ 1 ] *= rhs.m_data[ 1 ];
		m_data[ 2 ] *= rhs.m_data[ 2 ];
	}

	//!Vector Component Division
	/*!
	 * Divides the corresponding components of the vector by \a rhs,
	 * The result is stored in \a this vector.
	 * \warn No divide by zero checks.
	 */
	inline void operator/= ( const math::vector3 &rhs ) {
		m_data[ 0 ] /= rhs.m_data[ 0 ];
		m_data[ 1 ] /= rhs.m_data[ 1 ];
		m_data[ 2 ] /= rhs.m_data[ 2 ];
	}

	//!Scalar Subtraction
	/*!
	 * Subtracts \a rhs from each component in the vector.  
	 * The result is stored in \a this vector.
	 */
	inline void operator-= ( real_t rhs ) {
		m_data[ 0 ] -= rhs;
		m_data[ 1 ] -= rhs;
		m_data[ 2 ] -= rhs;
	}

	//!Scalar Addition
	/*!
	 * Adds \a rhs to each component in the vector.  
	 * The result is stored in \a this vector.
	 */
	inline void operator+= ( real_t rhs ) {
		m_data[ 0 ] += rhs;
		m_data[ 1 ] += rhs;
		m_data[ 2 ] += rhs;
	}

	//!Scalar Multiplication
	/*!
	 * Multiplies \a rhs to each component in the vector.  
	 * The result is stored in \a this vector.
	 */
	inline void operator*= ( real_t rhs ) {
		m_data[ 0 ] *= rhs;
		m_data[ 1 ] *= rhs;
		m_data[ 2 ] *= rhs;
	}

	//!Scalar Division
	/*!
	 * Divides \a rhs from each component in the vector.  
	 * The result is stored in \a this vector.
	 * \warn No divide by zero checks.
	 */
	inline void operator/= ( real_t rhs ) {
		m_data[ 0 ] /= rhs;
		m_data[ 1 ] /= rhs;
		m_data[ 2 ] /= rhs;
	}

	//! Returns a \em copy of \a x.
	inline real_t x( void ) const { 
		return m_data[ 0 ];
	}
	
	//! Returns a \em copy of \a y.
	inline real_t y( void ) const { 
		return m_data[ 1 ];
	}

	//! Returns a \em copy of \a z.
	inline real_t z( void ) const { 
		return m_data[ 2 ];
	}
	
	//! Sets the value of \a x.
	inline void x( real_t x_ ) {
		m_data[ 0 ] = x_;
	}
	
	//! Sets the value of \a y.
	inline void y( real_t y_ ) {
		m_data[ 1 ] = y_;
	}
	
	//! Sets the value of \a z.
	inline void z( real_t z_ ) {
		m_data[ 2 ] = z_;
	}

	//!Returns the length of the vector.
	/*!The length of a vector is defined as:
	 * \f$\sqrt{x^2+y^2+z^2}\f$.
	 */
	inline double length( void ) const {
		return std::sqrt( (m_data[ 0 ] * m_data[ 0 ]) +
			(m_data[ 1 ] * m_data[ 1 ]) +
			(m_data[ 2 ] * m_data[ 2 ])
		);
	}

	//!Normalizes the vector.
	/*! Sets this vector to be unit length.  A unit length vector is defined
	 * as \f$\hat{v}\equiv\frac{v}{|v|}\f$.
	 */
	inline void normalize( void ) {
		double len = length( );
		if( !math::equals( len, 0.0 ) ) {
			m_data[ 0 ] /= len;
			m_data[ 1 ] /= len;
			m_data[ 2 ] /= len;
		}
	}

	//!Negates (flips) the vector.
	/*!
	 * \note The internal data of the vector is changed.  No vector is returned.
	 * \sa math::vector3::operator-() const
	 */
	inline void negate( void )
	{
		m_data[ 0 ] *= -1.0;
		m_data[ 1 ] *= -1.0;
		m_data[ 2 ] *= -1.0;
	}

	//!Equals operator.
	/*! 
	 * Checks if the vector is equal to \a rhs.
	 * \note Accounts for precision errors.
	 * \returns True is the vectors are equal.
	 */
	inline bool operator== ( const math::vector3& rhs ) const {
		return (math::equals( m_data[ 0 ], rhs.m_data[ 0 ] ) &&
			math::equals( m_data[ 1 ], rhs.m_data[ 1 ] ) &&
			math::equals( m_data[ 2 ], rhs.m_data[ 2 ] )
		);
	}

	//!Not equals operator.
	/*! 
	 * Checks if the vector is not equal to \a rhs.
	 * \note Accounts for precision errors.
	 * \returns True is the vectors are not equal.
	 */
	inline bool operator!= ( const math::vector3& rhs ) const {
		return !(math::equals( m_data[ 0 ], rhs.m_data[ 0 ] ) &&
			math::equals( m_data[ 1 ], rhs.m_data[ 1 ] ) &&
			math::equals( m_data[ 2 ], rhs.m_data[ 2 ] )
		);
	}

	//!Cast operator to \c const \c real_t*
	/*!
	 * Returns a read only pointer to the vector's \a x, \a y, \a z data.
	 */
	inline operator const real_t*( void ) const {
		return m_data;
	}

	//! Linear Interpretation
	/*!
	 * Interprets the between the two vectors that \a a and \a b describe.
	 * \param percent A \a real_t between [0-1] descibing how much to interpret between \a qa and \a qb.
	 */
	static math::vector3 lerp( real_t percent, const math::vector3& a, 
		const math::vector3& b );
	
	///////////////////////////////////////////////////////////////////////////
	static const vector3 ZERO; //!< The vector \f$(0,0,0)\f$
	static const vector3 X_AXIS; //!< The vector \f$(1,0,0)\f$
	static const vector3 Y_AXIS; //!< The vector \f$(0,1,0)\f$
	static const vector3 Z_AXIS; //!< The vector \f$(0,0,1)\f$
};
	
#endif

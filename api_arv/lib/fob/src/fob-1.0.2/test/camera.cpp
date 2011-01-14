//FILE:         camera.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"

///////////////////////////////////////////////////////////////////////////////
camera::camera( void ):
	m_left( false ), m_right( false ), m_forward( false ), m_back( false ),
	speed( 5.0 )
{

}

///////////////////////////////////////////////////////////////////////////////
void
camera::view( void )
{
	math::vector3 look_pt = position + forward * 10.0;
	gluLookAt( 
		position.x( ), position.y( ), position.z( ),
		look_pt.x( ), look_pt.y( ), look_pt.z( ),
		up.x( ), up.y( ), up.z( )
	);
}

///////////////////////////////////////////////////////////////////////////////
void 
camera::set_speed( real_t s )
{
	speed = s;
}

///////////////////////////////////////////////////////////////////////////////
void 
camera::set_position( real_t x, real_t y, real_t z )
{
	position.set( x, y, z );
}

///////////////////////////////////////////////////////////////////////////////
void 
camera::set_forward_right( const math::vector3& f, const math::vector3 r )
{
	forward = f;
	right = r;
	up = math::cross( right, forward );
}

///////////////////////////////////////////////////////////////////////////////
void 
camera::set_look_right( const math::vector3& l, const math::vector3 r )
{
	forward = l - position;
	forward.normalize( );
	right = r;
	up = math::cross( right, forward );
}

///////////////////////////////////////////////////////////////////////////////
const math::vector3& 
camera::get_up( void ) const
{
	return up;
}

///////////////////////////////////////////////////////////////////////////////
const math::vector3& 
camera::get_right( void ) const
{
	return right;
}

//////////////////////////////////////////////////////////////////////////
void
camera::update( real_t elapsed )
{
	//figure out movement vector
	math::vector3 move( 0.0, 0.0, 0.0 );
	math::vector3 f( 0.0, 0.0, -1.0 );
	math::vector3 r( 1.0, 0.0, 0.0 );

	if( m_forward ) {
		move += f;
	}
	if( m_back ) {
		move += -1.0 * f;
	}
	if( m_right ) {
		move += r;
	}
	if( m_left ) {
		move += -1.0 * r;
	}

	//normalize then scale the vector by the current speed
	move.normalize( );
	move *= (speed * elapsed);

	//add the movement vector to the position
	position += move;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::move_forward( void )
{
	m_forward = true;
	m_back = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::move_left( void )
{
	m_left = true;
	m_right = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::move_right( void )
{
	m_right = true;
	m_left = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::move_back( void )
{
	m_back = true;
	m_forward = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::stop_forward( void )
{
	m_forward = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::stop_left( void )
{
	m_left = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::stop_right( void )
{
	m_right = false;
}

//////////////////////////////////////////////////////////////////////////
void 
camera::stop_back( void )
{
	m_back = false;
}

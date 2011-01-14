//FILE:         camera.h
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#ifndef COURNIA_CAMERA_H
#define COURNIA_CAMERA_H 1

#include "fob/vector.h"

class camera 
{
private:
	math::vector3 position;
	math::vector3 forward;
	math::vector3 right;
	math::vector3 up;

	bool m_left, m_right, m_forward, m_back;
	real_t speed;

public:
	camera( void );

	void view( void );

	void set_speed( real_t s );
	
	void set_position( real_t x, real_t y, real_t z );
	void set_forward_right( const math::vector3& f, const math::vector3 r );
	void set_look_right( const math::vector3& l, const math::vector3 r );

	const math::vector3& get_up( void ) const;
	const math::vector3& get_right( void ) const;

	void update( real_t elapsed );

	void move_forward( void );
	void move_left( void );
	void move_right( void );
	void move_back( void );
	void stop_forward( void );
	void stop_left( void );
	void stop_right( void );
	void stop_back( void );
};

#endif

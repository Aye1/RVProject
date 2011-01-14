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

//FILE:         fob.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>
//FIXME:        endian issues?  if so, can be fixed in examine,
//              change, send_cmd, unpack, etc.
//NOTE:         Checking the error seems to send the next command to the
//              master bird.

#include <iostream>
#include <iomanip>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "fob/fob.h"

#define DEBUG(x) (std::cerr << x << std::endl)
//#define DEBUG(x)

const fob::mode fob::POSITION = 0x01;
const fob::mode fob::ORIENTATION = 0x02;
const fob::mode fob::BUTTONS = 0x04;

const unsigned char fob::BUTTON_LEFT = 0x10;
const unsigned char fob::BUTTON_MIDDLE = 0x30;
const unsigned char fob::BUTTON_RIGHT = 0x70;

const unsigned char fob::STREAM = '@';

const unsigned char fob::POINT = 'B';
const unsigned char fob::SLEEP = 'G';

const unsigned char fob::EXAMINE = 'O';
const unsigned char fob::CHANGE = 'P';

const unsigned char fob::MODE_POS = 'V';
const unsigned char fob::MODE_ANG = 'W';
const unsigned char fob::MODE_POS_ANG = 'Y';
const unsigned char fob::MODE_POS_MAT = 'Z';
const unsigned char fob::MODE_POS_QUAT = ']';
const unsigned char fob::MODE_QUAT = 0x5C;
const unsigned char fob::FBB_RESET = '/';
const unsigned char fob::RUN = 'F';

const fob::command fob::HEMISPHERE = { 'L', 2 };
const fob::command fob::BUTTON_MODE = { 'M', 1 };
const fob::command fob::REF_FRAME_1 = { 'H', 12 };
const fob::command fob::REF_FRAME_2 = { 'r', 6 };

const fob::examine_option fob::BIRD_STATUS = { 0, 2 };
const fob::examine_option fob::ERROR_CODE = { 10, 1 };
const fob::examine_option fob::MODEL_ID = { 15, 10 };
const fob::examine_option fob::REF_FRAME_1_EXAMINE = { 17, 1 };
const fob::examine_option fob::ADDR_MODE = { 19, 1 };
const fob::examine_option fob::FLOCK_STATUS = { 36, FOB_MAX_BIRDS }; //14 is normal address mode only

const fob::change_option fob::REF_FRAME_1_ENABLE = { 17, 1 };
const fob::change_option fob::GROUP = { 35, 1 };
const fob::change_option fob::FBB_AUTO_CONFIG = { 50, 1 };

const float fob::SCALE = 1.0 / 32767.0;
const float fob::ANGLE_SCALE = 180.0 / 32767.0;

static const fob::error_t error_msgs[ ] = {
	{ "System Ram Failure",  fob::FATAL },                               //1
	{ "Non-Volatile Storage Write Failure", fob::FATAL },
	{ "PCB Configuration Data Corrupt", fob::WARNING1 },
	{ "Bird Transmitter Calibration Data Corrupt or Not Connected", fob::WARNING1 },
	{ "Bird Sensor Calibration Data Corrupt or Not Connected", fob::WARNING1 }, 
	{ "Invalid RS232 Command", fob::WARNING2 },
	{ "Not an FBB Master", fob::WARNING2 },
	{ "No Birds accessible in Device List", fob::WARNING2 },
	{ "Bird is Not Initialized", fob::WARNING2 },
	{ "FBB Serial Port Receive Error - Intra Bird Bus", fob::WARNING1 }, //10
	{ "RS232 Serial Port Receive Error", fob::WARNING1 },
	{ "FBB Serial Port Receive Error - FBB Host Bus", fob::WARNING1 },
	{ "No FBB Command Response", fob::WARNING1 },
	{ "Invalid FBB Host Command", fob::WARNING1 },
	{ "FBB Run Time Error", fob::FATAL },
	{ "Invalid CPU Speed", fob::FATAL },
	{ "No FBB Data", fob::WARNING1 },
	{ "Illegal Baud Rate", fob::WARNING1 },
	{ "Slave Acknowledge Error", fob::WARNING1 },
	{ "Intel 80186 CPU Error", fob::FATAL },                             //20
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "Intel 80186 CPU Error", fob::FATAL },
	{ "CRT Synchronization", fob::WARNING1 },
	{ "Transmitter Not accessible", fob::WARNING1 },
	{ "Extended Range Transmitter Not Attached", fob::WARNING2 },        //30
	{ "CPU Time Overflow", fob::WARNING2 },
	{ "Sensor Saturated", fob::WARNING1 },
	{ "Slave Configuration", fob::WARNING1 },
	{ "Watch Dog Timer", fob::WARNING1 },
	{ "Over Temperature", fob::WARNING1 }
};



///////////////////////////////////////////////////////////////////////////////
static const char*
error_level_str( fob::error_level lvl )
{
	switch( lvl ) {
		case fob::WARNING1:
			return "warning1";
		break;

		case fob::WARNING2:
			return "warning2";
		break;

		case fob::FATAL:
			return "fatal";
		break;

		default:
			return "unknown";
	}
}



///////////////////////////////////////////////////////////////////////////////
//from pg 26 of fbb manual
static void
unpack( unsigned char *buffer, short *output, int size )
{
	for( int i = 0, j = 0; i < size; i += 2 ) {
		//shift ls
		buffer[ i ] = buffer[ i ] << 1;

		output[ j++ ] = ((buffer[ i + 1 ] << 8) | buffer[ i ]) << 1;
	}
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::bird::set_mode( fob::mode mask )
{
	//bird can't only just send buttons
	if( mask == fob::BUTTONS ) {
		m_flock.set_error( "fob::bird::set_mode: must specify data mode with buttons" );
		return false;
	}

	//select this bird
	if( !m_flock.select_bird( *this ) ) {
		//select_bird will set error in parent flock
		return false;
	}

	//what info does the user want
	if( (mask & (fob::POSITION | fob::ORIENTATION)) == 
	 	(fob::POSITION | fob::ORIENTATION) ) {
		//want position and orientation
		DEBUG( "fob::bird::set_mode: pos/angle" );
		if( !m_flock.send_cmd( MODE_POS_ANG ) ) {
			//parent flock error set
			return false;
		}
	} else if( mask & fob::POSITION ) {
		//want position
		DEBUG( "fob::bird::set_mode: pos" );
		if( !m_flock.send_cmd( MODE_POS ) ) {
			//parent flock error set
			return false;
		}
	} else if( mask & fob::ORIENTATION ) {
		//want orientation
		DEBUG( "fob::bird::set_mode: angle" );
		if( !m_flock.send_cmd( MODE_ANG ) ) {
			//parent flock error set
			return false;
		}
	}

	//select this bird
	if( !m_flock.select_bird( *this ) ) {
		//select_bird will set error in parent flock
		return false;
	}

	//do we want button info
	unsigned char button_mode;
	if( mask & fob::BUTTONS ) {
		//want button info
		DEBUG( "fob::bird::set_mode: enabling buttons" );
		button_mode = 0x01;
	} else {
		//don't want button info
		button_mode = 0x00;
	}

	//send button command
	if( !m_flock.send_cmd( BUTTON_MODE, &button_mode ) ) {
		//parent flock error set
		return false;
	}

	//check for error
	m_flock.clear_device( );
	if( m_flock.check_error( ) ) {
		return false;
	}

	//set the mode
	lock_data( );
	m_mode = mask;
	unlock_data( );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
void 
fob::bird::set_rotation( const math::quaternion& quat )
{
	m_rotation = quat;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::bird::unpack_pos_angle( unsigned char *buffer, int size )
{
	//position angle format 12 bytes of data
	if( 12 != size ) {
		//wrong amount of data, bail
		DEBUG( "fob::bird::update: pos/ang size mismatch: " << size );
		return false;
	}

	//unpack data
	short unpacked[ 6 ];
	unpack( buffer, unpacked, 12 );

	//not let the user access this data
	lock_data( );
	
	//scale and copy data
	//map x, y, z from birds to -y, -z, x
	//float pos_scale = m_flock.get_scale( ) * SCALE;
	float pos_scale = 36.0 * SCALE;
	m_position.set(
		static_cast<float>( -unpacked[ 1 ] ) * pos_scale,
		static_cast<float>( -unpacked[ 2 ] ) * pos_scale,
		static_cast<float>( unpacked[ 0 ] ) * pos_scale

	);
	
	//note that bird sends z, y, x
	//map x, y, z from birds to -y, -z, x
	m_fob_angles.set( 
		static_cast<float>( -unpacked[ 4 ] ) * ANGLE_SCALE,
		static_cast<float>( -unpacked[ 3 ] ) * ANGLE_SCALE,
		static_cast<float>( unpacked[ 5 ] ) * ANGLE_SCALE
	);

	m_ori_dirty = true;

	//let the user have fun
	unlock_data( );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::bird::unpack_angle( unsigned char *buffer, int size )
{
	//angle format 6 bytes of data
	if( 6 != size ) {
		//wrong amount of data, bail
		DEBUG( "fob::bird::update: angle size mismatch" );
		return false;
	}

	//unpack data
	short unpacked[ 3 ];
	unpack( buffer, unpacked, 6 );

	//not let the user access this data
	lock_data( );
	
	//note that bird sends z, y, x
	//map x, y, z from birds to -y, -z, x
	m_fob_angles.set( 
		static_cast<float>( -unpacked[ 1 ] ) * ANGLE_SCALE,
		static_cast<float>( -unpacked[ 0 ] ) * ANGLE_SCALE,
		static_cast<float>( unpacked[ 2 ] ) * ANGLE_SCALE
	);

	//mark new orientation input
	m_ori_dirty = true;

	//let the user have fun
	unlock_data( );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::bird::unpack_pos( unsigned char *buffer, int size )
{
	//position format 6 bytes of data
	if( 6 != size ) {
		//wrong amount of data, bail
		DEBUG( "fob::bird::update: position size mismatch" );
		return false;
	}

	//unpack data
	short unpacked[ 3 ];
	unpack( buffer, unpacked, 6 );

	//not let the user access this data
	lock_data( );
		
	//scale and copy data
	//map x, y, z from birds to -y, -z, x
	//float pos_scale = m_flock.get_scale( ) * SCALE;
	float pos_scale = 36.0 * SCALE;
	m_position.set(
		static_cast<float>( -unpacked[ 1 ] ) * pos_scale,
		static_cast<float>( -unpacked[ 2 ] ) * pos_scale,
		static_cast<float>( unpacked[ 0 ] ) * pos_scale

	);

	//mark new orientation input
	m_ori_dirty = true;

	//let the user have fun
	unlock_data( );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::bird::update( unsigned char *buffer, int size )
{
	//do we have button data
	int button_size = 0;
	//DEBUG( "fob::bird::update: buffer size: " << size );
	lock_data( );
	if( m_mode & fob::BUTTONS ) {
		m_buttons = buffer[ size - 1 ];
		//DEBUG( "fob::bird::update: button read: " << (int)(m_buttons) );
		button_size = 1;
	}
	unlock_data( );
	
	//what info does the user want
	if( (m_mode & (fob::POSITION | fob::ORIENTATION)) == 
		(fob::POSITION | fob::ORIENTATION) ) {
		//unpack position orientation
		unpack_pos_angle( buffer, size - button_size );
	} else if( m_mode & fob::ORIENTATION ) {
		//unpack orientation
		unpack_angle( buffer, size - button_size );
	} else if( m_mode & fob::POSITION ) {
		//unpack orientation
		unpack_pos( buffer, size - button_size );
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
//assumes data is locked!
void 
fob::bird::update_orientation( void )
{
	//apply x and y rotation
	math::quaternion tmp;
	m_quaternion.from_angle_axis( math::to_radians( m_fob_angles.x( ) ),
		math::vector3::X_AXIS );
	tmp.from_angle_axis( math::to_radians( m_fob_angles.y( ) ),
		math::vector3::Y_AXIS );
	m_quaternion =  tmp * m_quaternion;
	m_quaternion.normalize( );
	
	//the rotation needs to be applied to the new reference frame
	//not the global one
	math::vector3 axis;
	tmp.set( math::vector3::Z_AXIS, 0.0 );
	axis = (m_quaternion * tmp * !m_quaternion).vec( );
	axis.normalize( );
	tmp.from_angle_axis( math::to_radians( m_fob_angles.z( ) ),
		axis );
	m_quaternion =  tmp * m_quaternion;
	m_quaternion.normalize( );
	
	//rotate by the correction rotation (for bad sensor installations)
	m_quaternion =  m_quaternion * m_rotation;
	m_quaternion.normalize( );

	//FIXME m_angles should contain the angles with m_rotation
	//      applied 
	m_angles = m_fob_angles;

	//update matrix
	m_quaternion.get_rotation_matrix( m_matrix );
	m_matrix.set_translation( m_position );

	//orientation is new
	m_ori_dirty = false;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::set_rts( bool high )
{
	int status;
	if( ioctl( m_device, TIOCMGET, &status ) < 0 ) {
		set_error( "fob::set_rts: could not read RTS: %s", 
			strerror( errno ) );
		return false;
	}

	if( high ) {
		if( status & TIOCM_RTS ) {
			//already high
			return true;
		}
	} else {
		if( !(status & TIOCM_RTS) ) {
			//already low
			return true;
		}
	}

	//fall through, must toggle rts
	status ^= TIOCM_RTS;
	if( ioctl( m_device, TIOCMSET, &status ) < 0 ) {
		set_error( "fob::set_rts: could not set RTS: %s", 
			strerror( errno ) );
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::select_bird( fob::bird& b )
{
	DEBUG( "fob::select_bird: " << b.m_address );
	unsigned char address = b.m_address;
	address += 0xF0;

	//send the command
	cmd_sleep( );
	if( write( m_device, &address, 1 ) != 1 ) {
		set_error( "fob::select_bird: could not send select bird" );
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool 
fob::select_bird( int bird_addr )
{
	DEBUG( "fob::select_bird i: " << bird_addr );
	unsigned char address = bird_addr;
	address += 0xF0;

	//send the command
	cmd_sleep( );
	if( write( m_device, &address, 1 ) != 1 ) {
		set_error( "fob::select_bird: could not send select bird" );
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
//totally for debugging, must be run before fly
//prints to stderr
bool 
fob::print_bird_status( void )
{
	if( m_fly ) {
		//we're flying, can't send this command
		set_error( "fob::print_bird_status: can not get status while flying" );
		return false;
	}
	
	unsigned char buffer[ 256 ];
	for( unsigned int i = 0; i < m_birds.size( ); ++i ) {
		//select the current bird
		if( !select_bird( *m_birds[ i ] ) ) {
			return false;
		}
	
		//get the status of this bird
		if( !examine( BIRD_STATUS, buffer ) ) {
			//examine sets error
			return false;
		}

		//any errors?
		clear_device( );
		if( check_error( ) ) {
			return false;
		}

		std::cout << "bird status: " << i << " ";
		for( int b = 1; b >= 0; --b ) {
			for( int j = 7; j >= 0; --j ) {
				std::cout << ((buffer[ b ] >> j) & 1);
			}
			std::cout << " ";
		}
		std::cout << std::endl;
		std::cout.flush( );
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
void 
fob::clear_device( void )
{
	//flush (discard) both data received but not read 
	//and data written but not transmitted
	tcflush( m_device, TCIOFLUSH );
}



///////////////////////////////////////////////////////////////////////////////
int
fob::read( unsigned char *buffer, int bytes )
{
	assert( bytes > 0 );

	//read in 1 byte at a time
	unsigned int read_error = 0;
	int i, count;
	for( i = 0; (i < bytes) && (read_error < 5); ) {
		//read in 1 byte at a time
		//std::cerr << "read: " << i << " " << bytes << " " << read_error << std::endl;
		count = ::read( m_device, &buffer[ i ], 1 );
		if( count < 0 ) {
			//error
			if( errno != EINTR ) {
				//error code returned, but not an interrupt
				//std::cerr << "eintr" << std::endl;
				++read_error;
			}
		} else if( count < 1 ) {
			//for some reason, couldn't read a byte (shouldn't happen)
			++read_error;
		} else {
			//byte read
			++i;
		}
	}

	return i;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::send_cmd( unsigned char cmd )
{
	//send command
	cmd_sleep( );
	if( write( m_device, &cmd, 1 ) != 1 ) {
		set_error( "fob::send_cmd: could not send command" );
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::send_cmd( fob::command cmd, unsigned char *options )
{
	//setup command string
	unsigned char buffer[ 256 ];
	buffer[ 0 ] = cmd.cmd;
	memcpy( &(buffer[ 1 ]), options, cmd.option_bytes );

	//send command
	cmd_sleep( );
	int size = 1 + cmd.option_bytes;
	if( write( m_device, buffer, size ) != size ) {
		set_error( "fob::send_cmd: could not send command" );
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::examine( fob::examine_option param, unsigned char *output )
{
	//setup command string
	unsigned char cmd[ 2 ];
	cmd[ 0 ] = EXAMINE;
	cmd[ 1 ] = param.param;

	//send command
	cmd_sleep( );
	if( write( m_device, cmd, 2 ) != 2 ) {
		set_error( "fob::examine: could not send examine command" );
		return false;
	}

	//get reply
	cmd_sleep( );
	int reply = read( output, param.reply_bytes );
	if( reply < 0 ) {
		//write error
		set_error( "fob::examine: could not recv examine reply: %s", 
			strerror( errno ) );
		return false;
	} else if( reply < param.reply_bytes ) {
		//not enough data
		set_error( "fob::examine: not enough data in examine reply: "
			"expected %d: recv %d", param.reply_bytes, reply );
		return false;
	}

	//success
	return true;

}



///////////////////////////////////////////////////////////////////////////////
bool
fob::change( fob::change_option param, unsigned char *option )
{
	//setup command string
	unsigned char cmd[ 256 ];
	cmd[ 0 ] = CHANGE;
	cmd[ 1 ] = param.param;
	memcpy( &(cmd[ 2 ]), option, param.option_bytes );

	//send command
	int size = 2 + param.option_bytes;
	if( write( m_device, cmd, size ) != size ) {
		set_error( "fob::examine: could not send examine command" );
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::check_address_mode( void )
{
	unsigned char buffer[ 256 ];
	if( !examine( ADDR_MODE, buffer ) ) {
		//examine sets error
		return false;
	}

	//must be using normal addressing mode
	DEBUG( "fob::check_address_mode: mode: " << (int)(buffer[ 0 ]) );
	if( buffer[ 0 ] != 0 ) {
		set_error( "fob::check_address_mode: "
			"flock must be set in normal addressing mode" );
		return false;
	}

	//check for error
	clear_device( );
	if( check_error( ) ) {
		return false;
	}

	//address mode ok
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::load_flock_status( void )
{
	//get status from flock
	unsigned char buffer[ 256 ];
	if( !examine( FLOCK_STATUS, buffer ) ) {
		//examine sets error
		return false;
	}

	//any errors?
	clear_device( );
	if( check_error( ) ) {
		return false;
	}
	
	//flock status reply format
	//bit 7: accessible (fly switch on)
	//bit 6: running (auto-configured and awake)
	//bit 5: sensor attached
	//bit 4: extended range xmtr (ERT) (0 if standard range)
	//bit 3: ERT #3
	//bit 2: ERT #2
	//bit 1: ERT #1
	//bit 0: xmtr attached (white cube (black? if ERT))
	
	//see what is attached
	for( int i = 0; i < FOB_MAX_BIRDS; ++i ) {
		//prints out all status bits for each connected bird (debugging)
		std::cerr << "flock status: " << i << ": ";
		for( int j = 7; j >= 0; --j ) {
			std::cerr << ((buffer[ i ] >> j) & 0x1);
		}
		std::cerr << std::endl;

		//is this bird accessible
		//mask with 1000 000
		if( buffer[ i ] & 0x80 ) {
			DEBUG( "fob::load_flock_status: bird: " << i + 1 << " accessible" );

			/*
			//make sure this bird is something we can handle (not ERT)
			//mask with 0001 1110
			if( buffer[ i ] & 0x1E ) {
				set_error( "fob::load_flock_status: unsupported extended range "
					"transmitter (ERT) detected" );
				return false;
			}
			*/
			
			//add a new bird to the bird list
			bird *b = new bird( *this );
			b->m_address = i + 1;
			m_birds.push_back( b );

			//transmitter attached?
			//mask with 0000 0001
			b->m_transmitter = (buffer[ i ] & 0x01);

			//sensor attached?
			//mask with 0010 0000
			b->m_sensor = (buffer[ i ] & 0x20);
		}
	}

	//get more information about each bird
	bool master_found = false;
	m_master = 666;
	for( unsigned int i = 0; i < m_birds.size( ); ++i ) {
		//select the current bird
		if( !select_bird( *m_birds[ i ] ) ) {
			return false;
		}

		//get the status of this bird
		if( !examine( BIRD_STATUS, buffer ) ) {
			//examine sets error
			return false;
		}

		//any errors?
		clear_device( );
		if( check_error( ) ) {
			return false;
		}

		//is this bird a master
		//mask 2nd byte with 1000 0000 (bit 15)
		//see pg.72 of fbb manual for details 
		if( buffer[ 1 ] & 0x80 ) {
			DEBUG( "fob::load_flock_status: possible master at address " << m_birds[ i ]->m_address );
			if( m_birds[ i ]->m_address < m_master ) {
				//new possible master
				master_found = true;
				m_master = m_birds[ i ]->m_address;
			}
		}
	}

	//make sure a master was found
	if( !master_found ) {
		set_error( "fob::load_flock_status: master bird could not be found" );
		return false;
	}
	DEBUG( "fob::load_flock_status: master found at address " << m_master );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
//sleep times are from fbb programming manual
bool
fob::auto_configure( unsigned int birds )
{
	if( birds == 0 ) {
		set_error( "fob::auto_configure: no birds detected" );
		return false;
	}
	
	//select the bird
	DEBUG( "fob::auto_configure: selecting master: " << m_master );
	if( !select_bird( m_master ) ) {
		//select_bird sets error
		return false;
	}

	//must sleep before the autoconfig call
	DEBUG( "fob::auto_configure: sleeping before autoconfig" );
	cmd_sleep( 2000000 ); //2 seconds
	
	unsigned char num_birds = (int)birds;
	DEBUG( "fob::auto_configure: birds to configure: " << (int)num_birds );
	if( !change( FBB_AUTO_CONFIG, &num_birds ) ) {
		//change sets error
		return false;
	}
	
	//must sleep after the autoconfig call
	DEBUG( "fob::auto_configure: sleeping 2 seconds after autoconfig" );
	cmd_sleep( 2000000 ); //2 seconds

	//any errors?
	clear_device( );
	if( check_error( ) ) {
		return false;
	}

	//set birds running
	if( !send_cmd( RUN ) ) {
		//send_cmd sets error
		return false;
	}

	//any errors?
	clear_device( );
	if( check_error( ) ) {
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::set_hemisphere( fob::bird& b, fob::hemisphere hemi )
{
	//select the bird
	if( !select_bird( b ) ) {
		//select_bird sets error
		return false;
	}

	//determine hemisphere cmd chars
	unsigned char cmd[ 2 ];
	switch( hemi ) {
		case FORWARD:
			cmd[ 0 ] = 0x00;
			cmd[ 1 ] = 0x00;
		break;
		
		case AFT:
			cmd[ 0 ] = 0x00;
			cmd[ 1 ] = 0x01;
		break;
		
		case UP:
			cmd[ 0 ] = 0x0C;
			cmd[ 1 ] = 0x01;
		break;
		
		case DOWN:
			cmd[ 0 ] = 0x0C;
			cmd[ 1 ] = 0x00;
		break;
		
		case LEFT:
			cmd[ 0 ] = 0x06;
			cmd[ 1 ] = 0x01;
		break;

		case RIGHT:
			cmd[ 0 ] = 0x06;
			cmd[ 1 ] = 0x00;
		break;

		default:
			set_error( "fob::set_hemisphere: unknown hemisphere" );
			return false;
		break;
	}

	//send hemisphere command
	if( !send_cmd( HEMISPHERE, cmd ) ) {
		//send_cmd sets error
		return false;
	}

	//any errors?
	clear_device( );
	if( check_error( ) ) {
		return false;
	}

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::reset( void )
{
	DEBUG( "fob::reset: initiating reset..." );
	//set rts high
	if( !set_rts( true ) ) {
		return false;
	}

	//set rts low (fly)
	if( !set_rts( false ) ) {
		return false;
	}

	//sleep 3 seconds to let the rts register
	//this is probably a bit of an overkill
	cmd_sleep( 3000000 );

	//send the reset command
	clear_device( );
	if( !send_cmd( FBB_RESET ) ) {
		//send_cmd sets error
		return false;
	}
	DEBUG( "fob::reset: ...done" );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
//returns true on error
bool
fob::check_error( void )
{
	unsigned char buffer[ 256 ];
	if( !examine( ERROR_CODE, buffer ) ) {
		//examine sets error
		return true;
	}

	//we should get back a single byte which contains the error code
	int code = (int)(buffer[ 0 ]);
	DEBUG( "fob::check_error: code: " << code );
	
	//make sure the error code returned is in the valid error code range
	//i.e. between [0,35]
	if( (code > 35) || (code < 0) ) {
		set_error( "fob::check_error: fob is returning corrupt data, "
		 "please reset the flock" );
		return true;
	}

	/*
	//these codes seem to happen alot, and are really annoying
	//ignoring them doesn't seem to cause much of a problem
	if( (code == 31) || (code == 32) || (code == 6) ) {
		//set_error( "fob::check_error: ignoring code: %d", code );
		const error_t *err = &error_msgs[ code - 1 ];
		DEBUG( "fob::check_error: ignoring code: " << code );
		std::cerr << "fob: ignoring error: " << code << ": "
			<< error_level_str( err->level ) << ": " << err->msg << std::endl;
		return false;
	}
	*/
	
	if( code ) {
		//code is non-zero, this means there is an error
		const error_t *err = &error_msgs[ code - 1 ];
		if( err->level < m_min_error_level ) {
			//user wants to ignore this error level
			std::cerr << "fob: ignoring error: " << code << ": "
				<< error_level_str( err->level ) << ": " << err->msg << std::endl;
		} else {
			//report the error
			set_error( "fob: %d: %s: %s\n", code, 
				error_level_str( err->level ), err->msg );
			return true;
		}
	}

	//no error
	return false;
}



///////////////////////////////////////////////////////////////////////////////
fob::fob( void ): m_error( true ), m_error_msg( "uninitialized" ),
	m_open( false ), m_device( -1 ), m_master( 1 ), m_fly( false ),
	m_group( false ), m_save( 0x00 ), m_sleep( 500000 ), 
	m_min_error_level( fob::FATAL )
{
	//create mutex
	pthread_mutex_init( &m_mutex, NULL );

	//create streaming thread
	if( pthread_create( &m_flock_thread, NULL, 
		get_flock_data, this ) != 0 ) {
		//could not create thread
		set_error( "fob::fob: could not create download thread" );
	}
}



///////////////////////////////////////////////////////////////////////////////
fob::fob( const std::string& device_name, fob::hemisphere hemi,
	fob::port_speed speed, unsigned long sleep_ms ):
	m_error( true ), m_error_msg( "uninitialized" ),
	m_open( false ), m_device( -1 ), m_master( 1 ), m_fly( false ),
	m_group( false ), m_save( 0x00 ), m_sleep( sleep_ms * 1000 ),
	m_min_error_level( fob::FATAL )
{
	//create streaming thread
	if( pthread_create( &m_flock_thread, NULL, 
		get_flock_data, this ) != 0 ) {
		//could not create thread
		set_error( "fob::fob: could not create download thread" );
	}

	//create mutex
	pthread_mutex_init( &m_mutex, NULL );
	
	//just call open, open handles setting the error state
	open( device_name, hemi, speed, sleep_ms );
}



///////////////////////////////////////////////////////////////////////////////
fob::~fob( void )
{
	//cancel the flock data retrieval thread
	pthread_cancel( m_flock_thread );

	//destroy mutex 
	pthread_mutex_destroy( &m_mutex );

	//close the serial port
	close( );
}



///////////////////////////////////////////////////////////////////////////////
const fob& 
fob::open( const std::string& device_name, 
	fob::hemisphere hemi, fob::port_speed speed, unsigned long sleep_ms )
{
	//is a device already open
	assert( !m_open );

	//save hemisphere
	m_hemisphere = hemi;

	//set the sleep time
	set_sleep( sleep_ms );
	
	//open serial port
	DEBUG( "fob::open: dev: '" << device_name << "'" );
	m_device = ::open( device_name.c_str( ), O_RDWR );
	if( m_device < 0 ) {
		set_error( "fob::open: %s", strerror( errno ) );
		return *this;
	}

	//save old serial port settings
	if( tcgetattr( m_device, &m_save_tio ) < 0 ) {
		set_error( "fob::open: %s", strerror( errno ) );
		return *this;
	}

	//init serial port settings
	struct termios settings;
	memset( &settings, 0, sizeof( struct termios ) );

	//control mode flags
	//B115200 - fast flow rate communication
	//CS8     - 8 bit, no parity, 1 stopbit
	//CLOCAL  - local connection, no modem contol
	//CREAD   - enable receiving characters
	settings.c_cflag = CS8 | CLOCAL | CREAD;
	if( speed == FAST ) {
		DEBUG( "fob::open: speed: 115200" );
		settings.c_cflag |= B115200;
	} else {
		DEBUG( "fob::open: speed: 38400" );
		settings.c_cflag |= B38400;
	}

	//input mode flags
	settings.c_iflag = IXOFF; //disable flow control

	//local mode flags 
	settings.c_lflag = 0; //non canonical input

	//control characters
	//set a 2 second timeout
	settings.c_cc[ VMIN ] = 0;
	settings.c_cc[ VTIME ] = 20;

	//tell the os the serial port settings we want
	tcflush( m_device, TCIFLUSH );
	if( tcsetattr( m_device, TCSANOW, &settings ) < 0 ) {
		set_error( "fob::open: %s", strerror( errno ) );
		return *this;
	}
	
	//clear serial device
	clear_device( );

	//reset the flock
	if( !reset( ) ) {
		//send_cmd sets error
		return *this;
	}

	//do an initial configure (this seems to be neccessary)
	if( !auto_configure( 1 ) ) {
		//auto_configure sets error
		return *this;
	}

	//must be in normal addressing mode
	clear_device( );
	DEBUG( "fob::open: checking addressing mode" );
	if( !check_address_mode( ) ) {
		//old bird versions may not respond to this check
		std::cerr << "fob::open: warning: addressing mode may be invalid" 
			<< std::endl;
	}

	//get bird info
	clear_device( );
	if( !load_flock_status( ) ) {
		//get_flock_status sets error
		return *this;
	}

	//we now have bird info, do the real configure
	clear_device( );
	if( !auto_configure( m_birds.size( ) ) ) {
		//auto_configure sets error
		return *this;
	}

	//set the hemisphere for each bird
	//FIXME this assumes each bird operates in the same hemisphere
	for( unsigned int i = 0; i < m_birds.size( ); ++i ) {
		if( m_birds[ i ]->m_sensor ) {
			clear_device( );
			if( !set_hemisphere( *m_birds[ i ], hemi ) ) {
				//set_hemisphere sets error
				return *this;
			}
		}
	}

	//success
	DEBUG( "fob::open: success" );
	m_open = true;
	clear_error( );
	return *this;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::close( void )
{
	DEBUG( "fob::close" );
	//is the device open?
	if( !m_open ) return true;

	//stop streaming and put the birds sleep
	clear_device( );
	if( m_birds.size( ) > 0 ) {
		select_bird( *m_birds[ 0 ] );
	}
	clear_device( );
	send_cmd( POINT );
	clear_device( );
	send_cmd( SLEEP );

	//put rts in low voltage (standby mode)
	cmd_sleep( );
	set_rts( false );
	
	//set the serial port settings to their old values
	if( tcsetattr( m_device, TCSANOW, &m_save_tio ) < 0 ) {
		set_error( "fob::close: %s", strerror( errno ) );
		return false;
	}
	
	//close the fob serial device
	if( ::close( m_device ) < 0 ) {
		set_error( "fob::close: %s", strerror( errno ) );
		return false;
	}

	//success
	m_open = false;
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::fly( void )
{
	DEBUG( "fob::fly" );
	lock( );
	if( m_fly ) {
		//already flying
		unlock( );
		return true;
	}
	unlock( );

	//place the flock into group mode
	DEBUG( "fob::fly: sending group command" );
	unsigned char options = 0x1;
	if( !change( GROUP, &options ) ) {
		//change sets error
		return false;
	}

	//we are in group mode now
	m_group = true;

	//place the flock in streaming mode
	DEBUG( "fob::fly: sending stream command" );
	if( !send_cmd( STREAM ) ) {
		//send_cmd sets error
		return false;
	}

	//we are now flying!
	lock( );
	m_fly = true;
	unlock( );

	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
bool
fob::update( void )
{	
	//if we are not flying, don't read streamed data
	lock( );
	if( !m_fly ) {
		unlock( );
		return true;
	}
	unlock( );

	unsigned int read_error = 0;
	unsigned int phases_found = 0;
	unsigned char buffer[ 256 ];
	unsigned int i = 0;

	//did the last byte saved contain a phasing bit
	//mask with 1000 000
	if( m_save & 0x80 ) {
		buffer[ i++ ] = m_save;
		++phases_found;
		m_save = 0x0;
	}
	
	//read in 1 byte as a type (as suggested by fbb manual)
	while( (phases_found < 2) && (read_error < 5) ) {
		//FIXME should i be checking ofr EINTR here?
		if( ::read( m_device, &buffer[ i ], 1 ) == 1 ) {
			//successful read, does this byte have the phasing bit
			//mask with 1000 000
			if( buffer[ i ] & 0x80 ) {
				++phases_found;
			}

			//next spot
			++i;
		} else {
			++read_error;
		}
	}

	if( read_error > 4 ) {
		//to many read errors (probably stalled for > 10 seconds)
		//5 * 2 second read timeout
		m_save = 0x0;
		set_error( "fob::update: flock no longer responding" );
		return false;
	}

	//save last byte (which is actually first byte of next record)
	m_save = buffer[ --i ];

	//get bird address
	int address = static_cast<int>( buffer[ i - 1 ] ) - 1;

	//make sure address is valid
	if( (address < 0) || (address >= static_cast<int>( m_birds.size( ))) ) {
		//corrupt data, do nothing
		std::cerr << "fob::update: warning: bad bird address: "
			<< address << " bytes read: " << i << std::endl;
		return true;
	}
	
	//send buffer to bird (bird makes sure data is sane)
	//DEBUG( "fob::update: record address: " << address );
	//DEBUG( "fob::update: size: " << i - 1 << " save: " << (int)m_save );
	m_birds[ address ]->update( buffer, i - 1 );
	
	//success
	return true;
}



///////////////////////////////////////////////////////////////////////////////
extern "C" {
	void*
	get_flock_data( void *data )
	{
		//when pthread_cancel is called, cancel immidiately
		pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
		
		//data is really a pointer to a fob
		fob *flock = (fob*)data;

		//update forever, parent object will call pthread_cancel( )
		while( 1 ) {
			flock->update( );
		}
	}
}

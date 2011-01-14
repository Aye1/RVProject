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

//FILE:         args.h
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include <iostream>
#include <unistd.h>
#include "commandoptions.h"
#include "fob/fob.h"

///////////////////////////////////////////////////////////////////////////////
struct cmd_args {
	std::string serial;
	unsigned long sleep_ms;
	bool old;
	fob::port_speed speed;

	cmd_args( void ): 
		serial( "/dev/ttyS0" ), 
		sleep_ms( 500 ),
		old( false ),
		speed( fob::FAST )
	{ }

	bool parse( int argc, const char *argv[ ] );
};



///////////////////////////////////////////////////////////////////////////////
bool
cmd_args::parse( int argc, const char *argv[ ] )
{
	//command line parser object
	commandoptions c;

	//register flags/options
	c.register_option( sleep_ms, "sleep", 's', "Time to sleep between commands (ms)", "MILLISECONDS" );
	c.register_flag( old, "old", 'o', "Compensate for older flock firmware" );
	c.register_argument( serial, "device", "Serial port flock is connected too" );

	try {
		c.process_command_line( argc, argv );
	} catch( commandoptions_error& ex ) {
		std::cerr << "error: " << ex.what( ) << std::endl;
		return false;
	}

	//check for old hardware
	if( old ) speed = fob::SLOW;

	//success
	return true;
}

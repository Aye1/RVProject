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

//FILE:         fly.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "fob/fob.h"
#include "args.h"

//global fun
fob flock;
bool quit = false;



//////////////////////////////////////////////////////////////////////////
//signal handler to catch CTRL-C
void 
catch_ctrl_c( int ) {
	quit = true;
}



//////////////////////////////////////////////////////////////////////////
std::string
get_button_str( unsigned char btn )
{
	if( 0 == btn ) {
		return "NON";
	} else if( fob::BUTTON_LEFT == btn ) {
		return "LFT";
	} else if( fob::BUTTON_RIGHT == btn ) {
		return "RGT";
	} else if( fob::BUTTON_MIDDLE == btn ) {
		return "MID";
	} else {
		return "UNK";
	}
}



//////////////////////////////////////////////////////////////////////////
int 
main( int argc, char *argv[] )
{
	//set ctrl-c signal handler
	signal( SIGINT, catch_ctrl_c );
	
	//parse arguments
	cmd_args args;
	if( !args.parse( argc, (const char**)argv ) ) {
		return 1;
	}

	//local vars
	fob::hemisphere hemisphere = fob::DOWN;
	
	//talk to flock
	flock.open( args.serial, hemisphere, args.speed, args.sleep_ms );
	if( !flock ) {
		std::cerr << "fatal: " << flock.get_error( ) << std::endl;
		return 1;
	}

	//get a list of birds connected to the machine
	fob::bird_list& birds = flock.get_birds( );

	//report how many birds are present
	std::cout << "number of birds: " << birds.size( ) << std::endl;

	//for each bird, set that we want position and button information
	for( unsigned int i = 0; i < birds.size( ); ++i ) {
		if( !birds[ i ]->set_mode( fob::POSITION | fob::BUTTONS ) ) {
			std::cerr << "fatal: " << flock.get_error( ) << std::endl;
			return 1;
		}
	}

	//birds configured, set the flock flying
	flock.fly( );

	//let the bird start up . . .
	sleep( 1 );

	//everything looks good, tell the user what's going on
	std::cerr << birds.size( ) << " birds opened on " << args.serial << std::endl;
	std::cerr << "Reporting position/button information" << std::endl;
	std::cerr << "(Press CTRL-C to quit)" << std::endl;

	//report position and orientation for each bird
	float pos[ 3 ];
	unsigned char button;
	std::string button_str;

	while( !quit ) {
		for( unsigned int i = 0; i < birds.size( ); ++i ) {
			//get position and button information from the bird
			birds[ i ]->get_position( pos );
			button = birds[ i ]->get_buttons( );

			//get a string to describe the button pressed
			button_str = get_button_str( button );

			//report
			fprintf( stderr, "x: %6.2f y: %6.2f z: %6.2f b: %s ",
				pos[ 0 ], pos[ 1 ], pos[ 2 ], button_str.c_str( ) );
		}

		for( unsigned int j = 0; j < 37 * birds.size( ); ++j ) {
			fprintf( stderr, "\b" );
		}
	}

	//shutdown the flock
	flock.close( );

	//success, no errors
	return 0;
}
